// matrix.hpp — Reusable Matrix class for linear algebra.
// Supports construction, accessors, +/−/×/÷, RREF, and inverse (Gauss–Jordan with partial pivoting).

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <vector>

class Matrix {
public:
  // --- Construction ---
  Matrix(std::size_t rows, std::size_t cols);
  Matrix(const std::vector<std::vector<double>>& data);
  Matrix(std::initializer_list<std::initializer_list<double>> init);

  // --- Accessors ---
  std::size_t rows() const { return rows_; }
  std::size_t cols() const { return cols_; }
  double& operator()(std::size_t row, std::size_t col);
  double operator()(std::size_t row, std::size_t col) const;

  // --- Arithmetic ---
  Matrix operator+(const Matrix& other) const;
  Matrix operator-(const Matrix& other) const;
  Matrix operator*(const Matrix& other) const;
  Matrix operator*(double scalar) const;
  Matrix operator/(double scalar) const;

  // --- RREF and inverse ---
  Matrix rref() const;
  Matrix inverse() const;

  // --- Comparison (approximate equality with tolerance) ---
  static bool approxEqual(const Matrix& a, const Matrix& b, double tolerance = 1e-9);

private:
  std::size_t rows_;
  std::size_t cols_;
  std::vector<double> data_;  // row-major: index = row * cols_ + col

  void boundsCheck(std::size_t row, std::size_t col) const;
  std::size_t index(std::size_t row, std::size_t col) const { return row * cols_ + col; }
};

// scalar * Matrix (non-member)
inline Matrix operator*(double scalar, const Matrix& m) { return m * scalar; }

#endif // MATRIX_HPP
