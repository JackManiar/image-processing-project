// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cstdint>
#include "Vector.h"

class Matrix {

protected:
    Vector<Vector<uint8_t>> data;

private:
    int numRows;
    int numCols;

public:
    // Default constructor
    Matrix();

    // Constructor with rows and columns
    Matrix(int rows, int cols);

    // Copy constructor
    Matrix(const Matrix& other);

    // Assignment operator
    Matrix& operator=(const Matrix& other);

    // Destructor
    ~Matrix();

    // Input stream operator
    friend std::istream& operator>>(std::istream& in, Matrix& mat);

    // Output stream operator
    friend std::ostream& operator<<(std::ostream& out, const Matrix& mat);

    // Arithmetic operators
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;

    // Subscript operator
    Vector<uint8_t>& operator[](int index);
    const Vector<uint8_t>& operator[](int index) const;

    // Number of rows
    int getRows() const;

    // Number of columns
    int getCols() const;

    // Transpose function (in-place)
    void transpose();
};

#endif // MATRIX_H

