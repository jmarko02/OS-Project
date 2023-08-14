//
// Created by os on 7/26/23.
//

#include "../h/riscv.hpp"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"

void Riscv::popSppSpie() { //mora biti non inlline, mora zaista da se pozove ova fja
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleExcEcallTrap() {

    uint64 a1 = r_a1();
    /*uint64 a2 = r_a2();
    uint64 a3 = r_a3();
    uint64 a4 = r_a4();*/
    long a0 = r_a0();

    uint64 scauseVar;
    __asm__ volatile ("csrr %[scause], scause" :[scause] "=r"(scauseVar));

    if(scauseVar == 0x0000000000000008UL || scauseVar == 0x0000000000000009UL){ //S-mode(9), U-mode(8)

        uint64 sepc = r_sepc() + 4; //
        uint64 sstatus = r_sstatus();//psw
        TCB::timeSliceCounter = 0;
        TCB::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);
        //zasto je ovo iznad uglavnom izostavljeno iz projekata koje ljudi stave na si wiki?
        if(a0 == 0x01){
            void* pointer = MemoryAllocator::getInstance().alloc(a1);
            w_a0_stack((long)pointer);
        }
        else if(a0 == 0x02){
            a0 = MemoryAllocator::getInstance().free((void*)a1);
            w_a0_stack(a0);
        }else {

        }

    } else { //unexpected trap cause
        //print scause, sepc and stval
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
        TCB::timeSliceCounter++;
        if(TCB::timeSliceCounter >= TCB::running->getTimeSlice()){
            uint64 sepc = r_sepc();
            uint64 sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }

        mc_sip(SIP_SSIP); //brisanje bita u supervisor interrupt pending registru
    }
}
