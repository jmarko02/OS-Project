//
// Created by os on 7/26/23.
//

#include"../h/riscv.hpp"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/_sem.hpp"

SleepingThreadList Riscv::sleepingThreads;
BoundedBuffer* Riscv::inputBuffer = nullptr;
BoundedBuffer* Riscv::outputBuffer = nullptr;
size_t Riscv::timerCounter = 0;

void Riscv::popSppSpie() { //mora biti non inline, mora zaista da se pozove ova fja

    if(TCB::running->userMode){
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

        if(a0 == 0x01){//mem_alloc

            void* pointer = MemoryAllocator::getInstance().alloc(a1);
            w_a0_stack((long)pointer);
        }
        else if(a0 == 0x02){//mem_free

            a0 = MemoryAllocator::getInstance().free((void*)a1);
            w_a0_stack(a0);

        }else if(a0 == 0x11){ //thread_create

            TCB** handle = (TCB**)a1;
            TCB::Body start_routine = (TCB::Body)a2;
            void* arg = (void*)a3;
            char* stack = (char*)a4;

            uint64 ret = 0;
            if (!handle) ret = -1;

            if (!ret) *handle = TCB::threadCreate(stack,start_routine,arg);
            if(*handle == nullptr) ret = -1;
            w_a0_stack(ret);

        } else if(a0 == 0x12) { //thread_exit

            TCB::running->setFinished(true);
            TCB::timeSliceCounter = 0;
            TCB::dispatch();

        } else if(a0 == 0x13){ //thread dispatch()
            
            TCB::timeSliceCounter = 0;
            TCB::dispatch();

        } else if(a0 == 0x14){ //thread_join
            TCB* handle = (TCB*)a1;
            if(handle != nullptr){
                 while(!handle->isFinished()) {
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
            }
            }
           
        
          
        } else if (a0 == 0x15) {
            TCB* handle = (TCB*)a1;
            time_t time = (time_t)a2;
            if(handle != nullptr && time >= 0){
                if(time == 0) {
                    while(!handle->isFinished()) {
                        TCB::timeSliceCounter = 0;
                        TCB::dispatch();
                    }
                } else {
                    TCB::running->timerCounterThread = timerCounter;
                    while(!handle->isFinished()){
                        if(timerCounter-TCB::running->timerCounterThread < time){
                            TCB::dispatch();
                        }else break;
                    }
                }
            }


        }else if (a0 == 0x21) { //sem_open
            _sem* handle = new _sem((unsigned)a2);
            if(handle == nullptr) {
                w_a0_stack(-1);
            } else {
                 _sem** h = (_sem**)a1;
                if(h == nullptr) {
                    w_a0_stack(-1);
                } else {
                    *(h) = handle;
                    w_a0_stack(0); 

                }
                
            }
           
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

            if (slice == 0) {
                w_a0_stack(-1);
            }
            if(slice != 0){
                TCB::running->setSleeping(true);
                Riscv::sleepingThreads.put(TCB::running,slice);
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
            }

        }else if(a0 == 0x41){ //getc

            char ret = -1;
            while (inputBuffer->empty()) {
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
            }
            ret = inputBuffer->getChar();
            w_a0_stack((long)ret);


        } else if(a0 == 0x42){ //putc

            char c = a1;
            while (outputBuffer->full()) {
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
            }
            outputBuffer->putChar(c);

        }
        w_sstatus(sstatus);
        w_sepc(sepc);

    } else { //unexpected trap cause
        //print scause, sepc and stval
        printString1("ERROR: scause: ");
        printInteger1(scauseVar);
        printString1("\nsepc: ");
        printInteger1(sepc);
        printString1("\nsstatus: ");
        printInteger1(sstatus);
        printString1("\n");
        while (true) flushOutputBuffer();
    }
}

void Riscv::flushOutputBuffer() {
    while(!Riscv::outputBuffer->empty() && *((char*)(CONSOLE_STATUS)) & CONSOLE_TX_STATUS_BIT){
        char c = Riscv::outputBuffer->getChar();
        *(char*)CONSOLE_TX_DATA = c;
    }
}

void Riscv::handleExternalTrap() {

    volatile int interruptNum = plic_claim();

    while(!inputBuffer->full() && *((char*)(CONSOLE_STATUS)) & CONSOLE_RX_STATUS_BIT){
        char c = (*(char*)CONSOLE_RX_DATA);
        inputBuffer->putChar(c);
    }
    plic_complete(interruptNum);

}

void Riscv::handleTimerTrap() {
    timerCounter++;
    time_t volatile temp = Riscv::sleepingThreads.peekFirstSlice();
    time_t volatile t1 = 0;
    
    if(temp != t1){
        Riscv::sleepingThreads.decFirst();
        if(Riscv::sleepingThreads.peekFirstSlice() == 0){
            Riscv::sleepingThreads.removeFinishedThreads();
        }
    }
    mc_sip(SIP_SSIP);
    
    TCB::timeSliceCounter++;
    if(TCB::timeSliceCounter >= TCB::running->getTimeSlice()) {
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        TCB::timeSliceCounter = 0;
        TCB::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
}
