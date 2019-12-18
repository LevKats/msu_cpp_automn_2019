#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <utility>
#include "ThreadPool.h"

struct A {};

void test1() {
    auto foo = [](const A&) { return 1; };
    ThreadPool pool(8);
    auto task1 = pool.exec(foo, A());
    assert(task1.get() == 1);
}

void test2() {
    ThreadPool pool(8);
    auto task2 = pool.exec([]() { return 1; });
    assert(task2.get() == 1);
}

void test3() {
    ThreadPool pool(8);
    auto task2 = pool.exec([]() { return 2; });
    auto task3 = pool.exec([]() { return 3; });
    auto task4 = pool.exec([]() { return 9; });
    assert(std::make_tuple(2, 3, 9) == std::make_tuple(task2.get(), task3.get(), task4.get()));
}


int main() {
    test1();
    test2();
    test3();
    return 0;
}

