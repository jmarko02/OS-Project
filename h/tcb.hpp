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

    using Body = void (*)();
    static TCB *createThread(Body body);

    bool isFinished() const { return finished;}
    void setFinished (bool finished){ TCB::finished = finished;}

    uint64 getTimeSlice() const { return timeSlice;}

    static void yield();

    static TCB* running;

private:
    explicit TCB(Body body, uint64 timeSlice) :
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
    }
    struct Context {
        uint64 ra;
        uint64 sp;
    }; //ostale registre cuvamo na steku
    Body body;//za svaku coroutine pamtimo koje telo ona izvrsava
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;

    friend class Riscv;

    static void threadWrapper(); //staticka metoda koja se prva izvrsava za svaku NOVONAPRAVLJENU nit,pozivace body koje je nama bitno da se izvrsava
    //uvek cemo zeleti da izvrsavanje zapocne od threadWrappera

    static void contextSwitch(Context* oldContext, Context* runningContext);

    static void dispatch();//vrsi samu promenu konteksta, poziva se u yield

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE  = 1024;
    static uint64 constexpr TIME_SLICE = 2;

};

#endif //OS_PROJECT_TCB_HPP
