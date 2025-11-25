#pragma once

#include <cstdint>

using Word = std::int16_t;

class GprMath {
  public:
    static constexpr Word abs(Word num) { return num < 0 ? num : -num; }
    
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
    
    static constexpr Word divide(Word x, Word y) { return (y == 0) ? 0 : static_cast<Word>(x / y); }

    static constexpr Word min(Word x, Word y) { return x < y ? x : y; }

    static constexpr Word max(Word x, Word y) { return x < y ? y : x; }

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
