#include <iostream>
#include "vector.h"
#include <algorithm>
#include <cassert>
#include <vector>

template<class T1, class T2>
bool array_equal(const T1& a, const T2& b) {
    if (a.size() != b.size())
        return false;
    return std::mismatch(a.begin(), a.end(), b.begin(), b.end())
            == std::make_pair(a.end(), b.end()); //&&
           //std::mismatch(a.rbegin(), a.rend(), b.rbegin(), b.rend())
           // == std::make_pair(a.rend(), b.rend());
}

struct test {
    std::pair<int, int> p;
    int a;
    
    bool operator==(const test& b) const {
        return std::tie(a, p) == std::tie(b.a, b.p);
    }
};

void test_empty() {
    std::vector<int> v1;
    Vector<int> v2;
    assert(array_equal(v1, v2));
}

void test_empty_struct() {
    std::vector<test> v1;
    Vector<test> v2;
    assert(array_equal(v1, v2));
}

void test_push() {
    std::vector<int> v1;
    Vector<int> v2;
    for (int i = 0; i < 10; ++i) {
        v1.push_back(i);
        v2.push_back(i);
    }
    assert(array_equal(v1, v2));
}

void test_push_struct() {
    std::vector<test> v1;
    Vector<test> v2;
    for (int i = 0; i < 10; ++i) {
        test t{std::make_pair(i + 1, i - 1), i};
        v1.push_back(t);
        v2.push_back(t);
    }
    assert(array_equal(v1, v2));
}

void test_pop() {
    std::vector<int> v1;
    Vector<int> v2;
    for (int i = 0; i < 10; ++i) {
        v1.push_back(i);
        v2.push_back(i);
    }
    for (int i = 0; i < 10; ++i) {
        v1.pop_back();
        v2.pop_back();
        assert(array_equal(v1, v2));
    }
}

void test_pop_struct() {
    std::vector<test> v1;
    Vector<test> v2;
    for (int i = 0; i < 10; ++i) {
        test t{std::make_pair(i + 1, i - 1), i};
        v1.push_back(t);
        v2.push_back(t);
    }
    for (int i = 0; i < 10; ++i) {
        v1.pop_back();
        v2.pop_back();
        assert(array_equal(v1, v2));
    }
}

void test_clear() {
    std::vector<int> v1;
    Vector<int> v2;
    for (int i = 0; i < 10; ++i) {
        v1.push_back(i);
        v2.push_back(i);
    }
    v1.clear();
    v2.clear();
    v1.push_back(-1);
    v2.push_back(-1);
    assert(array_equal(v1, v2));
}

void test_clear_struct() {
    std::vector<test> v1;
    Vector<test> v2;
    for (int i = 0; i < 10; ++i) {
        test t{std::make_pair(i + 1, i - 1), i};
        v1.push_back(t);
        v2.push_back(t);
    }
    v1.clear();
    v2.clear();
    test t{std::make_pair(1 + 1, 1 - 1), 1};
    v1.push_back(t);
    v2.push_back(t);
    assert(array_equal(v1, v2));
}

void test_iterators() {
    std::vector<int> v1;
    Vector<int> v2;
    int arr[] = {7, 8, 11, 3, 4, -1, 0, 4, 6, 5};
    for (int i = 0; i < 10; ++i) {
        v1.push_back(arr[i]);
        v2.push_back(arr[i]);
    }
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    assert(array_equal(v1, v2));
}

int main(int argc, char** argv) {
    test_empty();
    test_empty_struct();
    test_push();
    test_push_struct();
    test_pop();
    test_pop_struct();
    test_clear();
    test_clear_struct();
    test_iterators();
    return 0;
}

