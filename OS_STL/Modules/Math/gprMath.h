#pragma once

#include <cstdint>

/** @brief 16-bit signed integer type used throughout the OS_STL library */
using Word = std::int16_t;

/** @brief Static utility class providing mathematical operations for 16-bit words */
class GprMath {
  public:
    /**
     * @brief Computes the absolute value of a number
     * @param num The input number
     * @return The absolute value of num
     */
    static constexpr Word abs(Word num) { return num < 0 ? -num : num; }
    
    /**
     * @brief Multiplies two 16-bit integers using bit-shift algorithm
     * @param x The first multiplicand
     * @param y The second multiplicand
     * @return The product of x and y
     */
    static constexpr Word multiply(Word x, Word y) {
      Word result {};
      while (y > 0) {
        if (y & 1)
          result += x;

        x <<= 1;
        y >>= 1;
      }

      return result;
    }
    
    /**
     * @brief Divides two 16-bit integers (returns 0 if divisor is 0)
     * @param x The dividend
     * @param y The divisor
     * @return The quotient of x divided by y
     */
    static constexpr Word divide(Word x, Word y) { return (y == 0) ? 0 : static_cast<Word>(x / y); }

    /**
     * @brief Returns the minimum of two values
     * @param x The first value
     * @param y The second value
     * @return The smaller of x and y
     */
    static constexpr Word min(Word x, Word y) { return x < y ? x : y; }

    /**
     * @brief Returns the maximum of two values
     * @param x The first value
     * @param y The second value
     * @return The larger of x and y
     */
    static constexpr Word max(Word x, Word y) { return x < y ? y : x; }

    /**
     * @brief Computes the integer square root using digit-by-digit method
     * @param x The input value
     * @return The integer square root of x
     */
    static constexpr Word sqrt(Word x) {
      uint32_t n = static_cast<std::uint16_t>(x);
      uint16_t res { 0 };

      uint32_t bit { 1u << 14 };

      while (bit > n)
        bit >>= 2;

      while (bit != 0) {
        if (n >= res + bit) {
          n -= res + bit;
          res = (res >> 1) + bit;
        } else {
          res >>= 1;
        }
        bit >>= 2;
      }
      return static_cast<Word>(res & 0xFFFFu);
    }
};
