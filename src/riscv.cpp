//
// Created by os on 7/26/23.
//

#include "../h/riscv.hpp"
#include "../lib/hw.h"


void Riscv::handleExcEcallTrap() {
    uint64 scauseVar;
    asm volatile ("csrr %[scause], scause" :[scause] "=r"(scauseVar));
}
void Riscv::handleHardwareTrap() {}
void Riscv::handleTimerTrap() {}