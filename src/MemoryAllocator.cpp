//
// Created by os on 7/7/23.
//

#include "../h/MemoryAllocator.h"

MemoryAllocator::MemoryAllocator() {
    uint64 heapSize = (uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR; //in bytes
    uint64 blocksOnHeap = heapSize / MEM_BLOCK_SIZE; //+1?

    freeHead = (FreeNode*)HEAP_START_ADDR;
    freeHead->size = blocksOnHeap;
    freeHead->next = nullptr;
    freeHead->prev = nullptr;
}

MemoryAllocator& MemoryAllocator::getInstance() {
    static MemoryAllocator instance;
    return instance;
}

size_t MemoryAllocator::BytesToBlocks(size_t bytes) {
    return (bytes + MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE  + 1;
}

void *MemoryAllocator::alloc(size_t sizeInBytes) {
    size_t blockCount = BytesToBlocks(sizeInBytes);

    for(FreeNode* cur = freeHead; cur != nullptr; cur = cur->next){
        if(cur->size >= blockCount) { //first fit
            //*(size_t*)cur = blockCount;
            //*cur = blockCount;

            //dodaj : ako ostaje samo 1 blok, npr ako imam mesta za 10 blokova a alociram 9, onda alociraj svih 10 jer mi jedan ne znaci nista,2 vec znace
            if(cur->size - blockCount <= 1){
                *(size_t*)cur = blockCount;
                //pogledaj analogno dole if(!cur->prev) i tako odradi i za ovaj slucaj
                /*if(freeHead == cur ){
                    if(cur->next) freeHead = cur->next;
                    else freeHead = nullptr;
                } else if (cur->prev){
                    cur->prev->next = cur->next;
                }
                //
                cur->next = nullptr;
                cur->prev = nullptr;
                 */
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
                //newFragment->size = cur->size;
                newFragment->next = cur->next;
                newFragment->prev = cur->prev;
                *(size_t*)cur = blockCount;
                void* returnPointer = (char*)cur + 1*MEM_BLOCK_SIZE;
                return returnPointer;

            }


        }
    }
    return nullptr;
}

void MemoryAllocator::free(void* pointer) {

    size_t blockCount = *( (size_t*) ( (char*)pointer -1*MEM_BLOCK_SIZE) );
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

}

void MemoryAllocator::join(MemoryAllocator::FreeNode *ptr) {
    if(!ptr) return ;
    if( ptr->next && (char*)ptr + ptr->size*MEM_BLOCK_SIZE == (char*)(ptr->next)){
        ptr->size += ptr->next->size;
        ptr->next = ptr->next->next;

        if(ptr->next) ptr->next->prev = ptr;
    }
}
