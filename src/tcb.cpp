//
// Created by os on 8/12/23.
//
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.h"

TCB* TCB::running = nullptr;

time_t TCB::timeSliceCounter = 0;

/*TCB *TCB::createThread(Body body){
    return new TCB(body);
}*/

TCB *TCB::threadCreate(char *stack, TCB::Body body, void *arg) {
    return new TCB(stack,body,arg);
}

void TCB::yield() {
    Riscv::pushRegisters();
    TCB::dispatch();
    Riscv::popRegisters();
   // __asm__ volatile ("ecall");
}

void TCB::dispatch() {
    TCB* old = running;
    if(!old->isFinished() && !old->isBlocked()) {Scheduler::put(old);}
    running = Scheduler::get();
    if(old->isFinished()) delete old;
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    thread_exit();
//    running->setFinished(true);
//    TCB::dispatch();
    //thread_dispatch();
    //TCB::yield();//posto je ta nit finished , idemo na yield da pocne neka druga da se izvrsava
}

void *TCB::operator new(size_t size) {
    return MemoryAllocator::getInstance().alloc((size + MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE + 1);
}

void TCB::operator delete(void *ptr) noexcept {
    MemoryAllocator::getInstance().free(ptr);
}