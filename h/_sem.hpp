//
// Created by os on 8/16/23.
//

#ifndef OS_PROJECT__SEM_HPP
#define OS_PROJECT__SEM_HPP
#include "../h/list.hpp"

class TCB;

class _sem{

public:

    struct Blocked {
        TCB* thread;
        Blocked* next = nullptr;

        Blocked(TCB* thread, Blocked* next):thread(thread),next(next){}

        void* operator new (size_t size);
        void operator delete(void* ptr) noexcept ;
    };

    Blocked* head= nullptr;
    Blocked* tail = nullptr;

    _sem(unsigned val):value(val),closed(false){}
    ~_sem();
    int signal();
    int wait();
    int close();

    void block();
    void deblock();

    TCB* getFirst();
    void putLast(TCB* thread);

    void* operator new (size_t);
    void operator delete (void* ptr) noexcept;

private:
    int value;
    bool closed;
    int numOfBlockedThreads = 0;

    List<TCB> blockedThreads;
};

#endif //OS_PROJECT__SEM_HPP
