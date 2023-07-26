//
// Created by os on 7/26/23.
//

#ifndef OS_PROJECT_RISCV_HPP
#define OS_PROJECT_RISCV_HPP


extern "C" void supervisorTrap();

//extern "C" void handleExcEcallTrap();
//extern "C" void handleHardwareTrap();
//extern "C" void handleTimerTrap();

class Riscv{

private:
    static void handleExcEcallTrap();
    static void handleHardwareTrap();
    static void handleTimerTrap();
};

#endif //OS_PROJECT_RISCV_HPP
