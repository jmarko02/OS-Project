//
// Created by os on 8/16/23.
//

#include "../h/_sem.hpp"
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/MemoryAllocator.hpp"

void *_sem::operator new(size_t size) {
    return MemoryAllocator::getInstance().alloc((size + MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE + 1);
}

void _sem::operator delete(void *ptr) noexcept {
MemoryAllocator::getInstance().free(ptr);
}

void* _sem::Blocked::operator new(size_t size) {
    return MemoryAllocator::getInstance().alloc((size +  MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE + 1);
}

void _sem::Blocked::operator delete(void *ptr) noexcept {
    MemoryAllocator::getInstance().free(ptr);
}

_sem::~_sem() {
    close();
}

int _sem::signal() {
    if(closed) return -1;
    value++;
    if(value <= 0) deblock();
    return 0;
}

int _sem::wait() {
    value--;
    if(value < 0) block();
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
    /*if(blockedThreads.peekFirst() != nullptr) {
        while(blockedThreads.peekFirst()){
            blockedThreads.peekFirst()->setBlocked(false);
            Scheduler::put(blockedThreads.peekFirst());
            blockedThreads.removeFirst();
        }
    }
    Riscv::closedSemaphores->addLast(this);
     */
    if(head != nullptr){
        while(head){
            TCB* first = getFirst();
            first->setBlocked(false);
            Scheduler::put(first);
        }
    }
    return 0;
}

void _sem::block() {

    numOfBlockedThreads++;
    TCB::running->setBlocked(true);
    putLast(TCB::running);
    //blockedThreads.addLast(TCB::running);
    //Scheduler::put(TCB::running);
    //thread_dispatch(); //sis poziv u okviru sis poziva???
    TCB::dispatch();


}

void _sem::deblock() {
    numOfBlockedThreads--;
    TCB* tmp = getFirst();
    //TCB* tmp = blockedThreads.removeFirst();
    tmp->setBlocked(false);
    Scheduler::put(tmp);
}

TCB *_sem::getFirst() {
    if(head == nullptr) return nullptr;
    TCB* ret = head->thread;
    Blocked* old = head;
    head = head->next;
    if(!head) tail = nullptr;

    delete old;
    return ret;

}

void _sem::putLast(TCB *thread) {
    Blocked* newElement = new Blocked(thread,nullptr);
    if(head == nullptr){
        head = tail = newElement;
    } else {
        tail->next = newElement;
        tail = newElement;
    }
}