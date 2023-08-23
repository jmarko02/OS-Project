//
// Created by os on 8/23/23.
//

#ifndef OS_PROJECT_BOUNDEDBUFFER_HPP
#define OS_PROJECT_BOUNDEDBUFFER_HPP

#include "../h/_sem.hpp"
#include "../lib/hw.h"

class BoundedBuffer{
public:
    BoundedBuffer();
    char getChar();
    void putChar(char c);

    bool empty() const ;
    bool full() const ;
private:

    _sem *itemAvailable;
    _sem *spaceAvailable;

    static const uint64 capacity = 1024;

    char buffer[capacity];

    size_t head ,tail,count;
};

#endif //OS_PROJECT_BOUNDEDBUFFER_HPP
