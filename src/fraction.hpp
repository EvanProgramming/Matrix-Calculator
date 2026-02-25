// fraction.hpp — Exact rational number (numerator/denominator) for matrix input/output and arithmetic.

#ifndef FRACTION_HPP
#define FRACTION_HPP

#include <cstdint>
#include <string>

class Fraction {
public:
  Fraction();                          // 0/1
  Fraction(std::int64_t numerator, std::int64_t denominator = 1);

  std::int64_t numerator() const { return num_; }
  std::int64_t denominator() const { return denom_; }

  Fraction operator+(const Fraction& other) const;
  Fraction operator-(const Fraction& other) const;
  Fraction operator*(const Fraction& other) const;
  Fraction operator/(const Fraction& other) const;
  Fraction operator-() const;

  bool operator==(const Fraction& other) const;
  bool operator!=(const Fraction& other) const { return !(*this == other); }
  bool operator>(const Fraction& other) const;
  bool operator<(const Fraction& other) const { return other > *this; }

  bool isZero() const { return num_ == 0; }
  Fraction abs() const;

  // Parse "a/b", "a", "a.b" (decimal). Invalid/empty -> 0/1.
  static Fraction fromString(const std::string& s);
  // Display as "3", "1/2", "-2/5".
  std::string toString() const;

  double toDouble() const;

private:
  std::int64_t num_;
  std::int64_t denom_;  // always > 0

  void normalize();
  static std::int64_t gcd(std::int64_t a, std::int64_t b);
};

#endif // FRACTION_HPP
