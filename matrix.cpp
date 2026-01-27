#include "matrix.h"
#include <cmath>
#include <algorithm>

// Constructors
Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
    data.resize(rows, std::vector<double>(cols, 0.0));
}

Matrix::Matrix(size_t rows, size_t cols, double value) : rows(rows), cols(cols) {
    data.resize(rows, std::vector<double>(cols, value));
}

Matrix::Matrix(const std::vector<std::vector<double>>& data) {
    if (data.empty() || data[0].empty()) {
        throw std::invalid_argument("Matrix cannot be empty");
    }
    rows = data.size();
    cols = data[0].size();
    this->data = data;
    
    // Validate all rows have same size
    for (size_t i = 0; i < rows; i++) {
        if (data[i].size() != cols) {
            throw std::invalid_argument("All rows must have the same number of columns");
        }
    }
}

// Getters and setters
double Matrix::getElement(size_t row, size_t col) const {
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Matrix index out of range");
    }
    return data[row][col];
}

void Matrix::setElement(size_t row, size_t col, double value) {
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Matrix index out of range");
    }
    data[row][col] = value;
}

// Input/Output
void Matrix::print() const {
    for (size_t i = 0; i < rows; i++) {
        std::cout << "| ";
        for (size_t j = 0; j < cols; j++) {
            std::cout << std::setw(10) << std::fixed << std::setprecision(3) << data[i][j];
            if (j < cols - 1) std::cout << " ";
        }
        std::cout << " |" << std::endl;
    }
}

void Matrix::readFromInput() {
    std::cout << "Enter matrix elements row by row (space-separated):" << std::endl;
    for (size_t i = 0; i < rows; i++) {
        std::cout << "Row " << (i + 1) << ": ";
        for (size_t j = 0; j < cols; j++) {
            std::cin >> data[i][j];
        }
    }
}

// Basic operations
Matrix Matrix::operator+(const Matrix& other) const {
    if (!isCompatibleForAddition(other)) {
        throw std::invalid_argument("Matrices must have the same dimensions for addition");
    }
    
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (!isCompatibleForAddition(other)) {
        throw std::invalid_argument("Matrices must have the same dimensions for subtraction");
    }
    
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (!isCompatibleForMultiplication(other)) {
        throw std::invalid_argument("Number of columns of first matrix must equal number of rows of second matrix");
    }
    
    Matrix result(rows, other.cols);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < other.cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < cols; k++) {
                sum += data[i][k] * other.data[k][j];
            }
            result.data[i][j] = sum;
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            result.data[i][j] = data[i][j] * scalar;
        }
    }
    return result;
}

// Advanced operations
Matrix Matrix::transpose() const {
    Matrix result(cols, rows);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            result.data[j][i] = data[i][j];
        }
    }
    return result;
}

double Matrix::determinant() const {
    if (!isSquare()) {
        throw std::invalid_argument("Determinant can only be calculated for square matrices");
    }
    
    if (rows == 1) {
        return data[0][0];
    }
    
    if (rows == 2) {
        return data[0][0] * data[1][1] - data[0][1] * data[1][0];
    }
    
    // For larger matrices, use cofactor expansion
    double det = 0.0;
    for (size_t j = 0; j < cols; j++) {
        Matrix submatrix(rows - 1, cols - 1);
        for (size_t i = 1; i < rows; i++) {
            size_t colIndex = 0;
            for (size_t k = 0; k < cols; k++) {
                if (k != j) {
                    submatrix.setElement(i - 1, colIndex, data[i][k]);
                    colIndex++;
                }
            }
        }
        double cofactor = (j % 2 == 0 ? 1 : -1) * data[0][j] * submatrix.determinant();
        det += cofactor;
    }
    return det;
}

Matrix Matrix::cofactor() const {
    if (!isSquare()) {
        throw std::invalid_argument("Cofactor matrix can only be calculated for square matrices");
    }
    
    Matrix result(rows, cols);
    
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            // Create minor matrix
            Matrix minor(rows - 1, cols - 1);
            size_t minorRow = 0;
            for (size_t r = 0; r < rows; r++) {
                if (r == i) continue;
                size_t minorCol = 0;
                for (size_t c = 0; c < cols; c++) {
                    if (c == j) continue;
                    minor.setElement(minorRow, minorCol, data[r][c]);
                    minorCol++;
                }
                minorRow++;
            }
            
            double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
            result.setElement(i, j, sign * minor.determinant());
        }
    }
    
    return result;
}

Matrix Matrix::inverse() const {
    if (!isSquare()) {
        throw std::invalid_argument("Inverse can only be calculated for square matrices");
    }
    
    double det = determinant();
    if (std::abs(det) < 1e-10) {
        throw std::runtime_error("Matrix is singular (determinant is zero), cannot compute inverse");
    }
    
    Matrix cofactorMatrix = cofactor();
    Matrix adjugate = cofactorMatrix.transpose();
    return adjugate * (1.0 / det);
}

// Utility functions
bool Matrix::isCompatibleForAddition(const Matrix& other) const {
    return rows == other.rows && cols == other.cols;
}

bool Matrix::isCompatibleForMultiplication(const Matrix& other) const {
    return cols == other.rows;
}

// Static factory methods
Matrix Matrix::identity(size_t size) {
    Matrix result(size, size);
    for (size_t i = 0; i < size; i++) {
        result.setElement(i, i, 1.0);
    }
    return result;
}
