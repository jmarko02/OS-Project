#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"

//extern "C" void supervisorTrap();

//extern "C" void handleExcEcallTrap(){}
//extern "C" void handleHardwareTrap(){}
//extern "C" void handleTimerTrap(){}

#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"

#include "../h/syscall_c.h"

extern void userMain();


void userWrapper(void* arg){
    userMain();
}

int  main() {
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

   /*
   __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r"((uint64)&Riscv::supervisorTrap+1));
   uint64 mask = 0x02;
   __asm__ volatile("csrs sstatus, %0" : : "r"(mask));
   while(1){}
  */


   /* // ZA ASINHRONU
    TCB* threads[5];

    threads[0] = TCB::createThread(nullptr);
    TCB::running = threads[0];


   Riscv::w_stvec((uint64)&Riscv::supervisorTrap+1);
   Riscv::ms_sstatus(Riscv::SSTATUS_SIE);


    threads[1] = TCB::createThread(workerBodyA);
    printString("ThreadA created\n");

    threads[2] = TCB::createThread(workerBodyB);
    printString("ThreadB created\n");

    threads[3] = TCB::createThread(workerBodyC);
   printString("ThreadC created\n");


    threads[4] = TCB::createThread(workerBodyD);
    printString("ThreadD created\n");

     //main metoda prepusta procesor napravljenim korutinama

     while(! (threads[1]->isFinished() &&
                threads[2]->isFinished() &&
                threads[3]->isFinished() &&
                threads[4]->isFinished()))
     {
         TCB::yield();
     }

    for(auto &coroutine : threads){
        delete coroutine;
    }

    printString("FInished\n");
    return 0;
    */

    /*
   //za SINHRONU
    TCB* threads[3];

    threads[0] = TCB::createThread(nullptr);
    TCB::running = threads[0];

    threads[1] = TCB::createThread(workerBodyC);
    printString("ThreadC created\n");

    threads[2] = TCB::createThread(workerBodyD);
    printString("ThreadD created\n");

    while(! (threads[1]->isFinished() &&
             threads[2]->isFinished() ))
    {
        TCB::yield();
    }

    for(auto &coroutine : threads){
        delete coroutine;
    }
    printString("Finished\n");
    return 0;
     */

    Riscv::w_stvec((uint64)&Riscv::supervisorTrap+1);

    /*
    void* first = mem_alloc(100);
    mem_free(first);
    */


    /* //ZA THREAD_CREATE
    TCB* handle ;
    uint64 ret = thread_create(&handle,nullptr,nullptr);
    printInteger(ret);*/

    TCB* threads[2];

    thread_create(&threads[0],nullptr,nullptr);
    TCB::running = threads[0];

    Riscv::setMode(true);

    thread_create(&threads[1], userWrapper,nullptr);

    thread_join(threads[1]);


    return 0;
}
