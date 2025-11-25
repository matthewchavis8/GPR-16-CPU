#include "gprString.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>

constexpr size_t DEFAULT_CAP { 16 };

GprString::GprString()
 : m_data(std::make_unique<osChar[]>(DEFAULT_CAP))
 , m_length(0)
 , m_capacity(DEFAULT_CAP)
 {}

GprString::GprString(const osChar* data) 
 : m_data(nullptr)
 , m_length(0)
 , m_capacity(DEFAULT_CAP)
{
  size_t len { 0 };
  while (data[len] != 0)
    ++len;

  m_capacity = len > 0 ? len : DEFAULT_CAP;
  m_data = std::make_unique<osChar[]>(DEFAULT_CAP);

  for (std::size_t i{}; i < len; ++i)
    m_data[i] = data[i];
  m_length = len;
}

GprString::GprString(const GprString& src)
 : m_data(std::make_unique<osChar[]>(DEFAULT_CAP))
 , m_length(src.m_length)
 , m_capacity(src.m_capacity) {
   for (size_t i{}; i < m_length; ++i)
    m_data[i] = src.m_data[i];
}

GprString::GprString(GprString&& other)
 : m_data(std::move(other.m_data))
 , m_length(other.m_length)
 , m_capacity(other.m_capacity) {
   
  other.m_data = nullptr;
  other.m_length = 0;
  other.m_capacity = 0;
}

GprString& GprString::operator=(const GprString& other) {

  m_capacity = other.m_capacity;
  m_length   = other.m_length;
  m_data     = std::make_unique<osChar[]>(DEFAULT_CAP);

  for (size_t i{}; i < m_length; ++i)
    m_data[i] = other.m_data[i];

  return *this;
}

GprString& GprString::operator=(GprString&& other) {
  m_data = std::move(other.m_data);
  m_length = other.m_length;
  m_capacity = other.m_capacity;

  other.m_length = 0;
  other.m_capacity = 0;

  return *this;
}

void GprString::dispose() { m_data.reset();
  m_length = 0;
  m_capacity = 0;
}

size_t GprString::length() const { return m_length; }

osChar GprString::charAt(size_t idx) const {
  if (idx > m_length)
    throw std::out_of_range("[ERROR] index was out of range");

  return m_data[idx];
}

void GprString::setCharAt(size_t idx, osChar ch) {
  if (idx > m_length)
    throw std::out_of_range("[ERROR] index was out of range");

  m_data[idx] = ch;
}
GprString& GprString::appendChar(osChar c) {
  // chk if at max capacity and resize
  if (m_length + 1 > m_capacity) {
    size_t newCapacity = m_capacity == 0 ? DEFAULT_CAP : m_capacity * 2;
    auto newData = std::make_unique<osChar[]>(newCapacity);

    // Copy data
    for (std::size_t i{}; i < m_length; ++i)
      newData[i] = m_data[i];

    m_data = std::move(newData);
    m_capacity = newCapacity;
  }

  m_data[m_length++] = c;
  return *this;
}
void GprString::eraseLastChar() { m_length--; }

uint16_t GprString::intValue() const {
  if (m_length == 0)
    return 0;

  bool isNegative { (m_data[0] == static_cast<osChar>('-')) };
  size_t i = isNegative ? 1 : 0;
  int32_t value {};

  for (; i < m_length; ++i) {
    osChar c = m_data[i];

    if (c < static_cast<osChar>('0') || c > static_cast<osChar>('9'))
      break;

    value = value * 10 + (static_cast<int32_t>(c) - static_cast<int32_t>('0'));
  }
  if (isNegative)
    value = -value;
  
  // Ain't gonna lie we just gonna truncate it back down lmfao
  return static_cast<int16_t>(value);
}
