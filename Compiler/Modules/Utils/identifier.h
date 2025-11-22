/** @file
 *  @brief Enumeration of Jack identifier kinds (storage categories).
 */
#pragma once

enum class IdentifierKind {
  Static,
  Field,
  Arg,
  Var,
  None,
};
