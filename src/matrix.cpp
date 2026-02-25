// matrix.cpp — Matrix class implementation: storage, arithmetic, Gauss–Jordan (RREF, inverse).

#include "matrix.hpp"
#include <algorithm>
#include <sstream>

Matrix::Matrix(std::size_t rows, std::size_t cols)
  : rows_(rows), cols_(cols), data_(rows * cols, Fraction(0, 1)) {}

Matrix::Matrix(const std::vector<std::vector<Fraction>>& data) {
  if (data.empty()) {
    rows_ = 0;
    cols_ = 0;
    data_.clear();
    return;
  }
  rows_ = data.size();
  cols_ = data[0].size();
  for (const auto& row : data) {
    if (row.size() != cols_)
      throw std::invalid_argument("Matrix: inconsistent row lengths in vector<vector<Fraction>>");
  }
  data_.reserve(rows_ * cols_);
  for (const auto& row : data)
    data_.insert(data_.end(), row.begin(), row.end());
}

Matrix::Matrix(std::initializer_list<std::initializer_list<Fraction>> init) {
  if (init.size() == 0) {
    rows_ = 0;
    cols_ = 0;
    data_.clear();
    return;
  }
  rows_ = init.size();
  cols_ = init.begin()->size();
  data_.reserve(rows_ * cols_);
  for (const auto& row : init) {
    if (row.size() != cols_)
      throw std::invalid_argument("Matrix: inconsistent row lengths in initializer_list");
    data_.insert(data_.end(), row.begin(), row.end());
  }
}

void Matrix::boundsCheck(std::size_t row, std::size_t col) const {
#ifdef NDEBUG
  (void)row;
  (void)col;
#else
  if (row >= rows_ || col >= cols_) {
    std::ostringstream oss;
    oss << "Matrix index out of bounds: (" << row << ", " << col
        << ") for matrix of size " << rows_ << "x" << cols_;
    throw std::out_of_range(oss.str());
  }
#endif
}

Fraction& Matrix::operator()(std::size_t row, std::size_t col) {
  boundsCheck(row, col);
  return data_[index(row, col)];
}

const Fraction& Matrix::operator()(std::size_t row, std::size_t col) const {
  boundsCheck(row, col);
  return data_[index(row, col)];
}

Matrix Matrix::operator+(const Matrix& other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    std::ostringstream oss;
    oss << "Matrix addition: dimension mismatch (" << rows_ << "x" << cols_
        << ") vs (" << other.rows_ << "x" << other.cols_ << ")";
    throw std::invalid_argument(oss.str());
  }
  Matrix result(rows_, cols_);
  for (std::size_t i = 0; i < data_.size(); ++i)
    result.data_[i] = data_[i] + other.data_[i];
  return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    std::ostringstream oss;
    oss << "Matrix subtraction: dimension mismatch (" << rows_ << "x" << cols_
        << ") vs (" << other.rows_ << "x" << other.cols_ << ")";
    throw std::invalid_argument(oss.str());
  }
  Matrix result(rows_, cols_);
  for (std::size_t i = 0; i < data_.size(); ++i)
    result.data_[i] = data_[i] - other.data_[i];
  return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
  if (cols_ != other.rows_) {
    std::ostringstream oss;
    oss << "Matrix multiplication: dimension mismatch (" << rows_ << "x" << cols_
        << ") * (" << other.rows_ << "x" << other.cols_ << ")";
    throw std::invalid_argument(oss.str());
  }
  Matrix result(rows_, other.cols_);
  for (std::size_t i = 0; i < rows_; ++i)
    for (std::size_t k = 0; k < cols_; ++k) {
      const Fraction& a_ik = data_[index(i, k)];
      for (std::size_t j = 0; j < other.cols_; ++j)
        result.data_[result.index(i, j)] = result.data_[result.index(i, j)] + a_ik * other.data_[other.index(k, j)];
    }
  return result;
}

Matrix Matrix::operator*(const Fraction& scalar) const {
  Matrix result(rows_, cols_);
  for (std::size_t i = 0; i < data_.size(); ++i)
    result.data_[i] = data_[i] * scalar;
  return result;
}

Matrix Matrix::operator/(const Fraction& scalar) const {
  if (scalar.isZero()) {
    throw std::invalid_argument("Matrix division by scalar: scalar is zero.");
  }
  return *this * Fraction(scalar.denominator(), scalar.numerator());
}

// --- Gauss–Jordan: RREF with partial pivoting (exact Fraction arithmetic) ---
Matrix Matrix::rref() const {
  Matrix M = *this;
  std::size_t lead = 0;
  for (std::size_t r = 0; r < M.rows_ && lead < M.cols_; ++r) {
    std::size_t pivotRow = r;
    Fraction pivotVal = M(r, lead).abs();
    for (std::size_t i = r + 1; i < M.rows_; ++i) {
      Fraction v = M(i, lead).abs();
      if (v > pivotVal) {
        pivotVal = v;
        pivotRow = i;
      }
    }
    if (M(pivotRow, lead).isZero()) {
      ++lead;
      --r;
      continue;
    }
    if (pivotRow != r) {
      for (std::size_t c = 0; c < M.cols_; ++c)
        std::swap(M(r, c), M(pivotRow, c));
    }
    Fraction pivot = M(r, lead);
    for (std::size_t c = 0; c < M.cols_; ++c)
      M(r, c) = M(r, c) / pivot;
    for (std::size_t i = 0; i < M.rows_; ++i) {
      if (i == r) continue;
      Fraction factor = M(i, lead);
      for (std::size_t c = 0; c < M.cols_; ++c)
        M(i, c) = M(i, c) - factor * M(r, c);
    }
    ++lead;
  }
  return M;
}

Matrix Matrix::inverse() const {
  if (rows_ != cols_) {
    std::ostringstream oss;
    oss << "Matrix inverse: matrix must be square (got " << rows_ << "x" << cols_ << ").";
    throw std::invalid_argument(oss.str());
  }
  std::size_t n = rows_;
  Matrix aug(n, 2 * n);
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < n; ++j)
      aug(i, j) = (*this)(i, j);
    aug(i, n + i) = Fraction(1, 1);
  }
  std::size_t lead = 0;
  for (std::size_t r = 0; r < n && lead < n; ++r) {
    std::size_t pivotRow = r;
    Fraction pivotVal = aug(r, lead).abs();
    for (std::size_t i = r + 1; i < n; ++i) {
      Fraction v = aug(i, lead).abs();
      if (v > pivotVal) {
        pivotVal = v;
        pivotRow = i;
      }
    }
    if (aug(pivotRow, lead).isZero()) {
      std::ostringstream oss;
      oss << "Matrix inverse: matrix is singular (no pivot in column " << lead + 1 << ").";
      throw std::runtime_error(oss.str());
    }
    if (pivotRow != r) {
      for (std::size_t c = 0; c < 2 * n; ++c)
        std::swap(aug(r, c), aug(pivotRow, c));
    }
    Fraction pivot = aug(r, lead);
    for (std::size_t c = 0; c < 2 * n; ++c)
      aug(r, c) = aug(r, c) / pivot;
    for (std::size_t i = 0; i < n; ++i) {
      if (i == r) continue;
      Fraction factor = aug(i, lead);
      for (std::size_t c = 0; c < 2 * n; ++c)
        aug(i, c) = aug(i, c) - factor * aug(r, c);
    }
    ++lead;
  }
  Matrix inv(n, n);
  for (std::size_t i = 0; i < n; ++i)
    for (std::size_t j = 0; j < n; ++j)
      inv(i, j) = aug(i, n + j);
  return inv;
}

bool Matrix::approxEqual(const Matrix& a, const Matrix& b) {
  if (a.rows_ != b.rows_ || a.cols_ != b.cols_)
    return false;
  for (std::size_t i = 0; i < a.data_.size(); ++i)
    if (a.data_[i] != b.data_[i])
      return false;
  return true;
}
