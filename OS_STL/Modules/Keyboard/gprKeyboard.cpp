#include "gprKeyboard.h"

constexpr bool GprKeyboard::isNewLine(osChar ch) { return ch == static_cast<osChar>(128) || ch == u'\n'; }
constexpr bool GprKeyboard::isBackSpace(osChar ch) { return ch == static_cast<osChar>(129) || ch == u'\b'; }

Word GprKeyboard::keyPressed() { return GprMemory::peek(KBD_ADDR); }

osChar GprKeyboard::readChar() {
  Word code {};
  while (code == 0)
    code = GprMemory::peek(KBD_ADDR);

  osChar ch = static_cast<osChar>(code);
  
  // Spin until something is pressed
  while (GprMemory::peek(KBD_ADDR) != 0) {

  }


  return ch;
}

GprString GprKeyboard::readLine([[maybe_unused]] const GprString& msg) {
  constexpr Word MAX_LEN { 256 };
  GprString line;

  while (true) {
    osChar c = readChar();

    if (isNewLine(c)) {
      break;
    } else if (isBackSpace(c)) {
      if (line.length() > 0) {
        line.eraseLastChar();
      }
    } else {
      if (line.length() < MAX_LEN)
        line.appendChar(c);
    }
  }
  
  return line;
}

Word GprKeyboard::readInt(const GprString &msg) {
  GprString line = readLine(msg);
  return static_cast<Word>(line.intValue());
}
