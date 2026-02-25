// matrix.hpp — Reusable Matrix class for linear algebra (Fraction-based).
// Supports construction, accessors, +/−/×/÷, RREF, and inverse (Gauss–Jordan with partial pivoting).

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "fraction.hpp"
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <vector>

class Matrix {
public:
  // --- Construction ---
  Matrix(std::size_t rows, std::size_t cols);
  Matrix(const std::vector<std::vector<Fraction>>& data);
  Matrix(std::initializer_list<std::initializer_list<Fraction>> init);

  // --- Accessors ---
  std::size_t rows() const { return rows_; }
  std::size_t cols() const { return cols_; }
  Fraction& operator()(std::size_t row, std::size_t col);
  const Fraction& operator()(std::size_t row, std::size_t col) const;

  // --- Arithmetic ---
  Matrix operator+(const Matrix& other) const;
  Matrix operator-(const Matrix& other) const;
  Matrix operator*(const Matrix& other) const;
  Matrix operator*(const Fraction& scalar) const;
  Matrix operator/(const Fraction& scalar) const;

  // --- RREF and inverse ---
  Matrix rref() const;
  Matrix inverse() const;

  // --- Comparison (exact equality for Fraction) ---
  static bool approxEqual(const Matrix& a, const Matrix& b);

private:
  std::size_t rows_;
  std::size_t cols_;
  std::vector<Fraction> data_;  // row-major: index = row * cols_ + col

  void boundsCheck(std::size_t row, std::size_t col) const;
  std::size_t index(std::size_t row, std::size_t col) const { return row * cols_ + col; }
};

// scalar * Matrix (non-member)
inline Matrix operator*(const Fraction& scalar, const Matrix& m) { return m * scalar; }

#endif // MATRIX_HPP
