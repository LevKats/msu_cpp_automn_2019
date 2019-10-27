#include <stdexcept>

#include "Matrix.h"

Matrix::Row::Row(int* row, size_t size) : mem(row), length(size) {}
        
Matrix::Row::~Row() {}

const int& Matrix::Row::operator[](size_t ind) const {
    if (ind >= length)
        throw std::out_of_range("");
    return mem[ind];
}
        
int& Matrix::Row::operator[](size_t ind) {
    if (ind >= length)
        throw std::out_of_range("");
    return mem[ind];
}
    
Matrix::Matrix(size_t rownum, size_t colnum) 
: colnum(colnum), rownum(rownum) {
    memory = new int[colnum * rownum];
}

Matrix::~Matrix() {
    delete[] memory;
}

const Matrix::Row Matrix::operator[](size_t ind) const {
    if (ind >= rownum)
        throw std::out_of_range("");
    return Matrix::Row(memory + colnum * ind, colnum);
}

Matrix::Row Matrix::operator[](size_t ind) {
    if (ind >= rownum)
        throw std::out_of_range("");
    return Matrix::Row(memory + colnum * ind, colnum);
}

size_t Matrix::getRows() const {
    return rownum;
}

size_t Matrix::getColumns() const {
    return colnum;
}

bool Matrix::operator==(const Matrix& other) const {
    if (this == &other)
        return true;
    if (other.getRows() != this->getRows() ||
            other.getColumns() != this->getColumns())
        return false;
    bool result = true;
    for (size_t i = 0; i != this->getRows(); ++i)
        for (size_t j = 0; j != this->getColumns(); ++j)
            result &= (*this)[i][j] == other[i][j];
    return result;
}

bool Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}

const Matrix& Matrix::operator*=(int n) {
    for (size_t i = 0; i != this->getRows(); ++i)
        for (size_t j = 0; j != this->getColumns(); ++j)
            (*this)[i][j] *= n;
    return *this;
}
