#include "linear_allocator.h"

LinearAllocator::LinearAllocator(std::size_t size) : size(size)
{ 
    buf = new char[size];
    pos = buf;
}

LinearAllocator::~LinearAllocator() {
    delete[] buf;
}

char* LinearAllocator::alloc(std::size_t request_size) {
    if (request_size == 0) { return nullptr; }
    if (size - (pos - buf) < request_size)
        return nullptr;
    char* result = pos;
    pos += request_size;
    return result;
}

void LinearAllocator::reset() {
    pos = buf;
}

