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
    std::cout << "8.  Solve 3 Linear Equations (x, y, z)" << std::endl;
    std::cout << "9.  Exit" << std::endl;
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

void handleLinearSystem3x3() {
    std::cout << "\n--- Solve 3 Linear Equations (x, y, z) ---" << std::endl;
    std::cout << "Enter each equation as: a*x + b*y + c*z = d" << std::endl;
    std::cout << "Input the coefficients a, b, c, and constant d (space-separated)." << std::endl;

    double coeff[3][4];  // 3 equations, each with a, b, c, d
    for (int eq = 0; eq < 3; eq++) {
        std::cout << "\nEquation " << (eq + 1) << " - enter a b c d: ";
        while (!(std::cin >> coeff[eq][0] >> coeff[eq][1] >> coeff[eq][2] >> coeff[eq][3])) {
            std::cout << "Invalid input. Enter four numbers (a b c d): ";
            clearInputBuffer();
        }
    }

    std::cout << std::fixed << std::setprecision(4);

    // Display the system
    std::cout << "\n" << std::string(54, '=') << std::endl;
    std::cout << "STEP 0: Your system of equations" << std::endl;
    std::cout << std::string(54, '=') << std::endl;
    for (int eq = 0; eq < 3; eq++) {
        std::cout << "  " << coeff[eq][0] << "*x + " << coeff[eq][1] << "*y + "
                  << coeff[eq][2] << "*z = " << coeff[eq][3] << std::endl;
    }

    // Build augmented matrix [A|B] (3 rows x 4 columns)
    Matrix Aug(3, 4);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            Aug.setElement(i, j, coeff[i][j]);
        }
    }

    std::cout << "\n" << std::string(54, '=') << std::endl;
    std::cout << "STEP 1: Augmented matrix [ A | B ]" << std::endl;
    std::cout << std::string(54, '=') << std::endl;
    std::cout << "  (Each row is one equation; last column is the right-hand side.)\n" << std::endl;
    Aug.printAugmented();

    try {
        int stepNum = 2;

        // --- Forward elimination (Gaussian elimination) ---
        std::cout << "\n" << std::string(54, '=') << std::endl;
        std::cout << "STEP " << stepNum++ << ": Forward elimination (Gaussian elimination)" << std::endl;
        std::cout << std::string(54, '=') << std::endl;

        const double eps = 1e-10;

        // Pivot column 0: get non-zero in (0,0), then eliminate below
        if (std::abs(Aug.getElement(0, 0)) < eps) {
            if (std::abs(Aug.getElement(1, 0)) > eps) {
                std::cout << "  Swap R1 <-> R2 so pivot (1,1) is non-zero.\n" << std::endl;
                Aug.swapRows(0, 1);
                Aug.printAugmented();
            } else if (std::abs(Aug.getElement(2, 0)) > eps) {
                std::cout << "  Swap R1 <-> R3 so pivot (1,1) is non-zero.\n" << std::endl;
                Aug.swapRows(0, 2);
                Aug.printAugmented();
            } else {
                std::cout << "  Column 1 is all zeros: system has no unique solution." << std::endl;
                return;
            }
        }
        if (std::abs(Aug.getElement(0, 0)) > eps) {
            double m1 = Aug.getElement(1, 0) / Aug.getElement(0, 0);
            std::cout << "  R2 <- R2 + (" << -m1 << ")*R1  (eliminate below pivot in column 1)\n" << std::endl;
            Aug.addRowMultiple(1, 0, -m1);
            Aug.printAugmented();

            double m2 = Aug.getElement(2, 0) / Aug.getElement(0, 0);
            std::cout << "  R3 <- R3 + (" << -m2 << ")*R1  (eliminate below pivot in column 1)\n" << std::endl;
            Aug.addRowMultiple(2, 0, -m2);
            Aug.printAugmented();
        }

        // Pivot column 1: get non-zero in (1,1), then eliminate below
        if (std::abs(Aug.getElement(1, 1)) < eps) {
            if (std::abs(Aug.getElement(2, 1)) > eps) {
                std::cout << "  Swap R2 <-> R3 so pivot (2,2) is non-zero.\n" << std::endl;
                Aug.swapRows(1, 2);
                Aug.printAugmented();
            } else {
                std::cout << "  Pivot (2,2) is zero: system has no unique solution." << std::endl;
                return;
            }
        }
        if (std::abs(Aug.getElement(1, 1)) > eps) {
            double m3 = Aug.getElement(2, 1) / Aug.getElement(1, 1);
            std::cout << "  R3 <- R3 + (" << -m3 << ")*R2  (eliminate below pivot in column 2)\n" << std::endl;
            Aug.addRowMultiple(2, 1, -m3);
            Aug.printAugmented();
        }

        std::cout << "  Augmented matrix is now in upper triangular form.\n" << std::endl;

        // Check for singular: diagonal entry (2,2) must be non-zero
        if (std::abs(Aug.getElement(2, 2)) < eps) {
            std::cout << "  Pivot (3,3) is zero: system has no unique solution." << std::endl;
            return;
        }

        // --- Back substitution ---
        std::cout << "\n" << std::string(54, '=') << std::endl;
        std::cout << "STEP " << stepNum++ << ": Back substitution" << std::endl;
        std::cout << std::string(54, '=') << std::endl;
        std::cout << "  From the triangular system:\n" << std::endl;

        double z = Aug.getElement(2, 3) / Aug.getElement(2, 2);
        std::cout << "  Row 3: " << Aug.getElement(2, 2) << "*z = " << Aug.getElement(2, 3)
                  << "  =>  z = " << Aug.getElement(2, 3) << " / " << Aug.getElement(2, 2)
                  << " = " << std::setprecision(6) << z << std::endl;

        double y = (Aug.getElement(1, 3) - Aug.getElement(1, 2) * z) / Aug.getElement(1, 1);
        std::cout << "  Row 2: " << Aug.getElement(1, 1) << "*y + " << Aug.getElement(1, 2)
                  << "*z = " << Aug.getElement(1, 3) << "  =>  y = ("
                  << Aug.getElement(1, 3) << " - " << Aug.getElement(1, 2) << "*" << z
                  << ") / " << Aug.getElement(1, 1) << " = " << y << std::endl;

        double x = (Aug.getElement(0, 3) - Aug.getElement(0, 1) * y - Aug.getElement(0, 2) * z) / Aug.getElement(0, 0);
        std::cout << "  Row 1: " << Aug.getElement(0, 0) << "*x + " << Aug.getElement(0, 1)
                  << "*y + " << Aug.getElement(0, 2) << "*z = " << Aug.getElement(0, 3)
                  << "  =>  x = (" << Aug.getElement(0, 3) << " - " << Aug.getElement(0, 1)
                  << "*" << y << " - " << Aug.getElement(0, 2) << "*" << z << ") / "
                  << Aug.getElement(0, 0) << " = " << x << std::endl;

        std::cout << "\n" << std::string(54, '=') << std::endl;
        std::cout << "SOLUTION" << std::endl;
        std::cout << std::string(54, '=') << std::endl;
        std::cout << "  x = " << std::setprecision(6) << x << std::endl;
        std::cout << "  y = " << y << std::endl;
        std::cout << "  z = " << z << std::endl;
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
            std::cout << "Invalid input. Please enter a number between 1-9." << std::endl;
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
                handleLinearSystem3x3();
                break;
            case 9:
                std::cout << "\nThank you for using Matrix Calculator. Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice. Please enter a number between 1-9." << std::endl;
                break;
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
    
    return 0;
}
