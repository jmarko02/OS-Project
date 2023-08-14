//
// Created by os on 8/12/23.
//

#ifndef OS_PROJECT_CCB_HPP
#define OS_PROJECT_CCB_HPP

#include "../lib/hw.h"
#include "../h/scheduler.hpp"

class CCB{
public:

    ~CCB() { delete[] stack;}

    using Body = void (*)();
    static CCB *createCoroutine(Body body);

    bool isFinished() const { return finished;}
    void setFinished (bool finished){ CCB::finished = finished;}

    static void yield();

    static CCB* running;

private:
    CCB(Body body) :
        body(body),
        stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
        context({
                    body != nullptr ? (uint64) body : 0,
                    stack != nullptr ? (uint64)&stack[STACK_SIZE]: 0
            }),
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
    bool finished;

    static void contextSwitch(Context* oldContext, Context* runningContext);

    static void dispatch();//vrsi samu promenu konteksta, poziva se u yield

    static uint64 constexpr STACK_SIZE  = 1024;

};

#endif //OS_PROJECT_CCB_HPP
