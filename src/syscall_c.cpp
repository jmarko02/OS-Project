//
// Created by os on 8/15/23.
//
#include "../h/syscall_c.h"
#include "../h/riscv.hpp"
void PutArguments(short code, ...){
    __asm__ volatile("ecall");
}

void *mem_alloc(size_t size) {
    uint64 sizeInBlocks = (size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE + 1;
    PutArguments(0x01,sizeInBlocks);

    volatile uint64 a0;
    __asm__ volatile("mv %0, a0" : "=r"(a0));
    return (void*)a0;
}

int mem_free(void* ptr){
    PutArguments(0x02,(uint64)ptr);
    volatile uint64 a0;
    __asm__ volatile("mv %0, a0" : "=r"(a0));
    return a0;
}