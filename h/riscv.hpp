//
// Created by os on 7/26/23.
//

#ifndef OS_PROJECT_RISCV_HPP
#define OS_PROJECT_RISCV_HPP


#include "../lib/hw.h"
#include "../h/_sem.hpp"
#include "../h/sleepingThreadsList.hpp"
#include "../h/BoundedBuffer.hpp"

class Riscv{
public :

    //pop sstatus.spp and sstatus.spie bits
    static void popSppSpie();

    static uint64 r_scause();
    static void w_scause(uint64 scause);

    static uint64 r_sepc();
    static void w_sepc(uint64 sepc);

    static uint64 r_stvec();
    static void w_stvec(uint64 stvec);

    static uint64 r_stval();
    static void w_stval(uint64 stval);

    enum BitMaskSip{
        SIP_SSIP = (1<<1),
        SIP_STIP = (1<<5),
        SIP_SEIP = (1<<9),
    };

    static void ms_sip(uint64 mask);
    static void mc_sip(uint64 mask);
    static uint64 r_sip();
    static void w_sip(uint64 sip);

    enum BitMaskSstatus{
        SSTATUS_SIE = (1<<1),
        SSTATUS_SPIE = (1<<5),
        SSTATUS_SPP = (1<<8),
    };

    static void ms_sstatus(uint64 mask);
    static void mc_sstatus(uint64 mask);
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

    static BoundedBuffer* inputBuffer;
    static BoundedBuffer* outputBuffer;

    static void supervisorTrap();

    static void flushOutputBuffer();
private:

    static SleepingThreadList sleepingThreads;

    static void handleExcEcallTrap();
    static void handleExternalTrap();
    static void handleTimerTrap();
};

inline uint64 Riscv::r_scause() {
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void Riscv::w_scause(uint64 scause) {
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 Riscv::r_sepc() {
    uint64 volatile sepc;
    __asm__ volatile("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}
inline void Riscv::w_sepc(uint64 sepc) {
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 Riscv::r_stvec(){
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void Riscv::w_stvec(uint64 stvec) {
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 Riscv::r_stval() {
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void Riscv::w_stval(uint64 stval) {
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void Riscv::ms_sip(uint64 mask){
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sip(uint64 mask){
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sip() {
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void Riscv::w_sip(uint64 sip) {
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void Riscv::ms_sstatus(uint64 mask) {

    __asm__ volatile("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sstatus(uint64 mask) {
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sstatus() {
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void Riscv::w_sstatus(uint64 sstatus) {
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

inline void Riscv::w_a0(long a0) {
    __asm__ volatile ("mv a0, %0" : : "r"(a0));
}
inline long Riscv::r_a0() {
    long volatile a0;
    __asm__ volatile ("ld %0, 10*8(fp)" : "=r"(a0));
    return a0;
}

inline void Riscv::w_a0_stack(long a0){
    __asm__ volatile ("sd %0,10*8(fp)" : : "r"(a0));
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
    return a2;
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

#endif //OS_PROJECT_RISCV_HPP
