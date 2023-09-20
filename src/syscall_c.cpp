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

    char* stack = (char*)mem_alloc(DEFAULT_STACK_SIZE);
    PutArguments(0x11, handle, start_routine,arg, stack);
    volatile uint64 a0;
    __asm__ volatile("mv %0, a0" : "=r"(a0));
    return a0;
}

int thread_exit(){ 
    PutArguments(0x12);
    return 0;
}

void thread_dispatch(){
    PutArguments(0x13);
    return;
}

void thread_join(thread_t handle){
    PutArguments(0x14, handle);
    return;
}

void thread_join(thread_t handle, time_t time){
    PutArguments(0x15,handle,time);
    return;
}

int sem_open(sem_t* handle, unsigned init) {

    PutArguments(0x21, handle, init);
    volatile uint64 a0;
    __asm__ volatile("mv %0, a0" : "=r"(a0));
    return a0;
}

int sem_close(sem_t handle) {

    PutArguments(0x22, handle);
    volatile uint64 a0;
    __asm__ volatile("mv %0, a0" : "=r"(a0));
    return a0;
}
int sem_wait(sem_t handle) {

    PutArguments(0x23, handle);
    volatile uint64 a0;
    __asm__ volatile("mv %0, a0" : "=r"(a0));
    return a0;
}

int sem_signal(sem_t handle) {

    PutArguments(0x24, handle);
    volatile uint64 a0;
    __asm__ volatile("mv %0, a0" : "=r"(a0));
    return a0;
}

int time_sleep(time_t time) {

    PutArguments(0x31, time);
    volatile uint64 a0;
    __asm__ volatile("mv %0, a0" : "=r"(a0));
    return a0;
}

char getc() {

    PutArguments(0x41);
    volatile char a0;
    __asm__ volatile("mv %0, a0" : "=r"(a0));
    return a0;
}

void putc(char c) {
    PutArguments(0x42, c);
}