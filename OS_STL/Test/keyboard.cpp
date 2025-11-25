#include "gtest/gtest.h"
#include "../Modules/Keyboard/gprKeyboard.h"
#include "../Modules/Memory/gprMemory.h"

std::array<Word, 32768> GprMemory::m_sram;
Word GprMemory::m_heapPtr;

TEST(GprKeyboard, KeyboardAddress) {
  ASSERT_EQ(GprKeyboard::KBD_ADDR, 24576);
}

TEST(GprKeyboard, KeyPressedReturnsZeroWhenNoKey) {
  GprMemory::init();
  
  // Keyboard register should be 0 by default
  Word key = GprKeyboard::keyPressed();
  ASSERT_EQ(key, 0);
}

TEST(GprKeyboard, KeyPressedReturnsValue) {
  GprMemory::init();
  
  // Simulate key press by writing to keyboard address
  GprMemory::poke(24576, 65); // ASCII 'A'
  
  Word key = GprKeyboard::keyPressed();
  ASSERT_EQ(key, 65);
}

TEST(GprKeyboard, KeyPressedReadsFromCorrectAddress) {
  GprMemory::init();
  
  // Write to different addresses
  GprMemory::poke(24575, 99);
  GprMemory::poke(24576, 42);
  GprMemory::poke(24577, 88);
  
  // Should read from 24576
  Word key = GprKeyboard::keyPressed();
  ASSERT_EQ(key, 42);
}

TEST(GprKeyboard, MultipleKeyPressReads) {
  GprMemory::init();
  
  // Test sequence of key presses
  GprMemory::poke(24576, 65);
  ASSERT_EQ(GprKeyboard::keyPressed(), 65);
  
  GprMemory::poke(24576, 66);
  ASSERT_EQ(GprKeyboard::keyPressed(), 66);
  
  GprMemory::poke(24576, 0);
  ASSERT_EQ(GprKeyboard::keyPressed(), 0);
}

// Note: readChar, readLine, and readInt are blocking functions that wait for
// actual keyboard input, so we test them with mocked keyboard state

TEST(GprKeyboard, ReadCharBlockingBehavior) {
  // This test verifies the keyPressed function which readChar depends on
  GprMemory::init();
  
  // Simulate key being pressed
  GprMemory::poke(24576, 72); // 'H'
  ASSERT_EQ(GprKeyboard::keyPressed(), 72);
  
  // Simulate key released
  GprMemory::poke(24576, 0);
  ASSERT_EQ(GprKeyboard::keyPressed(), 0);
}

TEST(GprKeyboard, SpecialCharacterCodes) {
  GprMemory::init();
  
  // Test newline code
  GprMemory::poke(24576, 128);
  ASSERT_EQ(GprKeyboard::keyPressed(), 128);
  
  // Test backspace code
  GprMemory::poke(24576, 129);
  ASSERT_EQ(GprKeyboard::keyPressed(), 129);
}

TEST(GprKeyboard, NegativeKeyValues) {
  GprMemory::init();
  
  // Test with negative values (should work as they're just Words)
  GprMemory::poke(24576, -1);
  ASSERT_EQ(GprKeyboard::keyPressed(), -1);
}

