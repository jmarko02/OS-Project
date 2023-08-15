//
// Created by os on 8/15/23.
//
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.h"
#include "../h/tcb.hpp"

void* operator new (size_t size){
    return mem_alloc(size);
}
/*
void* operator new[](size_t size){
    return mem_alloc(size);
} */
void operator delete(void* ptr){
    mem_free(ptr);
}
/*
void operator delete[](void* ptr){
    mem_free(ptr);
}*/
Thread::Thread(void (*body)(void *), void *arg) {
    myHandle == nullptr;
    thread_create(&myHandle,body, arg);
}

Thread::~Thread() {
    myHandle->setFinished(true);
    delete myHandle;
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread() {
    myHandle = nullptr;
    thread_create(&myHandle, Thread::threadWrapper, (void*)this);
}

void Thread::threadWrapper(void * thread) {
    ((Thread*)thread)->run();
}


