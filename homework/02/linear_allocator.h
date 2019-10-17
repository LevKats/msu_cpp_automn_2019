#pragma once

#include <cstddef>

class LinearAllocator {
private:
    char *buf, *pos;

    std::size_t size;
public:
    LinearAllocator(std::size_t);

    char* alloc(std::size_t);

    void reset();

    ~LinearAllocator();
};

