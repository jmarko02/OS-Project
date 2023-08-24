//
// Created by os on 6/27/23.
//

#ifndef OS_PROJECT_MEMORYALLOCATOR_HPP
#define OS_PROJECT_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator {
private:
    struct FreeNode{
        size_t size; //size in blocks
        FreeNode* next;
        FreeNode* prev;
    };

    MemoryAllocator();
    FreeNode* freeHead;

public:
    static MemoryAllocator& getInstance();

    void join(FreeNode* ptr);

    void* alloc(size_t sizeInBlocks);
    int free(void* pointer);
};

#endif //OS_PROJECT_MEMORYALLOCATOR_HPP
