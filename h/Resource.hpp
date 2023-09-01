#ifndef RESOURCE_HPP
#define RESOURCE_HPP


#include "../h/syscall_cpp.hpp"

class Resourse : public PeriodicThread {
	public:	
        Resourse(int N);
        void take(int num_of_instances, int time_ticks);
        int give_back(int num_of_instances);

    protected:
        void periodicActivation() override;
    private:    
        int n;
        Semaphore* mutex;

        struct Elem {
            TCB* thread;
            Elem* next;

            Elem(TCB* t, Elem* next):thread(t),next(next){}
        };

        Elem* headBlocked = nullptr;
        Elem* tailBlocked = nullptr;

        Elem* firstActive = nullptr;
        Elem* lastActive = nullptr;
};

#endif //RESOURCE_HPP