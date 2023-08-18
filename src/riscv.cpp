//
// Created by os on 7/26/23.
//

#include"../h/riscv.hpp"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/print.hpp"
//#include "../h/syscall_c.h"
#include "../h/_sem.hpp"

bool Riscv::userMode = false;
SleepingThreadList Riscv::sleepingThreads;
List<_sem>* Riscv::closedSemaphores;

void Riscv::setMode(bool value){
    userMode = value;
}

void Riscv::popSppSpie() { //mora biti non inlline, mora zaista da se pozove ova fja

    if(userMode){
        mc_sstatus(Riscv::SSTATUS_SPP);

    } else {
        ms_sstatus(Riscv::SSTATUS_SPP);
    }
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile("sret");
}


void Riscv::handleExcEcallTrap() {

    uint64 volatile a1 = r_a1();
    uint64 volatile a2 = r_a2();
    uint64 volatile a3 = r_a3();
    uint64 volatile a4 = r_a4();
    long volatile a0 = r_a0();

    uint64 volatile scauseVar;

    __asm__ volatile ("csrr %[scause], scause" :[scause] "=r"(scauseVar));

    uint64 volatile sepc = r_sepc() + 4;
    uint64 volatile sstatus = r_sstatus();//psw


    if(scauseVar == 0x0000000000000008UL || scauseVar == 0x0000000000000009UL){ //S-mode(9), U-mode(8)


        //printInteger(a0);
        //printString("\n");
       //uint64 volatile sstatus = r_sstatus();//psw

        /* //ZA ASINHRONU
        uint64 volatile sepc = r_sepc() + 4; //
        uint64 volatile sstatus = r_sstatus();//psw
        TCB::timeSliceCounter = 0;
        TCB::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);
         */
        //zasto je ovo iznad uglavnom izostavljeno iz projekata koje ljudi stave na si wiki?

        if(a0 == 0x01){
            //printInteger(a0);
            void* pointer = MemoryAllocator::getInstance().alloc(a1);
            w_a0_stack((long)pointer);
        }
        else if(a0 == 0x02){
            //printInteger(a0);
            a0 = MemoryAllocator::getInstance().free((void*)a1);

            w_a0_stack(a0);

        }else if(a0 == 0x11){ //thread_create

            TCB** handle = (TCB**)a1;
            char* stack = (char*)a2;
            TCB::Body start_routine = (TCB::Body)a3;
            void* arg = (void*)a4;

            *handle = TCB::threadCreate(stack,start_routine,arg);
            uint64 ret = 0;
            if(handle == nullptr) ret = -1;
            w_a0_stack(ret);

        } else if(a0 == 0x12) { //thread_exit
            TCB::running->setFinished(true);
            TCB::dispatch();

        } else if(a0 == 0x13){ //thread dispatch()
            TCB::dispatch();

        } else if(a0 == 0x14){ //thread_join
            TCB* handle = (TCB*)a1;
            while(!handle->isFinished()) {
                TCB::dispatch();
            }
        } else if (a0 == 0x21) { //sem_open
            _sem* handle = new _sem((unsigned)a2);
            _sem** h = (_sem**)a1;
            *(h) = handle;
            w_a0_stack(0); //je l treba nekako da se vrati -1 kao kod greske? kako bi doslo do greske?

        } else if (a0 == 0x22) { //sem_close
            _sem* handle = (_sem*)a1;
            if(!handle) w_a0_stack(-1);
            else {
                a0 = handle->close();
                w_a0_stack(a0);
            }
        } else if (a0 == 0x23) { //sem_wait
            _sem* handle = (_sem*)a1;
            if(!handle) w_a0_stack(-1);
            else {
                a0 = handle->wait();
                w_a0_stack(a0);
            }
        }else if (a0 == 0x24) { //sem_signal
            _sem* handle = (_sem*)a1;
            if(!handle) w_a0_stack(-1);
            else {
                a0 = handle->signal();
                w_a0_stack(a0);
            }
        }else if (a0 == 0x31) { //time_sleep
            time_t slice = (time_t)a1;

            if (slice != 0) {
                TCB::running->setSleeping(true);
                Riscv::sleepingThreads.put(TCB::running,slice);
            }

            TCB::dispatch();

        }else if(a0 == 0x41){ //getc
            char c = __getc();
            w_a0_stack((long)c);

        } else if(a0 == 0x42){ //putc
            __putc((char)a1);
        }
        w_sstatus(sstatus);
        w_sepc(sepc);

    } else { //unexpected trap cause
        //print scause, sepc and stval
        printString1("ERR: \nscause: ");
        printInteger1(scauseVar);
        printString1("\nsepc: ");
        printInteger1(sepc);
        printString1("\nsstatus: ");
        printInteger1(sstatus);
        printString1("\n");
        while (true);
    }


}
void Riscv::handleExternalTrap() {
    uint64 scauseVar;
    __asm__ volatile ("csrr %[scause], scause" :[scause] "=r"(scauseVar));
    if(scauseVar == 0x8000000000000009UL){ //supervisor external interrupt
        console_handler();
    }//dok se ne stigne do konzole
}

void Riscv::handleTimerTrap() {
    uint64 scauseVar;
    __asm__ volatile ("csrr %[scause], scause" :[scause] "=r"(scauseVar));
    if(scauseVar == 0x8000000000000001UL){ //supervisor software interrupt(timer)
        //...
        time_t volatile temp = Riscv::sleepingThreads.peekFirstSlice();
        time_t t1 = -1;

        if(temp != t1){
            Riscv::sleepingThreads.decFirst();
            if(Riscv::sleepingThreads.peekFirstSlice() == 0){
                Riscv::sleepingThreads.removeFinishedThreads();
            }
        }
        mc_sip(SIP_SSIP);
        //ZA ASINHRONU
        TCB::timeSliceCounter++;
        if(TCB::timeSliceCounter >= TCB::running->getTimeSlice()) {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
       // mc_sip(SIP_SSIP); //brisanje bita u supervisor interrupt pending registru
    }
}
