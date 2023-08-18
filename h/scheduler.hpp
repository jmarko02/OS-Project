//
// Created by os on 8/14/23.
//

#ifndef OS_PROJECT_SCHEDULER_HPP
#define OS_PROJECT_SCHEDULER_HPP
#include "../lib/hw.h"
class TCB;
struct Node{
    TCB *data;
    Node *next;
    Node(TCB* data, Node* next) : data(data), next(next){}


    void* operator new (size_t);
    void operator delete (void* ptr) noexcept;
};

class Scheduler{
private:

    static Node *head;
    static Node *tail;
public:

    static TCB* get();
    static void put(TCB* ccb);


};
#endif //OS_PROJECT_SCHEDULER_HPP
