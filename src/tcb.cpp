//
// Created by os on 8/12/23.
//
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

TCB* TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body){
    return new TCB(body,TIME_SLICE);
}

void TCB::yield() {
    //Riscv::pushRegisters();
    //TCB::dispatch();
    //Riscv::popRegisters();
    __asm__ volatile ("ecall");
}

void TCB::dispatch() {
    TCB* old = running;
    if(!old->isFinished()) {Scheduler::put(old);}
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);

}

void TCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body();
    running->setFinished(true);
    TCB::yield();//posto je ta nit finished , idemo na yield da pocne neka druga da se izvrsava
}