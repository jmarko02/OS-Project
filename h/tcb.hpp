//
// Created by os on 8/12/23.
//

#ifndef OS_PROJECT_TCB_HPP
#define OS_PROJECT_TCB_HPP

#include "../lib/hw.h"
#include "../h/scheduler.hpp"

class TCB{
public:

    ~TCB() { delete[] stack;}

    using Body = void (*)(void*);
    static TCB *createThread(Body body);

    static TCB *threadCreate(char* stack, Body body, void* arg);

    bool isSleeping() const { return sleeping;}
    void setSleeping(bool sleeping) {TCB::sleeping = sleeping;}

    bool isFinished() const { return finished;}
    void setFinished (bool finished){ TCB::finished = finished;}

    bool isBlocked() const { return blocked;}
    void setBlocked(bool blocked){TCB::blocked = blocked;}

    uint64 getTimeSlice() const { return timeSlice;} // ZA ASINHRONU

    static void yield();

    static TCB* running;

    bool userMode;

    void* operator new (size_t);
    void operator delete (void* ptr) noexcept;


private:
    /*explicit TCB(Body body, uint64 timeSlice) : //ZA ASINHRONU
        body(body),
        stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
        context({
                        (uint64)&threadWrapper,
                    stack != nullptr ? (uint64)&stack[STACK_SIZE]: 0
            }),
        timeSlice(timeSlice),
        finished(false)
    {
        if(body != nullptr) Scheduler::put(this);
    }*/
    /*explicit TCB(Body body) : //ZA ASINHRONU
            body(body),
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),

            context({
                            (uint64)&threadWrapper,
                            stack != nullptr ? (uint64)&stack[STACK_SIZE]: 0
                    }),
            finished(false)
    {
        if(body != nullptr) Scheduler::put(this);
    }*/
public:
    TCB(char* stack, Body body, void* arg ):
            userMode(stack != nullptr),
            stack(stack),
            body(body),
            arg(arg),
            context({
                (uint64)&threadWrapper,
                stack != nullptr ? (uint64)&this->stack[DEFAULT_STACK_SIZE]: 0
            }),
            timeSlice(DEFAULT_TIME_SLICE),
            finished(false),
            blocked(false),
            sleeping(false)
    {
        if(body != nullptr) Scheduler::put(this);

    }
private:
    char *stack; //bice char* a ne uint64*
    Body body;//za svaku coroutine pamtimo koje telo ona izvrsava
    void* arg;
    struct Context {
        uint64 ra;
        uint64 sp;
    }; //ostale registre cuvamo na steku
    Context context;
    time_t timeSlice; //ZA ASINHRONU
    bool finished;
    bool blocked;
    bool sleeping;
    //cemu filipu sluze finished i sleeping?

    friend class _sem;
    friend class Riscv;

    static void threadWrapper(); //staticka metoda koja se prva izvrsava za svaku NOVONAPRAVLJENU nit,pozivace body koje je nama bitno da se izvrsava
    //uvek cemo zeleti da izvrsavanje zapocne od threadWrappera

    static void contextSwitch(Context* oldContext, Context* runningContext);

    static void dispatch();//vrsi samu promenu konteksta, poziva se u yield

    static time_t timeSliceCounter; //ZA ASINHRONU

};

#endif //OS_PROJECT_TCB_HPP
