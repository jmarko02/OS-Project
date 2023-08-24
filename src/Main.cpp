#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/print.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/syscall_c.h"

extern void userMain();

void userWrapper(void* arg){
    userMain();
}

void Idle (void*){
    while(true){
        thread_dispatch();
    }
}

TCB* userThread = nullptr;

void ConsoleThread(void*){
    while(!Riscv::outputBuffer->empty() || !userThread->isFinished()) {
        Riscv::flushOutputBuffer();
        thread_dispatch();
    }
}

/*
class PeriNit : public PeriodicThread {
    void periodicActivation() override;
public:
    PeriNit() : PeriodicThread(5){}
};

void PeriNit::periodicActivation() {
    printString1("Yee");
}
*/

int  main() {

    Riscv::w_stvec((uint64)&Riscv::supervisorTrap+1);
    TCB* threads[2];

    TCB::running = new TCB(nullptr, nullptr, nullptr);

    Riscv::inputBuffer = new BoundedBuffer();
    Riscv::outputBuffer = new BoundedBuffer();

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    thread_create(&threads[0],ConsoleThread,nullptr);
    
    thread_create(&threads[1],Idle,nullptr);

    //PeriNit* oprana = new PeriNit();

    //oprana->start();

    thread_create(&userThread, userWrapper,nullptr);
    
    thread_join(threads[0]);
 
    //oprana->terminate();
    
    // Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    //delete Riscv::inputBuffer;
    //delete Riscv::outputBuffer;
    
    return 0;
}
