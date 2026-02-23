// matrix.cpp — Matrix class implementation: storage, arithmetic, Gauss–Jordan (RREF, inverse).

#include "matrix.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>

namespace {
  constexpr double kEpsilon = 1e-10;
}

Matrix::Matrix(std::size_t rows, std::size_t cols)
  : rows_(rows), cols_(cols), data_(rows * cols, 0.0) {}

Matrix::Matrix(const std::vector<std::vector<double>>& data) {
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
      throw std::invalid_argument("Matrix: inconsistent row lengths in vector<vector<double>>");
  }
  data_.reserve(rows_ * cols_);
  for (const auto& row : data)
    data_.insert(data_.end(), row.begin(), row.end());
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> init) {
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

double& Matrix::operator()(std::size_t row, std::size_t col) {
  boundsCheck(row, col);
  return data_[index(row, col)];
}

double Matrix::operator()(std::size_t row, std::size_t col) const {
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
      double a_ik = data_[index(i, k)];
      for (std::size_t j = 0; j < other.cols_; ++j)
        result.data_[result.index(i, j)] += a_ik * other.data_[other.index(k, j)];
    }
  return result;
}

Matrix Matrix::operator*(double scalar) const {
  Matrix result(rows_, cols_);
  for (std::size_t i = 0; i < data_.size(); ++i)
    result.data_[i] = data_[i] * scalar;
  return result;
}

Matrix Matrix::operator/(double scalar) const {
  if (std::abs(scalar) < kEpsilon) {
    throw std::invalid_argument("Matrix division by scalar: scalar is zero (or too small).");
  }
  return *this * (1.0 / scalar);
}

// --- Gauss–Jordan: RREF with partial pivoting ---
Matrix Matrix::rref() const {
  Matrix M = *this;
  std::size_t lead = 0;
  for (std::size_t r = 0; r < M.rows_ && lead < M.cols_; ++r) {
    // Partial pivot: find row with largest |M(row, lead)|
    std::size_t pivotRow = r;
    double pivotVal = std::abs(M(r, lead));
    for (std::size_t i = r + 1; i < M.rows_; ++i) {
      double v = std::abs(M(i, lead));
      if (v > pivotVal) {
        pivotVal = v;
        pivotRow = i;
      }
    }
    if (pivotVal < kEpsilon) {
      ++lead;
      --r;
      continue;
    }
    // Swap rows r and pivotRow
    if (pivotRow != r) {
      for (std::size_t c = 0; c < M.cols_; ++c)
        std::swap(M(r, c), M(pivotRow, c));
    }
    double pivot = M(r, lead);
    for (std::size_t c = 0; c < M.cols_; ++c)
      M(r, c) /= pivot;
    for (std::size_t i = 0; i < M.rows_; ++i) {
      if (i == r) continue;
      double factor = M(i, lead);
      for (std::size_t c = 0; c < M.cols_; ++c)
        M(i, c) -= factor * M(r, c);
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
    aug(i, n + i) = 1.0;
  }
  std::size_t lead = 0;
  for (std::size_t r = 0; r < n && lead < n; ++r) {
    std::size_t pivotRow = r;
    double pivotVal = std::abs(aug(r, lead));
    for (std::size_t i = r + 1; i < n; ++i) {
      double v = std::abs(aug(i, lead));
      if (v > pivotVal) {
        pivotVal = v;
        pivotRow = i;
      }
    }
    if (pivotVal < kEpsilon) {
      std::ostringstream oss;
      oss << "Matrix inverse: matrix is singular (no pivot in column " << lead + 1 << ").";
      throw std::runtime_error(oss.str());
    }
    if (pivotRow != r) {
      for (std::size_t c = 0; c < 2 * n; ++c)
        std::swap(aug(r, c), aug(pivotRow, c));
    }
    double pivot = aug(r, lead);
    for (std::size_t c = 0; c < 2 * n; ++c)
      aug(r, c) /= pivot;
    for (std::size_t i = 0; i < n; ++i) {
      if (i == r) continue;
      double factor = aug(i, lead);
      for (std::size_t c = 0; c < 2 * n; ++c)
        aug(i, c) -= factor * aug(r, c);
    }
    ++lead;
  }
  Matrix inv(n, n);
  for (std::size_t i = 0; i < n; ++i)
    for (std::size_t j = 0; j < n; ++j)
      inv(i, j) = aug(i, n + j);
  return inv;
}

bool Matrix::approxEqual(const Matrix& a, const Matrix& b, double tolerance) {
  if (a.rows_ != b.rows_ || a.cols_ != b.cols_)
    return false;
  for (std::size_t i = 0; i < a.data_.size(); ++i)
    if (std::abs(a.data_[i] - b.data_[i]) > tolerance)
      return false;
  return true;
}
