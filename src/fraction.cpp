// fraction.cpp — Fraction implementation: normalize, arithmetic, parse, format.

#include "fraction.hpp"
#include <cstdlib>
#include <sstream>
#include <stdexcept>

std::int64_t Fraction::gcd(std::int64_t a, std::int64_t b) {
  a = std::abs(a);
  b = std::abs(b);
  while (b != 0) {
    std::int64_t t = b;
    b = a % b;
    a = t;
  }
  return a;
}

void Fraction::normalize() {
  if (denom_ == 0)
    throw std::invalid_argument("Fraction: denominator is zero.");
  if (denom_ < 0) {
    num_ = -num_;
    denom_ = -denom_;
  }
  std::int64_t g = gcd(num_, denom_);
  if (g != 0) {
    num_ /= g;
    denom_ /= g;
  }
}

Fraction::Fraction() : num_(0), denom_(1) {}

Fraction::Fraction(std::int64_t numerator, std::int64_t denominator)
  : num_(numerator), denom_(denominator) {
  normalize();
}

Fraction Fraction::operator+(const Fraction& other) const {
  return Fraction(num_ * other.denom_ + other.num_ * denom_, denom_ * other.denom_);
}

Fraction Fraction::operator-(const Fraction& other) const {
  return Fraction(num_ * other.denom_ - other.num_ * denom_, denom_ * other.denom_);
}

Fraction Fraction::operator*(const Fraction& other) const {
  return Fraction(num_ * other.num_, denom_ * other.denom_);
}

Fraction Fraction::operator/(const Fraction& other) const {
  if (other.num_ == 0)
    throw std::invalid_argument("Fraction: division by zero.");
  return Fraction(num_ * other.denom_, denom_ * other.num_);
}

Fraction Fraction::operator-() const {
  return Fraction(-num_, denom_);
}

bool Fraction::operator==(const Fraction& other) const {
  return num_ == other.num_ && denom_ == other.denom_;
}

bool Fraction::operator>(const Fraction& other) const {
  Fraction diff = *this - other;
  return diff.num_ > 0;
}

Fraction Fraction::abs() const {
  return Fraction(num_ < 0 ? -num_ : num_, denom_);
}

Fraction Fraction::fromString(const std::string& s) {
  std::string t;
  for (char c : s) {
    if (c != ' ' && c != '\t') t += c;
  }
  if (t.empty()) return Fraction(0, 1);

  std::size_t slash = t.find('/');
  if (slash != std::string::npos) {
    std::int64_t num = 0, denom = 1;
    try {
      num = std::stoll(t.substr(0, slash));
      denom = std::stoll(t.substr(slash + 1));
    } catch (...) {
      return Fraction(0, 1);
    }
    return Fraction(num, denom);
  }

  std::size_t dot = t.find('.');
  if (dot != std::string::npos) {
    std::string intPart = t.substr(0, dot);
    std::string fracPart = t.substr(dot + 1);
    std::int64_t whole = 0;
    try {
      whole = std::stoll(intPart);
    } catch (...) {
      whole = 0;
    }
    std::int64_t fracNum = 0;
    std::int64_t fracDenom = 1;
    if (!fracPart.empty()) {
      try {
        fracNum = std::stoll(fracPart);
        for (std::size_t i = 0; i < fracPart.size(); ++i)
          fracDenom *= 10;
      } catch (...) {}
    }
    return Fraction(whole, 1) + Fraction(fracNum, fracDenom);
  }

  try {
    return Fraction(std::stoll(t), 1);
  } catch (...) {
    return Fraction(0, 1);
  }
}

std::string Fraction::toString() const {
  if (denom_ == 1)
    return std::to_string(num_);
  return std::to_string(num_) + "/" + std::to_string(denom_);
}

double Fraction::toDouble() const {
  return static_cast<double>(num_) / static_cast<double>(denom_);
}
