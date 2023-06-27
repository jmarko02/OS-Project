//
// Created by os on 6/27/23.
//

#ifndef OS_PROJECT_MEMORYALLOCATOR_H
#define OS_PROJECT_MEMORYALLOCATOR_H

#include "../lib/hw.h"

class MemoryAllocator {
private:
    struct Node{
        size_t size;
        Node* next;
    };

public:
    Node* free_head;
    Node* used_head;
    //PRAVI SIGNLETON!!!

};

#endif //OS_PROJECT_MEMORYALLOCATOR_H
