#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <ostream>

using osChar = char16_t;

class GprString {
 private:
  std::unique_ptr<osChar[]> m_data;
  size_t m_length;
  size_t m_capacity;
 
 public:
  // Constructors
  GprString();
  explicit GprString(const osChar* buff);          // Initialize it with a string 
  GprString(const GprString& src);        // initialize it with a temp obj
  GprString(GprString&& other);           // Move Constructor

  GprString& operator=(const GprString& src); // Copy assignment
  GprString& operator=(GprString&& other);    // Copy assignment R values
  

  friend std::ostream& operator<<(std::ostream& os, const GprString& obj);   // Operator overload for <<
 
  // CORE API'S
  void dispose();
  size_t length() const;
  osChar charAt(size_t idx) const;
  void setCharAt(size_t idx, osChar ch);
  GprString& appendChar(osChar c);
  void eraseLastChar();
  uint16_t intValue() const;
  
  ~GprString() = default;
};
