#include "../lib/console.h"
#include "../h/MemoryAllocator.h"
void  main() {
    MemoryAllocator mem = MemoryAllocator::getInstance();
    void *first = mem.alloc(200);
    mem.free(first);
}
