#pragma once

#include <iostream>

class BigInt {
public:
    BigInt();
    BigInt(const BigInt&);
    BigInt(int);
    BigInt(BigInt&&);
    
    BigInt& operator=(const BigInt&);
    BigInt& operator=(int);
    BigInt& operator=(BigInt&&);
    
    BigInt operator-() const;
    friend BigInt operator-(BigInt&&);
    
    friend BigInt operator+(const BigInt&, const BigInt&);
    friend BigInt operator-(const BigInt&, const BigInt&);
    friend bool operator<(const BigInt&, const BigInt&);
    friend bool operator>(const BigInt&, const BigInt&);
    friend bool operator==(const BigInt&, const BigInt&);
    friend bool operator!=(const BigInt&, const BigInt&);
    friend bool operator<=(const BigInt&, const BigInt&);
    friend bool operator>=(const BigInt&, const BigInt&);
    
    friend BigInt operator+(int, const BigInt&);
    friend BigInt operator-(int, const BigInt&);
    friend bool operator<(int, const BigInt&);
    friend bool operator>(int, const BigInt&);
    friend bool operator==(int, const BigInt&);
    friend bool operator!=(int, const BigInt&);
    friend bool operator<=(int, const BigInt&);
    friend bool operator>=(int, const BigInt&);
    
    friend BigInt operator+(const BigInt&, int);
    friend BigInt operator-(const BigInt&, int);
    friend bool operator<(const BigInt&, int);
    friend bool operator>(const BigInt&, int);
    friend bool operator==(const BigInt&, int);
    friend bool operator!=(const BigInt&, int);
    friend bool operator<=(const BigInt&, int);
    friend bool operator>=(const BigInt&, int);
    
    bool is_nan() const;
    
    friend std::ostream& operator<<(std::ostream&, const BigInt&);
    
    ~BigInt();
private:
    BigInt natural_plus(const BigInt&) const;
    BigInt natural_minus(const BigInt&) const;
    bool abs_less(const BigInt&) const;


    bool sign;
    char* mem;
    size_t length;
};

