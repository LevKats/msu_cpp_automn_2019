#include "linear_allocator.h"
#include <iostream>
#include <cassert>
#include <string>

void test_allocate_overflow() {
    LinearAllocator alloc(4);
    auto ptr = alloc.alloc(10);
    assert(ptr == nullptr);
}

void test_allocate_no_overflow() {
    LinearAllocator alloc(4);
    auto ptr = alloc.alloc(4);
    assert(ptr != nullptr);
}

void test_allocate_several_times() {
    LinearAllocator alloc(4);
    auto ptr = alloc.alloc(1);
    assert(ptr != nullptr);
    ptr = alloc.alloc(2);
    assert(ptr != nullptr);
    ptr = alloc.alloc(1);
    assert(ptr != nullptr);
    ptr = alloc.alloc(1);
    assert(ptr == nullptr);
}

void test_allocate_zero_block() {
    LinearAllocator alloc(4);
    auto ptr = alloc.alloc(0);
    assert(ptr == nullptr);
}

void test_reset() {
    LinearAllocator alloc(4);
    auto ptr = alloc.alloc(4);
    assert(ptr != nullptr);
    alloc.reset();
    ptr = alloc.alloc(4);
    assert(ptr != nullptr);
}

void test_memory_usage() {
    LinearAllocator alloc(6);
    auto ptr = alloc.alloc(6);
    std::string str = "hello";
    for (int i = 0; i < str.length(); ++i)
        ptr[i] = str[i];
    ptr[5] = '\0';
    std::string str2 = ptr;
    assert(str == str2);
}

void test_memory_usage_several_times() {
    LinearAllocator alloc(6);
    std::string str = "hello";
    for (int j = 0; j < 5; ++j) {
        auto ptr = alloc.alloc(6);
        for (int i = 0; i < str.length(); ++i)
            ptr[i] = str[i];
        ptr[5] = '\0';
        std::string str2 = ptr;
        assert(str == str2);
        alloc.reset();
    }
}

int main() {
    test_allocate_overflow();
    test_allocate_no_overflow();
    test_allocate_several_times();
    test_allocate_zero_block();
    test_reset();
    test_memory_usage();
    test_memory_usage_several_times();
    return 0;
}
