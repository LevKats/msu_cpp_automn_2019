#include <iostream>
#include "BigInt.h"
#include <cassert>

void test_constructor() {
    BigInt a;
    assert(a.is_nan());
    
    BigInt b(a);
    assert(b.is_nan());
    
    b = 1;
    assert(b == 1);
    assert(a.is_nan());
    
    BigInt c(-12345);
    assert(c == -12345);
}

void test_nan() {
    BigInt a;
    assert(a.is_nan());
}

void test_move_copy() {
    BigInt a(4);
    assert(a == 4);
    BigInt b(std::move(a));
    assert(a.is_nan());
    assert(b == 4);
    
    a = b;
    assert(a == 4);
    assert(b == 4);
}

void test_unary_minus() {
    BigInt a(-4);
    assert(-a == 4);
    assert(-(-a) == a);
}

void test_eq_ne() {
    BigInt a(-12345);
    assert(a == -12345);
    assert(!(a != -12345));
    assert(!(a == 12345));
    assert(a != 12345);
    assert(!(a == -1));
    assert(a != -1);
    assert(-12345 == a);
    assert(!(-12345 != a));
    assert(BigInt(-12345) == a);
    assert(!(BigInt(-12345) != a));
    bool b = false;
    try {
        a == BigInt();
    }
    catch (std::domain_error &e) {
        b = true; // true only if exception happened
    }
    assert(b);
    
    b = false;
    try {
        BigInt() != a;
    }
    catch (std::domain_error &e) {
        b = true;
    }
    assert(b);
    
    b = false;
    try {
        a != BigInt();
    }
    catch (std::domain_error &e) {
        b = true;
    }
    assert(b);
    
    b = false;
    try {
        BigInt() != a;
    }
    catch (std::domain_error &e) {
        b = true;
    }
    assert(b);
}

void test_plus() {
    BigInt a(-12345);
    bool b = false;
    try {
        a + BigInt();
    }
    catch (std::domain_error &e) {
        b = true; 
    }
    assert(b);
    
    b = false;
    try {
        BigInt() + a;
    }
    catch (std::domain_error &e) {
        b = true;
    }
    assert(b); 
    
    int c = 123;
    int d = 927;
    BigInt bigc = c;
    BigInt bigd = d;
    assert(bigc + bigd == c + d);
    //std::cout << -bigc + bigd << ' '<< -c + d << std::endl;
    assert(-bigc + bigd == -c + d);
    assert(bigc + -bigd == c + -d);
    assert(-bigc + -bigd == -c + -d);
    assert(bigc + -bigc == 0);
    assert(bigd + -bigd == 0);
    assert(bigc + bigd + bigc == c + d + c);
    assert(bigd == d);
    assert(bigc == c);
}

void test_minus() {
    BigInt a(-12345);
    bool b = false;
    try {
        a - BigInt();
    }
    catch (std::domain_error &e) {
        b = true; 
    }
    assert(b);
    
    b = false;
    try {
        BigInt() - a;
    }
    catch (std::domain_error &e) {
        b = true;
    }
    assert(b);
    
    int c = 123;
    int d = 927;
    BigInt bigc = c;
    BigInt bigd = d;
    assert(bigc - bigd == c - d);
    assert(-bigc - bigd == -c - d);
    assert(bigc - -bigd == c - -d);
    assert(-bigc - -bigd == -c - -d);
    assert(bigc -bigc == 0);
    assert(bigd -bigd == 0);
    assert(bigc - bigd - bigc == c - d - c);
    assert(bigd == d);
    assert(bigc == c);
}

void test_less() {
    BigInt a(-12345);
    bool b = false;
    try {
        a < BigInt();
    }
    catch (std::domain_error &e) {
        b = true;
    }
    assert(b);
    
    b = false;
    try {
        BigInt() <= a;
    }
    catch (std::domain_error &e) {
        b = true;
    }
    assert(b);
    
    b = false;
    try {
        a <= BigInt();
    }
    catch (std::domain_error &e) {
        b = true;
    }
    assert(b);
    
    b = false;
    try {
        BigInt() <= a;
    }
    catch (std::domain_error &e) {
        b = true;
    }
    assert(b);
    
    int c = 123;
    int d = 927;
    BigInt bigc = c;
    BigInt bigd = d;
    
    assert(bigc < bigd == c <  d);
    assert(-bigc < bigd == -c < d);
    assert(bigc < -bigd == c < -d);
    assert(-bigc < -bigd == -c < -d);
    assert(bigc < bigc == c < c);
    assert(bigd < bigd == d < d);
    
    assert(bigc <= bigd == c <=  d);
    assert(-bigc <= bigd == -c <= d);
    assert(bigc <= -bigd == c <= -d);
    assert(-bigc <= -bigd == -c <= -d);
    assert(bigc <= bigc == c <= c);
    assert(bigd <= bigd == d <= d);
    
    assert(bigd == d);
    assert(bigc == c);
}

void test_greater() {
    BigInt a(-12345);
    bool b = false;
    try {
        a > BigInt();
    }
    catch (std::domain_error &e) {
        b = true;
    }
    assert(b);
    
    b = false;
    try {
        BigInt() >= a;
    }
    catch (std::domain_error &e) {
        b = true;
    }
    assert(b);
    
    b = false;
    try {
        a >= BigInt();
    }
    catch (std::domain_error &e) {
        b = true;
    }
    assert(b);
    
    b = false;
    try {
        BigInt() >= a;
    }
    catch (std::domain_error &e) {
        b = true;
    }
    assert(b);
    
    int c = 123;
    int d = 927;
    BigInt bigc = c;
    BigInt bigd = d;
    
    assert(bigc > bigd == c >  d);
    assert(-bigc > bigd == -c > d);
    assert(bigc > -bigd == c > -d);
    assert(-bigc > -bigd == -c > -d);
    assert(bigc > bigc == c > c);
    assert(bigd > bigd == d > d);
    
    assert(bigc >= bigd == c >=  d);
    assert(-bigc >= bigd == -c >= d);
    assert(bigc >= -bigd == c >= -d);
    assert(-bigc >= -bigd == -c >= -d);
    assert(bigc >= bigc == c >= c);
    assert(bigd >= bigd == d >= d);
    
    assert(bigd == d);
    assert(bigc == c);
}

int main(int argc, char** argv) {
    test_constructor();
    test_eq_ne();
    test_greater();
    test_less();
    test_minus();
    test_move_copy();
    test_nan();
    test_plus();
    test_unary_minus();
    return 0;
}

