//
// Created by os on 7/26/23.
//

#include "../h/riscv.hpp"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/print.hpp"
//#include "../h/syscall_c.h"

void Riscv::popSppSpie() { //mora biti non inlline, mora zaista da se pozove ova fja
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

    if(scauseVar == 0x0000000000000008UL || scauseVar == 0x0000000000000009UL){ //S-mode(9), U-mode(8)

       uint64 volatile sepc = r_sepc() + 4;
       uint64 volatile sstatus = r_sstatus();//psw

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
            uint64* stack = (uint64*)a2;
            TCB::Body start_routine = (TCB::Body)a3;
            void* arg = (void*)a4;

            *handle = TCB::threadCreate(stack,start_routine,arg);
            uint64 ret = 0;
            if(handle == nullptr) ret = -1;
            __asm__ volatile ("mv a0, %[rVal]" : : [rVal]"r"(ret));

        } else if(a0 == 0x12) { //thread_exit
            TCB::running->setFinished(true);
            TCB* t = TCB::running;
            TCB::dispatch();
            delete t;

        } else if(a0 == 0x13){ //thread dispatch()
            TCB::dispatch();

        } else if(a0 == 0x14){
            TCB** handle = (TCB**)a1;
            while(!(*handle)->isFinished()) {
                TCB::dispatch();
            }
        }
        w_sstatus(sstatus);
        w_sepc(sepc);

    } else { //unexpected trap cause
        //print scause, sepc and stval
        printString("ERR: ");
        printInteger(scauseVar);
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
        /* //ZA ASINHRONU
        TCB::timeSliceCounter++;
        if(TCB::timeSliceCounter >= TCB::running->getTimeSlice()){
            uint64 sepc = r_sepc();
            uint64 sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        */
        mc_sip(SIP_SSIP); //brisanje bita u supervisor interrupt pending registru
    }
}
