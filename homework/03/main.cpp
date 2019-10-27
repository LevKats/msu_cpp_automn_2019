#include <iostream>
#include <cassert>
#include <string>
#include "Matrix.h"

void test_constructor() {
    const size_t rows = 5;
    const size_t cols = 3;

    const Matrix m(rows, cols);

    assert(m.getRows() == 5);   
    assert(m.getColumns() == 3);
}

void test_index() {
    const size_t rows = 5;
    const size_t cols = 3;
    Matrix* m = new Matrix(rows, cols);
    (*m)[1][2] = 3;
    assert((*m)[1][2] == 3);
    delete m;
}

void test_const_index() {
    const size_t rows = 5;
    const size_t cols = 3;
    Matrix* m = new Matrix(rows, cols);
    (*m)[1][2] = 3;
    const Matrix *mc = const_cast<const Matrix*>(m);
    assert((*mc)[1][2] == 3);
    delete m;
}

void test_index_out_of_range_rows() {
    const size_t rows = 5;
    const size_t cols = 3;
    const Matrix m(rows, cols);
    volatile const Matrix::Row r = m[4]; 
    bool b = false;
    // ok, volatile to be not optimized 
    // with -O2
    try {
        volatile const Matrix::Row r2 = m[5];
    }
    catch (std::out_of_range& e) {
        assert(std::string{e.what()} == "");
        b = true;
    }
    assert(b);
}

void test_index_out_of_range_cols() {
    const size_t rows = 5;
    const size_t cols = 3;
    const Matrix m(rows, cols);
    const Matrix::Row r = m[1];
    volatile int i = r[2];
    bool b = false;
    // ok
    try {
        volatile int j = r[3];
    }
    catch (std::out_of_range& e) {
        assert(std::string{e.what()} == "");
        b = true;
    }
    assert(b);
}

void test_equal_not_equal() {
    const size_t rows = 5;
    const size_t cols = 3;
    Matrix m(rows, cols);
    
    const Matrix m2(rows - 1, cols - 1);
    assert(m != m2);
    assert(!(m == m2));
    
    assert(m == m);
    assert(!(m != m));
    
    const size_t rows2 = 2;
    const size_t cols2 = 2;
    Matrix m3(rows2, cols2), m4(rows2, cols2);
    m3[0][0] = m3[0][1] = m3[1][0] = m3[1][1] = 1;
    m4[0][0] = m4[0][1] = m4[1][0] = m4[1][1] = 1;
    assert(m3 == m4);
    assert(!(m3 != m4));
    
    m4[0][0] = 3;
    assert(m3 != m4);
    assert(!(m3 == m4));
}

void test_mult() {
    const size_t rows2 = 2;
    const size_t cols2 = 2;
    Matrix m3(rows2, cols2), m4(rows2, cols2);
    m3[0][0] = m4[0][0] = 1;
    m3[0][1] = m4[0][1] = 2;
    m3[1][0] = m4[1][0] = 3;
    m3[1][1] = m4[1][1] = 4;
    m3 *= 3;
    for (size_t i = 0; i != m3.getRows(); ++i)
        for (size_t j = 0; j != m3.getColumns(); ++j)
            assert(m3[i][j] == 3 * m4[i][j]);
}

int main() {
    test_constructor();
    test_index();
    test_const_index();
    test_index_out_of_range_rows();
    test_index_out_of_range_cols();
    test_equal_not_equal();
    test_mult;
    return 0;
}

