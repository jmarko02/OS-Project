#include "../h/Resource.hpp"
#include "../h/print.hpp"
#include "../h/tcb.hpp"
#include "../h/scheduler.hpp"

Resourse::Resourse(int N): PeriodicThread(5), n(N){
    mutex = new Semaphore(1);
}

void Resourse::take(int num_of_instances, int time_ticks)
{
    if(time_ticks % 5 != 0) return;
    TCB::running->ticks = time_ticks;
    mutex->wait();
    
    TCB::running->resources = num_of_instances;
    if (n >= num_of_instances){
        n -= num_of_instances;
        printString1("Nit ");
        printInteger1(TCB::running->id);
        printString1(" uzima ");
        printInteger1(TCB::running->resources);
        printString1(" resursa\n");

        Elem* newElem = new Elem(TCB::running,nullptr);
        if(!firstActive) firstActive = lastActive = newElem;
        else {
            lastActive->next = newElem;
            lastActive = newElem;
        }
        
        mutex->signal();
        

    } else { //block
        TCB::running->waiting = true;
        Elem* newElem = new Elem(TCB::running,nullptr);
        if(!headBlocked) headBlocked = tailBlocked = newElem;
        else {
            tailBlocked->next = newElem;
            tailBlocked = newElem;
        }
       mutex->signal();
        TCB::timeSliceCounter = 0;
        TCB::dispatch();

    }
}
int Resourse::give_back(int num_of_instances)
{   
    if(TCB::running->resources == 0 ) return 0;

    if(TCB::running->resources == -1) return 0;

    if(num_of_instances > TCB::running->resources){
        num_of_instances = TCB::running->resources;
    }
    mutex->wait();

    TCB::running->resources -= num_of_instances;
    n += num_of_instances;
    printString1("Nit ");
    printInteger1(TCB::running->id);
    printString1(" vraca ");
    printInteger1(num_of_instances);
    printString1(" resursa \n");

    if(TCB::running->resources == 0){ //izbaci iz liste aktivnih
        Elem* pom = firstActive;
        Elem* prev = nullptr;
        while(pom && pom->thread !=TCB::running){
            prev = pom;
            pom = pom->next;
        }
        if(!prev){
            Elem* old = pom;
            firstActive = firstActive->next;
            pom = pom->next;
            old->next = nullptr;
            delete old;
        }
        else {
            Elem* old = pom;
            prev->next = pom->next;
            pom = pom->next;
            old->next = nullptr;
            delete old;
        }
    }  

    Elem* pom = headBlocked;
    Elem* prev = nullptr;

    while(pom != nullptr){
        if(pom->thread->resources <= n){
            n -= pom->thread->resources;
            pom->thread->waiting = false;
            printString1("Nit ");
            printInteger1(pom->thread->id);
            printString1(" uzima ");
            printInteger1(pom->thread->resources);
            printString1(" resursa\n");

            Elem* newElem = new Elem(TCB::running,nullptr);
            if(!firstActive) firstActive = lastActive = newElem;
            else {
                lastActive->next = newElem;
                lastActive = newElem;
            }


            Scheduler::put(pom->thread);

            if(!prev){

                Elem* old = pom;
                headBlocked = headBlocked->next;
                if(!headBlocked) tailBlocked = nullptr;
                pom = pom->next;
                old->next = nullptr;
                delete old;

            } else {
                Elem* old = pom;
                prev->next = pom->next;
                pom = pom->next;
                old->next = nullptr;
                delete old;
                
            }
        } else {
            prev = pom;
            pom = pom->next;
        }

         
    }

    mutex->signal();

    return num_of_instances;
}
void Resourse::periodicActivation()
{
    mutex->wait();

    Elem* pom = firstActive, *prev = nullptr;
    while(pom){

        if(pom->thread->ticks == 0) {
            prev = pom;
            pom = pom->next;
            continue;
        }

        pom->thread->ticks -= 5;
        if(pom->thread->ticks == 0){
           n += pom->thread->resources;

            printString1("Niti ");
            printInteger1(pom->thread->id);
            printString1(" oduzeto ");
            printInteger1(pom->thread->resources);
            printString1(" resursa \n");

            pom->thread->resources = -1;

            if(!prev){
                Elem* old = pom;
                firstActive = firstActive->next;
                pom = pom->next;
                old->next = nullptr;
                delete old;
            }
            else {
                Elem* old = pom;
                prev->next = pom->next;
                pom = pom->next;
                old->next = nullptr;
                delete old;
            }
        } else {
            prev = pom;
            pom = pom->next;
        }
    }

    pom = headBlocked;
    prev = nullptr;
    while(pom != nullptr){
        if(pom->thread->resources <= n){

            n -= pom->thread->resources;
            pom->thread->waiting = false;
            printString1("Nit ");
            printInteger1(pom->thread->id);
            printString1(" uzima ");
            printInteger1(pom->thread->resources);
            printString1(" resursa\n");

            Elem* newElem = new Elem(TCB::running,nullptr);
            if(!firstActive) firstActive = lastActive = newElem;
            else {
                lastActive->next = newElem;
                lastActive = newElem;
            }


            Scheduler::put(pom->thread);

            if(!prev){

                Elem* old = pom;
                headBlocked = headBlocked->next;
                if(!headBlocked) tailBlocked = nullptr;
                pom = pom->next;
                old->next = nullptr;
                delete old;

            } else {
                Elem* old = pom;
                prev->next = pom->next;
                pom = pom->next;
                old->next = nullptr;
                delete old;
                
            }
        } else {
            prev = pom;
            pom = pom->next;
        } 
    }

    mutex->signal();

}
