#include "matrix.h"
#include <iostream>
#include <limits>
#include <iomanip>

void printMenu() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "           MATRIX CALCULATOR" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "1.  Matrix Addition (A + B)" << std::endl;
    std::cout << "2.  Matrix Subtraction (A - B)" << std::endl;
    std::cout << "3.  Matrix Multiplication (A * B)" << std::endl;
    std::cout << "4.  Scalar Multiplication (k * A)" << std::endl;
    std::cout << "5.  Matrix Transpose (A^T)" << std::endl;
    std::cout << "6.  Matrix Determinant (det(A))" << std::endl;
    std::cout << "7.  Matrix Inverse (A^-1)" << std::endl;
    std::cout << "8.  Exit" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Enter your choice: ";
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

size_t getSize(const std::string& dimension) {
    size_t size;
    std::cout << "Enter number of " << dimension << ": ";
    while (!(std::cin >> size) || size == 0) {
        std::cout << "Invalid input. Please enter a positive integer: ";
        clearInputBuffer();
    }
    return size;
}

void handleAddition() {
    std::cout << "\n--- Matrix Addition ---" << std::endl;
    size_t rows = getSize("rows");
    size_t cols = getSize("columns");
    
    std::cout << "\nMatrix A:" << std::endl;
    Matrix A(rows, cols);
    A.readFromInput();
    
    std::cout << "\nMatrix B:" << std::endl;
    Matrix B(rows, cols);
    B.readFromInput();
    
    try {
        Matrix result = A + B;
        std::cout << "\nResult (A + B):" << std::endl;
        result.print();
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void handleSubtraction() {
    std::cout << "\n--- Matrix Subtraction ---" << std::endl;
    size_t rows = getSize("rows");
    size_t cols = getSize("columns");
    
    std::cout << "\nMatrix A:" << std::endl;
    Matrix A(rows, cols);
    A.readFromInput();
    
    std::cout << "\nMatrix B:" << std::endl;
    Matrix B(rows, cols);
    B.readFromInput();
    
    try {
        Matrix result = A - B;
        std::cout << "\nResult (A - B):" << std::endl;
        result.print();
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void handleMultiplication() {
    std::cout << "\n--- Matrix Multiplication ---" << std::endl;
    std::cout << "Matrix A dimensions:" << std::endl;
    size_t rowsA = getSize("rows");
    size_t colsA = getSize("columns");
    
    std::cout << "\nMatrix B dimensions:" << std::endl;
    size_t rowsB = getSize("rows");
    size_t colsB = getSize("columns");
    
    if (colsA != rowsB) {
        std::cout << "Error: Number of columns of A must equal number of rows of B" << std::endl;
        return;
    }
    
    std::cout << "\nMatrix A:" << std::endl;
    Matrix A(rowsA, colsA);
    A.readFromInput();
    
    std::cout << "\nMatrix B:" << std::endl;
    Matrix B(rowsB, colsB);
    B.readFromInput();
    
    try {
        Matrix result = A * B;
        std::cout << "\nResult (A * B):" << std::endl;
        result.print();
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void handleScalarMultiplication() {
    std::cout << "\n--- Scalar Multiplication ---" << std::endl;
    size_t rows = getSize("rows");
    size_t cols = getSize("columns");
    
    std::cout << "\nMatrix A:" << std::endl;
    Matrix A(rows, cols);
    A.readFromInput();
    
    double scalar;
    std::cout << "\nEnter scalar value: ";
    while (!(std::cin >> scalar)) {
        std::cout << "Invalid input. Please enter a number: ";
        clearInputBuffer();
    }
    
    Matrix result = A * scalar;
    std::cout << "\nResult (" << scalar << " * A):" << std::endl;
    result.print();
}

void handleTranspose() {
    std::cout << "\n--- Matrix Transpose ---" << std::endl;
    size_t rows = getSize("rows");
    size_t cols = getSize("columns");
    
    std::cout << "\nMatrix A:" << std::endl;
    Matrix A(rows, cols);
    A.readFromInput();
    
    Matrix result = A.transpose();
    std::cout << "\nResult (A^T):" << std::endl;
    result.print();
}

void handleDeterminant() {
    std::cout << "\n--- Matrix Determinant ---" << std::endl;
    size_t size = getSize("rows/columns (square matrix)");
    
    std::cout << "\nMatrix A:" << std::endl;
    Matrix A(size, size);
    A.readFromInput();
    
    try {
        double det = A.determinant();
        std::cout << "\nDeterminant of A: " << std::fixed << std::setprecision(6) << det << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void handleInverse() {
    std::cout << "\n--- Matrix Inverse ---" << std::endl;
    size_t size = getSize("rows/columns (square matrix)");
    
    std::cout << "\nMatrix A:" << std::endl;
    Matrix A(size, size);
    A.readFromInput();
    
    try {
        Matrix result = A.inverse();
        std::cout << "\nResult (A^-1):" << std::endl;
        result.print();
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

int main() {
    int choice;
    
    std::cout << "Welcome to the Matrix Calculator!" << std::endl;
    
    while (true) {
        printMenu();
        
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number between 1-8." << std::endl;
            clearInputBuffer();
            continue;
        }
        
        clearInputBuffer();
        
        switch (choice) {
            case 1:
                handleAddition();
                break;
            case 2:
                handleSubtraction();
                break;
            case 3:
                handleMultiplication();
                break;
            case 4:
                handleScalarMultiplication();
                break;
            case 5:
                handleTranspose();
                break;
            case 6:
                handleDeterminant();
                break;
            case 7:
                handleInverse();
                break;
            case 8:
                std::cout << "\nThank you for using Matrix Calculator. Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice. Please enter a number between 1-8." << std::endl;
                break;
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
    
    return 0;
}
