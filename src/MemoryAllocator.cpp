//
// Created by os on 7/7/23.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator::MemoryAllocator() {
    uint64 heapSize = (uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR; //in bytes
    uint64 blocksOnHeap = heapSize / MEM_BLOCK_SIZE; 

    freeHead = (FreeNode*)HEAP_START_ADDR;
    freeHead->size = blocksOnHeap;
    freeHead->next = nullptr;
    freeHead->prev = nullptr;
}

MemoryAllocator& MemoryAllocator::getInstance() {
    static MemoryAllocator instance;
    return instance;
}

void *MemoryAllocator::alloc(size_t blockCount) {

    if(blockCount <= 0 ) return nullptr;

    for(FreeNode* cur = freeHead; cur != nullptr; cur = cur->next){
        if(cur->size >= blockCount) { //first fit

             if(cur->size - blockCount <= 1){
                *(size_t*)cur = blockCount;
                if(cur->prev) cur->prev->next = cur->next;
                else freeHead = cur->next;
                if(cur->next) cur->next->prev = cur->prev;
                void* returnPointer = (char*)cur + 1*MEM_BLOCK_SIZE;
                return returnPointer;
            } else {
                FreeNode* newFragment = (FreeNode*)((char*)cur+ blockCount*MEM_BLOCK_SIZE);
                if(!cur->prev){
                    size_t savedSize = freeHead->size - blockCount;
                    freeHead = newFragment;
                    newFragment->size = savedSize;
                }
                else {
                    cur->prev->next = newFragment;
                    newFragment->size = cur->size - blockCount;
                }
                if(cur->next) cur->next->prev = newFragment;

                newFragment->next = cur->next;
                newFragment->prev = cur->prev;
                 //newFragment->size = cur->size - blockCount;
                *(size_t*)cur = blockCount;
                void* returnPointer = (char*)cur + 1*MEM_BLOCK_SIZE;
                return returnPointer;
            }
        }
    }
    return nullptr;
}

int MemoryAllocator::free(void* pointer) {

    if (pointer == nullptr || (uint64 *)pointer<(uint64*)HEAP_START_ADDR || (uint64 *)pointer>=(uint64*)HEAP_END_ADDR) return -1;

    size_t blockCount = *( (size_t*) ( (char*)pointer -1*MEM_BLOCK_SIZE) );

    auto addr = (char*)pointer - MEM_BLOCK_SIZE;
    if((char*)addr + blockCount*MEM_BLOCK_SIZE >= HEAP_END_ADDR) return -2;

    FreeNode* cur = nullptr;
    if(!freeHead || pointer < freeHead) cur = nullptr;
    else {
        for(cur = freeHead; cur->next != nullptr && pointer > cur->next; cur = cur->next);
    }
    FreeNode* newSeg = (FreeNode*)((char*)pointer - 1*MEM_BLOCK_SIZE);
    newSeg->size = blockCount;
    newSeg->prev = cur;
    if(cur) newSeg->next = cur->next;
    else newSeg->next = freeHead;
    if(newSeg->next) newSeg->next->prev = newSeg;
    if(cur) cur->next = newSeg;
    else freeHead = newSeg;

    join(newSeg);
    join(cur);
    return 0;
}

void MemoryAllocator::join(MemoryAllocator::FreeNode *ptr) {
    if(!ptr) return ;
    if( ptr->next && (char*)ptr + ptr->size*MEM_BLOCK_SIZE == (char*)(ptr->next)){
        ptr->size += ptr->next->size;
        ptr->next = ptr->next->next;

        if(ptr->next) ptr->next->prev = ptr;
    }
}
