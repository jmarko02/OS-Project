//
// Created by os on 8/12/23.
//
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"

TCB* TCB::running = nullptr;

//uint64 TCB::timeSliceCounter = 0;

/*TCB *TCB::createThread(Body body){
    return new TCB(body);
}*/

TCB *TCB::threadCreate(uint64 *stack, TCB::Body body, void *arg) {
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
    if(!old->isFinished()) {Scheduler::put(old);}
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);

}

void TCB::threadWrapper() {
    //Riscv::popSppSpie();
    running->body(running->arg);
    //running->setFinished(true);
    //TCB::yield();//posto je ta nit finished , idemo na yield da pocne neka druga da se izvrsava
}

void *TCB::operator new(size_t size) {
    return MemoryAllocator::getInstance().alloc((size + MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE + 1);
}

void TCB::operator delete(void *ptr) noexcept {
    MemoryAllocator::getInstance().free(ptr);
}