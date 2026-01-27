# Matrix Calculator

A comprehensive C++ matrix calculator with a clean command-line interface that supports various matrix operations.

## Features

- **Matrix Addition**: Add two matrices of the same dimensions
- **Matrix Subtraction**: Subtract two matrices of the same dimensions
- **Matrix Multiplication**: Multiply two compatible matrices
- **Scalar Multiplication**: Multiply a matrix by a scalar value
- **Matrix Transpose**: Compute the transpose of any matrix
- **Matrix Determinant**: Calculate the determinant of square matrices
- **Matrix Inverse**: Compute the inverse of invertible square matrices

## Building

To compile the program, use the provided Makefile:

```bash
make
```

This will create an executable named `matrix_calculator`.

## Running

To run the program:

```bash
make run
```

Or directly:

```bash
./matrix_calculator
```

## Usage

1. Run the program and select an operation from the menu (1-8)
2. Enter the dimensions of your matrix(es) when prompted
3. Input matrix elements row by row (space-separated values)
4. View the result

## Example

```
1. Matrix Addition (A + B)
2. Matrix Subtraction (A - B)
3. Matrix Multiplication (A * B)
...
Enter your choice: 1

Enter number of rows: 2
Enter number of columns: 2

Matrix A:
Row 1: 1 2
Row 2: 3 4

Matrix B:
Row 1: 5 6
Row 2: 7 8

Result (A + B):
|       6.000       8.000 |
|      10.000      12.000 |
```

## Error Handling

The program includes comprehensive error handling for:
- Invalid matrix dimensions
- Dimension mismatches for operations
- Singular matrices (non-invertible)
- Invalid user input

## Requirements

- C++11 or later
- A C++ compiler (g++, clang++, etc.)

## Clean Build

To remove compiled files:

```bash
make clean
```
