//
// Created by os on 8/23/23.
//
#include "../h/BoundedBuffer.hpp"
#include "../h/_sem.hpp"
BoundedBuffer::BoundedBuffer() : head(0), tail(0),count(0){

    itemAvailable = new _sem(0);
    spaceAvailable = new _sem(1024);
}

char BoundedBuffer::getChar() {
    itemAvailable->wait();
    char c = buffer[head];
    head = (head + 1) % capacity;
    count--;
    spaceAvailable->signal();
    return c;
}

void BoundedBuffer::putChar(char c) {
    spaceAvailable->wait();
    buffer[tail] = c;
    tail = (tail + 1) % capacity;
    count++;
    itemAvailable->signal();
}

bool BoundedBuffer::empty() const {
    return count == 0;
}

bool BoundedBuffer::full() const {
    return count == capacity;
}

