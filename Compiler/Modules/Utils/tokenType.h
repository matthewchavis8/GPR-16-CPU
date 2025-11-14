/** @file
 *  @brief Enumeration of high-level token categories produced by the tokenizer.
 */

#pragma once

enum class Token {
  Keyword,
  Symbol,
  Identifier,
  IntConst,
  StringConst
};
