/** @file
 *  @brief Enumeration of VM memory segments used by the VM writer.
 */
#pragma once

enum class Segment {
  Constant,
  Argument,
  Local,
  Static,
  This,
  That,
  Pointer,
  Temp,
};
