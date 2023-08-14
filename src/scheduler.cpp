//
// Created by os on 8/14/23.
//
#include "../h/scheduler.hpp"
#include "../h/MemoryAllocator.hpp"

Node* Scheduler::head = nullptr;
Node* Scheduler::tail = nullptr;

CCB *Scheduler::get() {
    if( !head ){return 0;}

    Node *node = head;
    head = head->next;
    if(!head) {tail = 0;}

    CCB* ret = node->data;
    //MemoryAllocator::getInstance().free(node);
    delete node; //umesto ove linije iznad
    return ret;

}

void Scheduler::put(CCB *ccb) {
    //Node *node = (Node*)MemoryAllocator::getInstance().alloc((sizeof(Node)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    Node* node = new Node(ccb,0); // ova linija umesto ove iznad
    //sledece dve linije umesto ktora u structuri
    node->data = ccb;
    node->next = 0;
    if(tail != nullptr){
        tail->next = node;
        tail = node;
    } else {
        tail = head = node;
    }
}
