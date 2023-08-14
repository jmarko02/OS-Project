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

    //static MemoryAllocator instance ;


    MemoryAllocator();
    FreeNode* freeHead;

public:
    static MemoryAllocator& getInstance();

    size_t BytesToBlocks(size_t bytes);
    void join(FreeNode* ptr);

    void* alloc(size_t sizeInBlocks);
    int free(void* pointer);
};
//MemoryAllocator* MemoryAllocator::instance = nullptr;

#endif //OS_PROJECT_MEMORYALLOCATOR_HPP
