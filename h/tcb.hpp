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

    bool isFinished() const { return finished;}
    void setFinished (bool finished){ TCB::finished = finished;}

    //uint64 getTimeSlice() const { return timeSlice;} // ZA ASINHRONU

    static void yield();

    static TCB* running;

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
    TCB(char* stack, Body body, void* arg ):
            stack(stack),
            body(body),
            arg(arg),
            context({
                (uint64)&threadWrapper,
                stack != nullptr ? (uint64)&this->stack[DEFAULT_STACK_SIZE]: 0
            }),
            finished(false)
    {
        if(body != nullptr) Scheduler::put(this);

    }
    char *stack; //bice char* a ne uint64*
    Body body;//za svaku coroutine pamtimo koje telo ona izvrsava
    void* arg;
    struct Context {
        uint64 ra;
        uint64 sp;
    }; //ostale registre cuvamo na steku
    Context context;
    //uint64 timeSlice; //ZA ASINHRONU
    bool finished;


    friend class Riscv;

    static void threadWrapper(); //staticka metoda koja se prva izvrsava za svaku NOVONAPRAVLJENU nit,pozivace body koje je nama bitno da se izvrsava
    //uvek cemo zeleti da izvrsavanje zapocne od threadWrappera

    static void contextSwitch(Context* oldContext, Context* runningContext);

    static void dispatch();//vrsi samu promenu konteksta, poziva se u yield

    //static uint64 timeSliceCounter; //ZA ASINHRONU

    static uint64 constexpr STACK_SIZE  = 1024;
    // static uint64 constexpr TIME_SLICE = 2; //ZA ASINHRONU

};

#endif //OS_PROJECT_TCB_HPP
