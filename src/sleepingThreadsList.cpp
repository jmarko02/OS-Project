//
// Created by os on 8/17/23.
//
#include "../h/sleepingThreadsList.hpp"
#include "../h/MemoryAllocator.hpp"

void SleepingThreadList::decFirst() {
    if(head != nullptr){
        head->relativeTime-=1;
    }
}

time_t SleepingThreadList::peekFirstSlice() {
    if(!head) {
        return 0;
    }
    return head->relativeTime;
}

void SleepingThreadList::removeFinishedThreads() {
    while(head != nullptr && head->relativeTime == 0){
        Elem* elem = head;
        head = head->next;
        if(!head) {tail = nullptr;}
        elem->data->setSleeping(false);
        Scheduler::put(elem->data);
        MemoryAllocator::getInstance().free(elem);
    }
}

void SleepingThreadList::put(TCB *thread, time_t slice) {
    if(head == nullptr){
        head = (Elem*)MemoryAllocator::getInstance().alloc((sizeof(Elem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE+1);
        head->data = thread;
        head->next = nullptr;
        head->relativeTime = slice;
        tail = head;
    } else {
        Elem* temp = head;
        Elem* prev = nullptr;
        while(temp != tail && slice >= temp->relativeTime){
            slice -= temp->relativeTime;
            prev = temp;
            temp = temp->next;
        }
        if(temp == tail) {
            if(slice >= tail->relativeTime){
                Elem* novi = (Elem*)MemoryAllocator::getInstance().alloc((sizeof(Elem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE+1);
                novi->data = thread;
                novi->next = nullptr;
                novi->relativeTime = slice - tail->relativeTime;
                tail->next = novi;
                tail = novi;
            }
            else {
                Elem* novi = (Elem*)MemoryAllocator::getInstance().alloc((sizeof(Elem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE+1);
                novi->data = thread;
                novi->next = tail;
                novi->relativeTime = slice;
                if(prev != nullptr){
                    prev->next = novi;
                } else {
                    head = novi;
                }
                tail->relativeTime -= slice;
            }
        }
        else {
            Elem* novi = (Elem*)MemoryAllocator::getInstance().alloc((sizeof(Elem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE+1);
            novi->data = thread;
            novi->next  = temp;
            novi->relativeTime = slice;
            if(prev) {
                prev->next = novi;
            } else {
                head = novi;
            }
            temp->relativeTime -= slice;
        }
    }
}
