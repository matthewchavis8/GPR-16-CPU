#include "gtest/gtest.h"

// Not gonna lie using hacks here for accessing private members
#define private public
#include "../Modules/String/gprString.h"

void checkStr(size_t i, const osChar* expected, const osChar* result) {
  for (size_t ch {}; ch < i; ++ch)
    EXPECT_EQ(expected[ch], result[ch]);
};
  
TEST(GprString, GprStringConstructors) {
  // Defualt constructor
  GprString str1;
  ASSERT_NE(str1.m_data, nullptr);
  ASSERT_EQ(str1.m_length, 0);
  ASSERT_EQ(str1.m_capacity, 16);
  
  // Constructor intialize with buffer
  const osChar* test_data = u"Matthew";
  GprString str2(test_data);
  ASSERT_EQ(str2.m_length, 7);
  ASSERT_EQ(str2.m_capacity, 7);
  checkStr(7, test_data, str2.m_data.get());
  
  // Copy Constructor and Object copied from should be set to 0
  GprString tmpStr1(u"Matthew");
  GprString str3(std::move(tmpStr1));
  ASSERT_EQ(str3.m_length, 7);
  ASSERT_EQ(str3.m_capacity, 7);
  checkStr(7, test_data, str3.m_data.get());
  // Checking if previous copied object is 0
  ASSERT_EQ(tmpStr1.m_data, nullptr);
  ASSERT_EQ(tmpStr1.m_capacity, 0);
  ASSERT_EQ(tmpStr1.m_length, 0);
  
  // Move Constructor
  GprString tmpStr2(u"Matthew");
  GprString str4 = std::move(tmpStr2);
  ASSERT_EQ(str4.m_length, 7);
  ASSERT_EQ(str4.m_capacity, 7);
  checkStr(7, test_data, str4.m_data.get());
  // Checking if previous copied object is 0
  ASSERT_EQ(tmpStr2.m_data, nullptr);
  ASSERT_EQ(tmpStr2.m_capacity, 0);
  ASSERT_EQ(tmpStr2.m_length, 0);

  // @brief Copy Assignemnt
  GprString stringToCpy1(u"hughMongus");
  GprString str5 = stringToCpy1;
  checkStr(stringToCpy1.m_length, stringToCpy1.m_data.get(), str5.m_data.get());

  // @brief Move Assignement
  GprString stringToCpy2(u"hughMongus");
  GprString str6 = std::move(stringToCpy2);
  ASSERT_EQ(stringToCpy2.m_data, nullptr);
  ASSERT_EQ(stringToCpy2.m_capacity, 0);
  ASSERT_EQ(stringToCpy2.m_length, 0);
  const osChar* strToComp = u"hughMongus";
  checkStr(str6.m_length, str6.m_data.get(), strToComp);
}

TEST(GprString, GprStringDispose) {
  GprString str(u"hughMongus");
  str.dispose();
  ASSERT_EQ(str.m_data, nullptr);
  ASSERT_EQ(str.m_capacity, 0);
  ASSERT_EQ(str.m_length, 0);
}

TEST(GprString, GprStringStats) {
  GprString str(u"hughMongus");
  ASSERT_EQ(str.m_capacity, 10);
  ASSERT_EQ(str.m_length, 10);
}

TEST(GprString, GprStringCharAt) {
  GprString str(u"hughMongus");
  const osChar expected = str.charAt(0);
  ASSERT_EQ(expected, u'h');
}

TEST(GprString, GprStringSetCharAt) {
  GprString str(u"Matt");
  str.setCharAt(0, u'k');
  const osChar* expected = u"katt";
  checkStr(str.m_length, expected, str.m_data.get());
}
TEST(GprString, GprStringAppendChar) {
  GprString str(u"Matt");
  osChar appendCh = u'C';
  str.appendChar(appendCh);
  const osChar* expected = u"MattC";
  checkStr(0, expected, str.m_data.get());
}

TEST(GprString, GprStringEraseLastChar) {
  GprString str(u"Matt");
  str.eraseLastChar();
  const osChar* expected = u"Mat";
  checkStr(str.m_length, expected, str.m_data.get());
}

TEST(GprString, GprStringIntValue) {
  GprString str(u"123");
  ASSERT_EQ(str.intValue(), 123);
}
