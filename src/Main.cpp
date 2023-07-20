#include "../lib/console.h"
#include "../h/MemoryAllocator.h"
void  main() {
    MemoryAllocator mem = MemoryAllocator::getInstance();
    void *one = mem.alloc(200);
    void *two = mem.alloc(200);
    void *three = mem.alloc(500);
    void *four = mem.alloc(150);
    void *five = mem.alloc(60);

    mem.free(two);
    void* six = mem.alloc(500);
    mem.free(four);
    mem.free(three);
    void* seven = mem.alloc(200);
    void* eight = mem.alloc(10000);
    void* nine = mem.alloc(1000);
    mem.free(one);
    mem.free(five);
    mem.free(six);
    mem.free(seven);
    mem.free(eight);
    mem.free(nine);


}
