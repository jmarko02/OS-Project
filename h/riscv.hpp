//
// Created by os on 7/26/23.
//

#ifndef OS_PROJECT_RISCV_HPP
#define OS_PROJECT_RISCV_HPP


#include "../lib/hw.h"
extern "C" void supervisorTrap();

extern "C" void handleExcEcallTrap();
extern "C" void handleHardwareTrap();
extern "C" void handleTimerTrap();

class Riscv{
public :

    static uint64 r_scause();
    static void w_scause(uint64 scause);

    static uint64 r_sepc();
    static void w_sepc(uint64 sepc);

    static uint64 r_sstatus();
    static void w_sstatus(uint64 sstatus);




    static long r_a0();
    static void w_a0(long a0);
    static void w_a0_stack(long a0);

    static uint64 r_a1();
    static void w_a1(uint64 a1);

    static uint64 r_a2();
    static void w_a2(uint64 a2);

    static uint64 r_a3();
    static void w_a3(uint64 a3);

    static uint64 r_a4();
    static void w_a4(uint64 a4);

private:
    static void handleExcEcallTrap();
    static void handleHardwareTrap();
    static void handleTimerTrap();
};

inline void Riscv::w_scause(uint64 scause) {
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}
inline uint64 Riscv::r_scause() {
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}
inline uint64 Riscv::r_sepc() {
    uint64 volatile sepc;
    __asm__ volatile("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}
inline void Riscv::w_sepc(uint64 sepc) {
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline void Riscv::w_a0(long a0) {
    __asm__ volatile ("mv a0, %0" : : "r"(a0));
}
inline long Riscv::r_a0() {
    long volatile a0;
    __asm__ volatile ("ld %0, 10*8(fp)" : "=r"(a0));
    return a0;
}
inline void Riscv::w_a1(uint64 a1) {
    __asm__ volatile ("mv a1, %0" : : "r"(a1));
}
inline uint64 Riscv::r_a1() {
    uint64 volatile a1;
    __asm__ volatile ("ld %0, 11*8(fp)" : "=r"(a1));
    return a1;
}
inline void Riscv::w_a2(uint64 a2) {
    __asm__ volatile ("mv a2, %0" : : "r"(a2));
}
inline uint64 Riscv::r_a2() {
    uint64 volatile a2;
    __asm__ volatile ("ld %0,12*8(fp)" : "=r"(a2));
}
inline void Riscv::w_a3(uint64 a3) {
    __asm__ volatile ("mv a3, %0" : : "r"(a3));
}
inline uint64 Riscv::r_a3() {
    uint64 volatile a3;
    __asm__ volatile ("ld %0, 13*8(fp)" : "=r"(a3));
    return a3;
}
inline void Riscv::w_a4(uint64 a4) {
    __asm__ volatile("mv a4, %0" : :"r"(a4));
}
inline uint64 Riscv::r_a4() {
    uint64 volatile a4;
    __asm__ volatile("ld %0, 14*8(fp)" : "=r"(a4));
    return a4;
}
inline void Riscv::w_a0_stack(long a0){
    __asm__ volatile ("sd %0,10*8(fp)" : : "r"(a0));
}
#endif //OS_PROJECT_RISCV_HPP
