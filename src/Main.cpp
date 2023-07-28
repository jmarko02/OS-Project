#include "../lib/console.h"
#include "../h/MemoryAllocator.h"
#include "../h/riscv.hpp"

//extern "C" void supervisorTrap();

//extern "C" void handleExcEcallTrap(){}
//extern "C" void handleHardwareTrap(){}
//extern "C" void handleTimerTrap(){}

void  main() {
    /*
    MemoryAllocator mem = MemoryAllocator::getInstance();

   void *one = mem.alloc(200);
    void *two = mem.alloc(200);
    void *three = mem.alloc(500);
    void *four = mem.alloc(150);
    void *five = mem.alloc(60);

    int x = mem.free(two);
    void* six = mem.alloc(500);
    x =mem.free(four);
    x =mem.free(three);
    void* seven = mem.alloc(200);
    void* eight = mem.alloc(10000);
    void* nine = mem.alloc(1000);
    x =mem.free(one);
    x =mem.free(five);
    x =mem.free(six);
    x =mem.free(seven);
    x =mem.free(eight);
    x =mem.free(nine);
    */
   /*
    void *one = mem.alloc(200);
    void *two = mem.alloc(100);
    void *three = mem.alloc(500);

    mem.free(one);
    void *four = mem.alloc(200);
    mem.free(two);
    void *five = mem.alloc(100);
    mem.free(four);
    mem.free(five);
    void* six = mem.alloc(300);
    mem.free(six);
    mem.free(three);
    */

   __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r"((uint64)&Riscv::supervisorTrap+1));
   uint64 mask = 0x02;
   __asm__ volatile("csrs sstatus, %0" : : "r"(mask));
   while(1){}

}
