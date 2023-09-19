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
    ((Thread*)thread)->run();
}

void Thread::join() {
    thread_join(myHandle);
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

int Thread::getThreadId(){
    return thread_id();
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

PeriodicThread::Elem* PeriodicThread::head = nullptr;
PeriodicThread::Elem* PeriodicThread::tail = nullptr;

void PeriodicThread::terminate() {
    period = 0;
}

void PeriodicThread::stopThread()
{
    terminate();
}
PeriodicThread::PeriodicThread(time_t period) {
    this->period = period;
    addLast(this);
}

void PeriodicThread::addLast(PeriodicThread * t)
{
    Elem* node = new Elem(t);
    if(tail != nullptr){
        tail->next = node;
        tail = node;
    } else {
        tail = head = node;
    }
}
PeriodicThread * PeriodicThread::getFirst()
{
    if(!head) return 0;

    Elem* node = head;
    head = head->next;
    if(!head) tail = 0;

    PeriodicThread* ret = node->thread;
    delete node;
    return ret;
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
