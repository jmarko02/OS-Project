#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/print.hpp"
#include "../h/syscall_cpp.hpp"

//extern "C" void supervisorTrap();

//extern "C" void handleExcEcallTrap(){}
//extern "C" void handleHardwareTrap(){}
//extern "C" void handleTimerTrap(){}

#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"

#include "../h/syscall_c.h"

//ZA TESTOVE
extern void userMain();

//ZA TESTOVE
void userWrapper(void* arg){
    userMain();
}

bool finished = false;

void ConsoleThread(void*){
    while(!Riscv::outputBuffer->empty() || !finished){
        while(!Riscv::outputBuffer->empty() && *((char*)(CONSOLE_STATUS)) & CONSOLE_TX_STATUS_BIT){
            char c = Riscv::outputBuffer->getChar();
            *(char*)CONSOLE_TX_DATA = c;
        }
        thread_dispatch();
    }
}


void worker1(void*){
    for(int i = 0 ; i < 10; i++){
        printString1("1: i=");
        printInteger1(i);
        printString1("\n");
        time_sleep(10);
    }
    thread_exit();
}
void worker2(void*){
    for(int i=0; i < 16; i++){
        printString1("2: i=");
        printInteger1(i);
        printString1("\n");
        time_sleep(20);

    }
    thread_exit();
}

class PeriNit : public PeriodicThread {
    void periodicActivation() override;
public:
    PeriNit() : PeriodicThread(5){}
};

void PeriNit::periodicActivation() {
    printString1("Yee");
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

    //Riscv::w_stvec((uint64)&Riscv::supervisorTrap+1);
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    /*
    void* first = mem_alloc(100);
    mem_free(first);
    */


    /* //ZA THREAD_CREATE
    TCB* handle ;
    uint64 ret = thread_create(&handle,nullptr,nullptr);
    printInteger(ret);*/


    //ZA SEDMI TEST (BEZ SETMODE ZA 1. I 2. TEST)


    Riscv::w_stvec((uint64)&Riscv::supervisorTrap+1);
    TCB* threads[3];

    //thread_create(&threads[0],nullptr,nullptr);
    //TCB::running = threads[0];

    TCB::running = new TCB(nullptr, nullptr, nullptr);

    Riscv::inputBuffer = new BoundedBuffer();
    Riscv::outputBuffer = new BoundedBuffer();

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    thread_create(&threads[1],ConsoleThread,nullptr);

    //PeriNit* oprana = new PeriNit();

    //oprana->start();

    thread_create(&threads[2], userWrapper,nullptr);

    thread_join(threads[2]);
    finished = true;
    //thread_dispatch();
    //thread_exit(); //nakon svakog testa izlazi error 5

    //oprana->terminate();
    /*
    Riscv::w_stvec((uint64)&Riscv::supervisorTrap+1);
    TCB* threads[3];

    thread_create(&threads[0],nullptr,nullptr);
    TCB::running = threads[0];
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    printString1("AAA\n");
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    thread_create(&threads[1], ::worker1,nullptr);

    thread_create(&threads[2], ::worker2,nullptr);

    printString1("AAA\n");

    //thread_join(threads[1]);
    printString1("AAA\n");
    thread_join(threads[2]);
    */ //kada se stavi join ne radi, kada se ne stavi join ->radi sleep lepo ;
    // kako su svi testovi radili iako sam imao join na kraju main

    return 0;
}
