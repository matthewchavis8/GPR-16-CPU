#pragma once

#include "../Memory/gprMemory.h"
#include "../String/gprString.h"
#include <cstdint>

/**
 * @brief Keyboard input handler using memory-mapped I/O
 * 
 * Keyboard mapped at address 24576. Special codes: 128 (newline), 129 (backspace)
 */
class GprKeyboard {
  private:
    /** @brief Checks if character is newline (128 or '\\n') */
    static constexpr bool isNewLine(osChar ch);
    
    /** @brief Checks if character is backspace (129 or '\\b') */
    static constexpr bool isBackSpace(osChar ch);
    
  public:
    static constexpr Word KBD_ADDR = 24576;

    /**
     * @brief Gets current key code (non-blocking)
     * @return Character code or 0 if no key pressed
     */
    static Word keyPressed();
    
    /**
     * @brief Waits for and reads a single character (blocking)
     * @return The character code
     */
    static osChar readChar();
    
    /**
     * @brief Reads a line of text until Enter (blocking, max 256 chars)
     * @param msg Optional prompt message
     * @return Entered line without newline
     */
    static GprString readLine([[maybe_unused]] const GprString& msg);
    
    /**
     * @brief Reads and parses an integer (blocking)
     * @param msg Optional prompt message
     * @return Parsed integer or 0 if invalid
     */
    static Word readInt(const GprString& msg);
};
