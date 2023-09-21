//
// Created by os on 8/15/23.
//
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.h"
#include "../h/tcb.hpp"

void* operator new (size_t size){
    return mem_alloc(size);
}

void* operator new[](size_t size) {
    return mem_alloc(size);
}
void operator delete(void* ptr) {
    mem_free(ptr);
}

void operator delete[](void* ptr) {
    mem_free(ptr);
}

Thread::Thread(void (*body)(void *), void *arg):myHandle(nullptr), body(body),arg(arg) {}

Thread::Thread() : myHandle(nullptr), body(threadWrapper), arg(this){}

int Thread::start() {
    return thread_create(&myHandle,body,arg);
}

Thread::~Thread() {
    myHandle->setFinished(true);
    //delete myHandle; // ovo ne smem sad da oslobodim jer druga nit moze da dodje na isto mesto
}

void Thread::dispatch() {
    thread_dispatch();
}

void Thread::threadWrapper(void * thread) {
    mutex->wait();
    ((Thread*)thread)->run();
}

void Thread::join() {
    thread_join(myHandle);
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

int Thread::maxUserThreads = 5;
uint64 Thread::counter = 0;
uint64 Thread::maxTime = 0;
uint64 Thread::intervalTime = 0;

Semaphore* Thread::mutex = nullptr;

void Thread::SetMaximumThreads(int num_of_threads, uint64 max_time, uint64 interval_time){
    maxUserThreads = num_of_threads;
    maxTime = max_time;
    intervalTime = interval_time;
    mutex = new Semaphore(maxUserThreads);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

void PeriodicThread::terminate() {
    period = 0;
}

PeriodicThread::PeriodicThread(time_t period) {
    this->period = period;
}

void PeriodicThread::run() {
    while (period) {
        periodicActivation();
        time_sleep(period);
    }
}

PeriodicThread::~PeriodicThread() {
    terminate(); 
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}
