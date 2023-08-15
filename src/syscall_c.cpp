//
// Created by os on 8/15/23.
//
#include "../h/syscall_c.h"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
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

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    uint64* stack = (uint64*)mem_alloc(DEFAULT_STACK_SIZE);
    //uint64* stack = MemoryAllocator::getInstance().alloc((DEFAULT_STACK_SIZE+ MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE+1);
    PutArguments(0x11, handle, stack, start_routine,arg);
    volatile uint64 a0;
    __asm__ volatile("mv %0, a0" : "=r"(a0));
    return a0;
}

int thread_exit(){
    PutArguments(0x12);
    return 0;
}

void thread_join(){
    PutArguments(0x13);
    return;
}

void thread_join(thread_t handle){
    PutArguments(0x14, handle);
    return;
}