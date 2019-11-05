#include "BigInt.h"
#include <cstdio>

BigInt::BigInt() : sign(true), length(0), mem(nullptr) {};

BigInt::BigInt(const BigInt& other) 
: sign(other.sign), length(other.length) {
    if (other.is_nan()) {
        mem = nullptr;
    }
    else {
        mem = new char[length];
        for (int i = 0; i < length; ++i)
            mem[i] = other.mem[i];
    }
}
    
BigInt::BigInt(int i) {
    if (i == 0) {
        length = 1;
        sign = true;
        mem = new char[1];
        mem[0] = 0;
    }
    else {
        sign = true;
        if (i <= 0) {
            i = -i;
            sign = false;
        }
        char buf[10];
        int size;
        for (size = 0; i; ++size) {
            buf[size] = i % 10;
            i /= 10;
        }
        length = size;
        mem = new char[length];
        for (int j = 0; j < length; ++j)
            mem[j] = buf[j];
    }
}
    
BigInt::BigInt(BigInt&& moved) 
: sign(moved.sign), length(moved.length), mem(moved.mem) {
    moved.length = 0;
    moved.sign = true;
    moved.mem = nullptr;
}
    
const BigInt& BigInt::operator=(const BigInt& other) {
    if (this == &other)
        return *this;
    sign = other.sign;
    length = other.length;
    mem = new char[length];
    for (int i = 0; i < length; ++i)
        mem[i] = other.mem[i];
    return *this;
}

const BigInt& BigInt::operator=(int i) {
    *this = BigInt(i);
    return *this;
}

const BigInt& BigInt::operator=(BigInt&& moved) {
    if (this == &moved)
        return *this;
    sign = moved.sign;
    length = moved.length;
    delete[] mem;
    mem = moved.mem;
    moved.length = 0;
    moved.sign = true;
    moved.mem = nullptr;
    return *this;
}
    
BigInt BigInt::operator-() const {
    if (this->is_nan())
        throw std::domain_error("object is nan");
            
    BigInt result(*this);
    if (result.length == 1 && result.mem[0] == 0)
        return result;
    result.sign = !sign;
    return result;
}

BigInt operator-(BigInt&& rvalue) {
    if (rvalue.is_nan())
        throw std::domain_error("object is nan");
    
    //std::cout << "rvalue " << rvalue << '\n';
    rvalue.sign = !rvalue.sign;
    return rvalue;
}

BigInt natural_plus(const BigInt& left, const BigInt& right) {
    if (left.is_nan()) {
        throw std::domain_error("left object is nan");
    }
    if (right.is_nan()) {
        throw std::domain_error("right object is nan");
    }
    
    if (left.length >= right.length) {
        BigInt result;
        char add = 0;
        for (int i = 0; i < left.length; ++i) {
            int r = i < right.length ? right.mem[i] : 0;
            add = (left.mem[i] + r + add) > 9;
        }
        result.length = left.length + add;
        result.sign = true;
        result.mem = new char[result.length];
        add = 0;
        for (int i = 0; i < result.length; ++i) {
            int r = i < right.length ? right.mem[i] : 0;
            int l = i < left.length? left.mem[i] : 0;
            result.mem[i] = (l + r + add) % 10;
            add = (l + r + add) > 9;
        }
        return result;
    }
    else {
        return natural_plus(right, left);
    }
}

BigInt natural_minus(const BigInt& left, const BigInt& right) {
    if (left.is_nan()) {
        throw std::domain_error("left object is nan");
    }
    if (right.is_nan()) {
        throw std::domain_error("right object is nan");
    }
    
    BigInt result;
    result.length = 0;
    result.sign = true;
    char add = false;
    for (int i = 0; i < left.length; ++i) {
        int r = i < right.length ? right.mem[i] : 0;
        int digit = left.mem[i] - add - r; 
        add = (digit) < 0;
        if (digit != 0)
            result.length = i + 1; 
    }
    if (result.length == 0)
        return BigInt(0);
    result.mem = new char[result.length];
    add = false;
    for (int i = 0; i < result.length; ++i) {
        int r = i < right.length ? right.mem[i] : 0;
        int digit = left.mem[i] - add - r;
        add = digit < 0;
        result.mem[i] = add ? 10 + digit : digit; 
    }
    return result;
}
    
BigInt operator+(const BigInt& left, const BigInt& right) {
    if (left.is_nan()) {
        throw std::domain_error("left object is nan");
    }
    if (right.is_nan()) {
        throw std::domain_error("right object is nan");
    }
    
    if (left.sign == right.sign) {
        return left.sign ? natural_plus(left, right) :
            -natural_plus(left, right);
    }
    else {
        if (left.sign)
            return !abs_less(left, right) ? natural_minus(left, right) :
                -natural_minus(right, left);
        else
            return !abs_less(left, right) ? -natural_minus(left, right) :
                natural_minus(right, left);
    }
}

BigInt operator-(const BigInt& left, const BigInt& right) {
    if (left.is_nan()) {
        throw std::domain_error("left object is nan");
    }
    if (right.is_nan()) {
        throw std::domain_error("right object is nan");
    }
    
    return left + (-right);
}

bool abs_less(const BigInt& left, const BigInt& right) {
    if (left.is_nan()) {
        throw std::domain_error("left object is nan");
    }
    if (right.is_nan()) {
        throw std::domain_error("right object is nan");
    }
    
    if (left.length < right.length)
        return true;
    else if (left.length > right.length)
        return false;
    else {
        for (int i = left.length - 1; i >= 0; --i) {
            if (left.mem[i] < right.mem[i])
                return true;
            else if (left.mem[i] > right.mem[i])
                return false;
        }
        return false;
    }
}

bool operator<(const BigInt& left, const BigInt& right) {
    if (left.is_nan()) {
        throw std::domain_error("left object is nan");
    }
    if (right.is_nan()) {
        throw std::domain_error("right object is nan");
    }
    
    if (left.sign != right.sign) {
        if (!left.sign)
            return true;
        return false;
    }
    if (left.sign) {
        return abs_less(left, right);
    }
    else {
        return !abs_less(left, right);
    }
}

bool operator>(const BigInt& left, const BigInt& right) {
    if (left.is_nan()) {
        throw std::domain_error("left object is nan");
    }
    if (right.is_nan()) {
        throw std::domain_error("right object is nan");
    }
    
    return right < left;
}

bool operator==(const BigInt& left, const BigInt& right) {
    if (left.is_nan()) {
        throw std::domain_error("left object is nan");
    }
    if (right.is_nan()) {
        throw std::domain_error("right object is nan");
    }
    
    if (left.sign != right.sign)
        return false;
    if (left.length != right.length)
        return false;
    
    for (int i = left.length - 1; i >= 0; --i) {
        if (left.mem[i] != right.mem[i])
            return false;
    }
    return true;
}

bool operator!=(const BigInt& left, const BigInt& right) {
    return !(left == right);
}

bool operator<=(const BigInt& left, const BigInt& right) {
    return left < right || left == right;
}

bool operator>=(const BigInt& left, const BigInt& right) {
    return left > right || left == right;
}


BigInt operator+(int left, const BigInt& right) {
    return BigInt(left) + right;
}

BigInt operator-(int left, const BigInt& right) {
    return BigInt(left) - right;
}

bool operator<(int left, const BigInt& right) {
    return BigInt(left) < right;
}

bool operator>(int left, const BigInt& right) {
    return BigInt(left) > right;
}

bool operator==(int left, const BigInt& right) {
    return BigInt(left) == right;
}

bool operator!=(int left, const BigInt& right) {
    return !(left == right);
}

bool operator<=(int left, const BigInt& right) {
    return BigInt(left) <= right;
}

bool operator>=(int left, const BigInt& right) {
    return BigInt(left) >= right;
}

BigInt operator+(const BigInt& left, int right) {
    return left + BigInt(right);
}

BigInt operator-(const BigInt& left, int right) {
    return left - BigInt(right);
}

bool operator<(const BigInt& left, int right) {
    return left < BigInt(right);
}

bool operator>(const BigInt& left, int right) {
    return left > BigInt(right);
}

bool operator==(const BigInt& left, int right) {
    return left == BigInt(right);
}

bool operator!=(const BigInt& left, int right) {
    return !(left == right);
}

bool operator<=(const BigInt& left, int right) {
    return left <= BigInt(right);
}

bool operator>=(const BigInt& left, int right) {
    return left >= BigInt(right);
}

bool BigInt::is_nan() const {
    return sign && length == 0 && mem == nullptr;
}

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    if (num.is_nan()) {
        os << "nan";
        return os;
    }
    
    if (!num.sign) 
        os << '-';
    for (int i = num.length - 1; i >= 0; --i)
        os << static_cast<char>(num.mem[i] + '0');
    return os;
}

BigInt::~BigInt() {
    delete[] mem;
}

