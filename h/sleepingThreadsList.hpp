//
// Created by os on 8/17/23.
//

#ifndef OS_PROJECT_SLEEPINGTHREADSLIST_HPP
#define OS_PROJECT_SLEEPINGTHREADSLIST_HPP

#include "../h/scheduler.hpp"
#include "../h/tcb.hpp"

class SleepingThreadList{
public:
    SleepingThreadList():head(nullptr),tail(nullptr){}
    SleepingThreadList(const SleepingThreadList&) = delete;
    SleepingThreadList &operator=(const SleepingThreadList&) = delete;

    void decFirst();
    time_t peekFirstSlice();
    void removeFinishedThreads();

    void put(TCB* thread,time_t slice);

private:
    struct Elem{
        TCB* data;
        Elem* next;
        time_t relativeTime;

    };
    Elem *head, *tail;
};

#endif //OS_PROJECT_SLEEPINGTHREADSLIST_HPP
