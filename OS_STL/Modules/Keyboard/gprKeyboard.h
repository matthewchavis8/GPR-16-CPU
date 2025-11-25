#pragma once

#include "../Memory/gprMemory.h"
#include "../String/gprString.h"
#include <cstdint>


class GprKeyboard {
  private:
    static constexpr bool isNewLine(osChar ch);
    static constexpr bool isBackSpace(osChar ch);
  public:
    static constexpr Word KBD_ADDR = 24576;

    static Word keyPressed();
    static osChar readChar();
    static GprString readLine([[maybe_unused]] const GprString& msg);
    static Word readInt(const GprString& msg);
};
