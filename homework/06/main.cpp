#include <iostream>
#include <cassert>
#include <string>
#include "format.h"

void test_no_args_empty() {
    bool error = false;
    try {
        auto text = format(nullptr);
    }
    catch (const std::runtime_error &e) {
        error = true;
    }
    assert(error);
}

void test_no_args_extra_open_parentheses() {
    bool error = false;
    try {
        auto text = format("sdklfjsldkfj { slkkldskf");
    }
    catch (const std::runtime_error &e) {
        error = true;
    }
    assert(error);
}

void test_no_args_extra_close_parentheses() {
    bool error = false;
    try {
        auto text = format("sdklfjsldkfj } slkkldskf");
    }
    catch (const std::runtime_error &e) {
        error = true;
    }
    assert(error);
}

void test_one_arg_empty() {
    bool error = false;
    try {
        auto text = format(nullptr, "one");
    }
    catch (const std::runtime_error &e) {
        error = true;
    }
    assert(error);
}

void test_one_arg_extra_open_parentheses() {
    bool error = false;
    try {
        auto text = format("sdklfjsldkfj {0} { slkkldskf", "one");
    }
    catch (const std::runtime_error &e) {
        error = true;
    }
    assert(error);
}

void test_one_arg_extra_close_parentheses() {
    bool error = false;
    try {
        auto text = format("sdklfjsldkfj {0} } slkkldskf", "one");
    }
    catch (const std::runtime_error &e) {
        error = true;
    }
    assert(error);
}

void test_one_arg_extra_indicies() {
    bool error = false;
    try {
        auto text = format("sdklfjsldkfj {0} {4} slkkldskf", "one");
    }
    catch (const std::runtime_error &e) {
        error = true;
    }
    assert(error);
}

void test_several_args_empty() {
    bool error = false;
    try {
        auto text = format(nullptr, "one", 2);
    }
    catch (const std::runtime_error &e) {
        error = true;
    }
    assert(error);
}

void test_several_args_extra_open_parentheses() {
    bool error = false;
    try {
        auto text = format("sdklfjsldkfj {0} {1} { slkkldskf", "one", 2);
    }
    catch (const std::runtime_error &e) {
        error = true;
    }
    assert(error);
}

void test_several_args_extra_close_parentheses() {
    bool error = false;
    try {
        auto text = format("sdklfjsldkfj {0} } {1} slkkldskf", "one", 2);
    }
    catch (const std::runtime_error &e) {
        error = true;
    }
    assert(error);
}

void test_several_args_extra_indicies() {
    bool error = false;
    try {
        auto text = format("sdklfjsldkfj {0} {4} {1} slkkldskf", "one", 2);
    }
    catch (const std::runtime_error &e) {
        error = true;
    }
    assert(error);
}

void test_no_args_ok() {
    auto text = format("sdklfjsldkfj  slkkldskf");
    assert(text == "sdklfjsldkfj  slkkldskf");
}

void test_one_arg_ok() {
    auto text = format("sdklfjsldkfj {0} slkklds{0}kf{0} {0}", 24);
    assert(text == "sdklfjsldkfj 24 slkklds24kf24 24");
}

void test_several_args_ok() {
    auto text = format("sdklfjsldkfj {0} slkklds{0}kf{1} {0} {1}", 24, 1);
    assert(text == "sdklfjsldkfj 24 slkklds24kf1 24 1");
}

void test_several_extra_args_ok() {
    auto text = format("sdklfjsldkfj {0} slkklds{0}kf{1} {0} {1}", 24, 1,
            "one");
    assert(text == "sdklfjsldkfj 24 slkklds24kf1 24 1");
}

void test_strange_indicies() {
    bool error = false;
    try {
        auto text = format("sdklfjsldkfj {0} {4} {1} slkkldskf", "one", 2, 2);
    }
    catch (const std::runtime_error &e) {
        error = true;
    }
    assert(error);
    error = false;
    try {
        auto text = format("sdklfjsldkfj {0} {:(} {1} slkkldskf", "one", 2, 2);
    }
    catch (const std::runtime_error &e) {
        error = true;
    }
    assert(error);
}

int main() {
    test_no_args_empty();
    test_no_args_extra_open_parentheses();
    test_no_args_extra_close_parentheses();
    test_one_arg_empty();
    test_one_arg_extra_open_parentheses();
    test_one_arg_extra_close_parentheses();
    test_one_arg_extra_indicies();
    test_several_args_empty();
    test_several_args_extra_open_parentheses();
    test_several_args_extra_close_parentheses();
    test_several_args_extra_indicies();
    test_no_args_ok();
    test_one_arg_ok();
    test_several_args_ok();
    test_several_extra_args_ok();
    test_strange_indicies();
    return 0;
}

