//
// Created by os on 8/16/23.
//

#ifndef OS_PROJECT__SEM_HPP
#define OS_PROJECT__SEM_HPP
#include "../h/list.hpp"

class TCB;

class _sem{

public:
    _sem(unsigned val):value(val),closed(false){}
    ~_sem();
    int signal();
    int wait();
    int close();

    void block();
    void deblock();


    void* operator new (size_t);
    void operator delete (void* ptr) noexcept;

private:
    int value;
    bool closed;
    int numOfBlockedThreads = 0;

    List<TCB> blockedThreads;

};

#endif //OS_PROJECT__SEM_HPP
