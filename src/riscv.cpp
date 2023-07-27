//
// Created by os on 7/26/23.
//

#include "../h/riscv.hpp"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.h"


void Riscv::handleExcEcallTrap() {
    uint64 a1 = r_a1();
    uint64 a2 = r_a2();
    uint64 a3 = r_a3();
    uint64 a4 = r_a4();
    long a0 = r_a0();

    uint64 scauseVar;
    __asm__ volatile ("csrr %[scause], scause" :[scause] "=r"(scauseVar));

    if(scauseVar == 0x08UL || scauseVar == 0x09UL){
        uint64 sepc = r_sepc() + 4;
        uint64 psw = r_sstatus();

        if(a0 == 0x01){
            void* pointer = MemoryAllocator::getInstance().alloc(a1);
            w_a0_stack((long)pointer);
        }
        else if(a0 == 0x02){
            a0 = MemoryAllocator::getInstance().free((void*)a1);
            w_a0_stack(a0);
        }else {

        }
    }


}
void Riscv::handleHardwareTrap() {}
void Riscv::handleTimerTrap() {}
