//
// Created by os on 8/14/23.
//

#ifndef OS_PROJECT_SCHEDULER_HPP
#define OS_PROJECT_SCHEDULER_HPP
class CCB;
struct Node{
    CCB *data;
    Node *next;
    Node(CCB* data, Node* next) :data(data),next(next){}
};

class Scheduler{
private:

    static Node *head;
    static Node *tail;
public:

    static CCB* get();
    static void put(CCB* ccb);
};
#endif //OS_PROJECT_SCHEDULER_HPP
