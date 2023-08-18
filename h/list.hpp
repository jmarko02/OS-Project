//
// Created by os on 7/29/23.
//

#ifndef OS_PROJECT_LIST_HPP
#define OS_PROJECT_LIST_HPP

#include "../lib/mem.h"
#include "../h/MemoryAllocator.hpp"
#include "../lib/hw.h"

template<typename T>
class List{
    struct Elem
    {
        T* data;
        Elem* next;

        Elem(T* data, Elem* next) :data(data),next(next){}

        void* operator new (size_t size){
            return MemoryAllocator::getInstance().alloc((size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE+1);
        }
        void operator delete(void* ptr) noexcept {
            MemoryAllocator::getInstance().free(ptr);
        }

    };
    Elem* head, *tail;

    //void* operator new (size_t size){return alloc(size);}
    //void operator delete(void* ptr){free(ptr);}
public:
    List():head(0), tail(0){}

    List(const List<T>&) = delete;
    List<T> &operator=(const List<T>&) = delete;

    void addFirst(T* data){
        Elem* elem = new Elem(data,head);
        head = elem;
        if(!tail) {tail = head;}
    }

    void addLast(T* data){
        Elem* elem = new Elem(data,0);
        if(tail){
            tail->next = elem;
            tail = elem;
        }else {
            head = tail = elem;
        }
    }

    T* removeFirst(){
        if(!head) {return 0;}

        Elem* elem = head;
        head = head->next;
        if(!head) {tail = 0;}

        T* ret = elem->data;
        delete elem;
        return ret;
    }

    T* peekFirst(){
        if(!head) {return 0;}
        return head->data;
    }

    T* removeLast(){
        if(!head) {return 0;}

        Elem* prev = 0;
        for(Elem* curr = head; curr & curr != tail ; curr = curr->next){
            prev = curr;
        }

        Elem* elem = tail;
        if(prev){prev->next = 0;}
        else{ head = 0;}
        tail = prev;

        T* ret = elem->data;
        delete elem;
        return ret;
    }

    T* peekLast(){
        if(!tail) {return 0;}
        return tail->data;
    }
};

#endif //OS_PROJECT_LIST_HPP
