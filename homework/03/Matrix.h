#pragma once
#include <exception>

class Matrix {
public:
    class Row {
    public:
        Row(int*, size_t);
        ~Row();
        const int& operator[](size_t) const;
        int& operator[](size_t);
    private:
        size_t length;
        int* mem;
    };
    
    Matrix(size_t, size_t);
    ~Matrix();
    const Row operator[](size_t) const;
    Row operator[](size_t);
    size_t getRows() const;
    size_t getColumns() const;
    bool operator==(const Matrix&) const;
    bool operator!=(const Matrix&) const;
    const Matrix& operator*=(int);
private:
    int *memory;
    size_t rownum, colnum;
};
