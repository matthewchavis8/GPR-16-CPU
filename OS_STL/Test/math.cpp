#include "gtest/gtest.h"
#include <cstdint>
#include "../Modules/Math/gprMath.h"

TEST(OS_MATH, Math_Abs) {
  int16_t res { GprMath::abs(10) };
  int16_t expected { 10 };

  ASSERT_EQ(res, expected);
  
  // Test with negative input
  int16_t res2 { GprMath::abs(-10) };
  ASSERT_EQ(res2, 10);
}

TEST(OS_MATH, Math_Multiply) {
  int16_t res { GprMath::multiply(10, 10) };
  int16_t expected { 100 };

  ASSERT_EQ(res, expected);
}

TEST(OS_MATH, Math_Divide) {
  int16_t res { GprMath::divide(100, 10) };
  int16_t expected { 10 };

  ASSERT_EQ(res, expected);
}

TEST(OS_MATH, Math_Min) {
  int16_t res { GprMath::min(100, 2) };
  int16_t expected { 2 };

  ASSERT_EQ(res, expected);
}

TEST(OS_MATH, Math_Max) {
  int16_t res { GprMath::max(100, 2) };
  int16_t expected { 100 };

  ASSERT_EQ(res, expected);
}

TEST(OS_MATH, Math_sqrt) {
  int16_t res { GprMath::sqrt(25) };
  int16_t expected { 5 };

  ASSERT_EQ(res, expected);
}
