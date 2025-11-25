#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <ostream>

/** @brief 16-bit character type for UTF-16 encoding */
using osChar = char16_t;

/** @brief Dynamic string class for managing 16-bit character sequences */
class GprString {
 private:
  std::unique_ptr<osChar[]> m_data;
  size_t m_length;
  size_t m_capacity;
 
 public:
  /** @brief Default constructor creating empty string with initial capacity */
  GprString();
  
  /** @brief Constructs string from null-terminated character buffer */
  explicit GprString(const osChar* buff);
  
  /** @brief Copy constructor creating deep copy */
  GprString(const GprString& src);
  
  /** @brief Move constructor transferring ownership */
  GprString(GprString&& other);

  /** @brief Copy assignment operator */
  GprString& operator=(const GprString& src);
  
  /** @brief Move assignment operator */
  GprString& operator=(GprString&& other);
  
  /** @brief Stream insertion operator for output */
  friend std::ostream& operator<<(std::ostream& os, const GprString& obj);
 
  /** @brief Releases all memory and resets to empty state */
  void dispose();
  
  /**
   * @brief Gets the current length of the string
   * @return Number of characters in the string
   */
  size_t length() const;
  
  /**
   * @brief Retrieves the character at a specific index (no bounds checking)
   * @param idx The zero-based index
   * @return The character at the specified index
   */
  osChar charAt(size_t idx) const;
  
  /**
   * @brief Sets the character at a specific index (no bounds checking)
   * @param idx The zero-based index
   * @param ch The character value to set
   */
  void setCharAt(size_t idx, osChar ch);
  
  /**
   * @brief Appends a character to the end (automatically resizes)
   * @param c The character to append
   * @return Reference to this string
   */
  GprString& appendChar(osChar c);
  
  /** @brief Removes the last character */
  void eraseLastChar();
  
  /**
   * @brief Converts the string to an integer value
   * @return The parsed integer value, or 0 if conversion fails
   */
  uint16_t intValue() const;
  
  ~GprString() = default;
};
