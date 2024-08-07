//
// Created by os on 8/14/23.
//
#include "../h/scheduler.hpp"
#include "../h/MemoryAllocator.hpp"

Node* Scheduler::head = nullptr;
Node* Scheduler::tail = nullptr;

TCB *Scheduler::get() {
    if( !head ){return 0;}

    Node *node = head;
    head = head->next;
    if(!head) {tail = 0;}

    TCB* ret = node->data;

    delete node; 
    return ret;

}

void Scheduler::put(TCB *tcb) {
    
    Node* node = new Node(tcb,0); 
    if(tail != nullptr){
        tail->next = node;
        tail = node;
    } else {
        tail = head = node;
    }
}

void * Node::operator new(size_t size) {
    return MemoryAllocator::getInstance().alloc((size+ MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE+1);
}

void Node::operator delete(void *ptr) noexcept{
    MemoryAllocator::getInstance().free(ptr);
}
