//
// Created by os on 8/15/23.
//

#ifndef OS_PROJECT_SYSCALL_CPP_HPP
#define OS_PROJECT_SYSCALL_CPP_HPP

#include "syscall_c.h"
void* operator new (size_t);
void operator delete (void*);
class Semaphore;
class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    void join();
    static void dispatch ();
    static int sleep (time_t);

    static void threadWrapper(void* );

    static void SetMaximumThreads(int num_of_threads, uint64 max_time, uint64 interval_time);

    uint64 getId() const { return id;}


protected:
    Thread ();
    virtual void run () {}

    static uint64 maxTime;
    static uint64 intervalTime;
    static Semaphore* mutex;
    
    static uint64 cnt;

private:
    thread_t myHandle;
    void (*body)(void*); void* arg;

    static int maxUserThreads;

    static uint64 counter;
    uint64 id = ++counter;

};

class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};
class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
    virtual void run() override;
    virtual ~PeriodicThread() override;

private:
    time_t period;
};
class Console {
public:
    static char getc ();
    static void putc (char);
};

#endif //OS_PROJECT_SYSCALL_CPP_HPP
