//
// Created by os on 8/16/23.
//

#include "../h/_sem.hpp"
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"

void *_sem::operator new(size_t size) {
    return MemoryAllocator::getInstance().alloc((size + MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE + 1);
}

void _sem::operator delete(void *ptr) noexcept {
MemoryAllocator::getInstance().free(ptr);
}

_sem::~_sem() {
    close();
}

int _sem::signal() {
    if(closed) return -1;
    value++;
    if(val <= 0) deblock();
    return 0;
}

int _sem::wait() {
    val--;
    if(val < 0) block();
    if(!closed) {
        return 0;
    }else {
        if(numOfBlockedThreads == 0) return 0;
        else {
            numOfBlockedThreads--;
            return -1;
        }
    }
}

int _sem::close() {
    if(closed) return -1;
    closed = true;
    if(blockedThreads.peekFirst() != nullptr) {
        while(blockedThreads.peekFirst()){
            blockedThreads.peekFirst()->setBlocked(false);
            Scheduler::put(blockedThreads.peekFirst());
            blockedThreads.removeFirst();
        }
    }
    Riscv::closedSemaphores->addLast(this);
    retunr 0;
}

void _sem::block() {
    numOfBlockedThreads++;
    TCB::running->setBlocked(true);
    Scheduler::put(TCB::running);
    thread_dispatch(); //sis poziv u okviru sis poziva???
}

void _sem::deblock() {
    numOfBlockedThreads--;
    TCB* tmp = blockedThreads.removeFirst();
    tmp->setBlocked(false);
    Scheduler::put(tmp);
}
