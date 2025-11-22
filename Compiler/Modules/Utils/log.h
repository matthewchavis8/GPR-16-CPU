/** @file
 *  @brief Lightweight logging helper that prints an error and throws.
 */
#pragma once

#include <exception>
#include <string_view>
#include <iostream>
#include <string>
#include <type_traits>

/**
 * @brief Log an error message to stderr and throw an exception of the given type.
 * @tparam Exception Exception type deriving from `std::exception`.
 * @param msg        Error message to log and use as the exception text.
 */
template <typename Exception>
inline void log(std::string_view msg) {
  static_assert(std::is_base_of_v<std::exception, Exception>, "Must pass a Exception");

  std::cerr << "[ERROR] " << msg << '\n';
  throw Exception(std::string(msg));
}
