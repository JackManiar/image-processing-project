// Matrix.cpp

#include "Matrix.h"
#include <stdexcept>

// Default constructor
Matrix::Matrix(): numRows(0), numCols(0), data() {}

Matrix::Matrix(int rows, int cols): numRows(rows), numCols(cols), data(rows){ //data has param rows so that data has row elements
    for(size_t i = 0; i < rows; i++)
    data[i] = Vector<uint8_t>(cols);
}

// Copy constructor
Matrix::Matrix(const Matrix& other): numRows(other.numRows), numCols(other.numCols), data(other.data){}

// Assignment operator
Matrix& Matrix::operator=(const Matrix& other) {
    if(this!= &other){
        numRows = other.numRows;
        numCols = other.numCols;
        data = other.data;
    }
    return *this;
}

// Destructor
Matrix::~Matrix() {} //maybe write something here that prints out when the columns are deleted to make sure that the vector destructor takes care of everything

// Number of rows
int Matrix::getRows() const {
    return data.getsize();
}

// Number of columns
int Matrix::getCols() const {
    return data.getsize() > 0 ? data[0].getsize() : 0;
}

// Input stream operator
std::istream& operator>>(std::istream& in, Matrix& mat) {
    for(size_t i = 0; i <mat.numRows; i++)
        in >> mat.data[i];
    return in;
}

// Output stream operator
std::ostream& operator<<(std::ostream& out, const Matrix& mat) {
    for(size_t i = 0; i < mat.numRows; i++){
        out << mat.data[i] << std::endl; //makes use of our definition of the << operator in the vector class
    }
        return out;
}

// Arithmetic operators
/*
Matrix Matrix::operator+(const Matrix& other) const {
    if(numRows != other.numRows || numCols != other.numcols)
        throw std::invalid_argument("Matrix dimensions must match (+)");
    
    Matrix result(numRows, numCols);
    for(size_t i = 0; i < numRows; i++){
        result.data[i] = data[i] + other.data[i];
    }
    return result;
}
*/

//Arithmetic operator with casting like the 1st project
Matrix Matrix::operator+(const Matrix& other) const{
    if(numRows != other.numRows || numCols != other.numCols)
        throw std::invalid_argument("Matrix dimensions must match (+)");
    
    Matrix result(numRows, numCols);
    for(size_t i = 0; i < numRows; i++){
        for(size_t j = 0; j < numCols; j++){
            int sum = static_cast<int>(data[i][j]) + static_cast<int>(other.data[i][j]);
            result.data[i][j] = (sum > 255) ? static_cast<uint8_t>(255) : static_cast<uint8_t>(sum);
        }
    }
    return result;
}
/*
Matrix Matrix::operator-(const Matrix& other) const {
    if(numRows != other.numRows || numCols != other.numcols)
        throw std::invalid_argument("Matrix dimensions must match (-)");
    
    Matrix result(numRows, numCols);
    for(size_t i = 0; i < numRows; i++){
        result.data[i] = data[i] - other.data[i]; 
    }
    return result;
}
*/
//
Matrix Matrix::operator-(const Matrix& other) const {
    if(numRows != other.numRows || numCols != other.numCols)
        throw std::invalid_argument("Matrix dimensions must match (-)");
    
    Matrix result(numRows, numCols);
    for(size_t i = 0; i < numRows; i++){
        for(size_t j = 0; j < numCols; j++){
            int diff = static_cast<int>(data[i][j]) - static_cast<int>(other.data[i][j]);
            result.data[i][j] = (diff < 0) ? static_cast<uint8_t>(0) : static_cast<uint8_t>(diff);
        }
    }
    return result;
}

//should implement standard matrix multiplication so will be more involved
Matrix Matrix::operator*(const Matrix& other) const {
    if(numCols != other.numRows)
        throw std::invalid_argument("Noncompatible dimensions for matrix multiplication");
    
    Matrix result(numRows, other.numCols);
    for(size_t i = 0; i < numRows; i++){
        for(size_t j = 0; j < other.numCols; j++){
            int sum = 0;
            for(size_t k = 0; k < numCols; k++){
                sum += static_cast<int>(data[i][k]) * static_cast<int>(other.data[k][j]);
            }
            result.data[i][j] = (sum > 255) ? static_cast<uint8_t>(255) : static_cast<uint8_t>(sum);
        }
    }
    return result;
}

// Subscript operator
Vector<uint8_t>& Matrix::operator[](int index) {
    if(index < 0 || index >= numRows)
        throw std::out_of_range("Row index is out of range");
    return data[index];
}

const Vector<uint8_t>& Matrix::operator[](int index) const {
    if(index < 0 || index >= numRows)
        throw std::out_of_range("Row index is out of range");
    return data[index];
}

// Transpose function (in-place)
void Matrix::transpose() {
    Matrix temp(numCols, numRows);
    for(size_t i = 0; i < numCols; i++){
        for(size_t j = 0; j < numRows; j++){
            temp[i][j] = data[j][i];
        }
    }
    data = temp.data;
    int tempR = numRows;
    numRows = numCols;
    numCols = tempR;
}

