#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <stdexcept>

class Matrix {
private:
    std::vector<std::vector<double>> data;
    size_t rows;
    size_t cols;

public:
    // Constructors
    Matrix(size_t rows, size_t cols);
    Matrix(size_t rows, size_t cols, double value);
    Matrix(const std::vector<std::vector<double>>& data);
    
    // Getters
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }
    double getElement(size_t row, size_t col) const;
    void setElement(size_t row, size_t col, double value);
    
    // Input/Output
    void print() const;
    void readFromInput();
    
    // Basic operations
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(double scalar) const;
    
    // Advanced operations
    Matrix transpose() const;
    double determinant() const;
    Matrix inverse() const;
    Matrix cofactor() const;
    
    // Utility
    bool isSquare() const { return rows == cols; }
    bool isCompatibleForAddition(const Matrix& other) const;
    bool isCompatibleForMultiplication(const Matrix& other) const;
    
    // Static factory methods
    static Matrix identity(size_t size);
};

#endif // MATRIX_H
