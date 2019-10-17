#include "linear_allocator.h"
#include <new>
#include <iostream>

LinearAllocator::LinearAllocator(std::size_t size) : size(size) {
    try {
        this->buf = new char[size];
    }
    catch (std::bad_alloc &ba) {
	    // std::cerr << "can't allocate memory " << ba.what() << '\n';
        throw std::out_of_range("can't allocate memory");
    }
    //todo throw something
    this->pos = this->buf;
}

LinearAllocator::~LinearAllocator() {
    delete[] this->buf;
}

char* LinearAllocator::alloc(std::size_t size) {
    if (size == 0) { return nullptr; }
    if (this->size - (this->pos - this->buf) < size)
        return nullptr;
    char* result = this->pos;
    this->pos += size;
    return result;
}

void LinearAllocator::reset() {
    this->pos = this->buf;
}

