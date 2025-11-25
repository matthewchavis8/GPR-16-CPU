#include "gtest/gtest.h"
#include "../Modules/Screen/gprScreen.h"
#include "../Modules/Memory/gprMemory.h"

std::array<Word, 32768> GprMemory::m_sram;
Word GprMemory::m_heapPtr;

TEST(GprScreen, Constants) {
  ASSERT_EQ(GprScreen::WIDTH, 512);
  ASSERT_EQ(GprScreen::HEIGHT, 256);
  ASSERT_EQ(GprScreen::SCREEN_BASE, 16384);
}

TEST(GprScreen, ClearScreen) {
  GprMemory::init();
  
  // Set some screen memory to non-zero
  GprMemory::poke(16384, -1);
  GprMemory::poke(16385, -1);
  GprMemory::poke(20000, -1);
  
  GprScreen::clearScreen();
  
  // Verify screen memory is cleared
  ASSERT_EQ(GprMemory::peek(16384), 0);
  ASSERT_EQ(GprMemory::peek(16385), 0);
  ASSERT_EQ(GprMemory::peek(20000), 0);
  ASSERT_EQ(GprMemory::peek(24575), 0); // Last screen address
}

TEST(GprScreen, SetColor) {
  // Test color is set (cannot directly test static bool, but verify no crash)
  GprScreen::setColor(true);
  GprScreen::setColor(false);
  SUCCEED();
}

TEST(GprScreen, DrawPixel) {
  GprMemory::init();
  GprScreen::clearScreen();
  GprScreen::setColor(true);
  
  // Draw pixel at (0, 0)
  GprScreen::drawPixel(0, 0);
  Word value = GprMemory::peek(16384);
  ASSERT_NE(value, 0);
  
  // Clear and draw pixel at (16, 0) - should be in next word
  GprScreen::clearScreen();
  GprScreen::drawPixel(16, 0);
  ASSERT_EQ(GprMemory::peek(16384), 0); // First word should be clear
  ASSERT_NE(GprMemory::peek(16385), 0); // Second word should have pixel
}

TEST(GprScreen, DrawPixelOutOfBounds) {
  GprMemory::init();
  GprScreen::clearScreen();
  
  // These should not crash or corrupt memory
  GprScreen::drawPixel(600, 0);   // Out of bounds X
  GprScreen::drawPixel(0, 300);   // Out of bounds Y
  GprScreen::drawPixel(1000, 1000); // Way out of bounds
  
  SUCCEED();
}

TEST(GprScreen, DrawLine) {
  GprMemory::init();
  GprScreen::clearScreen();
  GprScreen::setColor(true);
  
  // Draw a horizontal line
  GprScreen::drawLine(10, 10, 20, 10);
  
  // Verify some pixels are set (line should affect memory)
  bool pixels_set = false;
  for (int addr = 16384; addr < 24576; ++addr) {
    if (GprMemory::peek(addr) != 0) {
      pixels_set = true;
      break;
    }
  }
  ASSERT_TRUE(pixels_set);
}

TEST(GprScreen, DrawRectangle) {
  GprMemory::init();
  GprScreen::clearScreen();
  GprScreen::setColor(true);
  
  // Draw a small rectangle
  GprScreen::drawRectangle(50, 50, 60, 60);
  
  // Verify some pixels are set
  bool pixels_set = false;
  for (int addr = 16384; addr < 24576; ++addr) {
    if (GprMemory::peek(addr) != 0) {
      pixels_set = true;
      break;
    }
  }
  ASSERT_TRUE(pixels_set);
}

TEST(GprScreen, DrawCircle) {
  GprMemory::init();
  GprScreen::clearScreen();
  GprScreen::setColor(true);
  
  // Draw a circle
  GprScreen::drawCircle(100, 100, 20);
  
  // Verify some pixels are set
  bool pixels_set = false;
  for (int addr = 16384; addr < 24576; ++addr) {
    if (GprMemory::peek(addr) != 0) {
      pixels_set = true;
      break;
    }
  }
  ASSERT_TRUE(pixels_set);
}

TEST(GprScreen, DrawWithBlackAndWhite) {
  GprMemory::init();
  GprScreen::clearScreen();
  
  // Draw black pixel
  GprScreen::setColor(true);
  GprScreen::drawPixel(0, 0);
  Word black_val = GprMemory::peek(16384);
  ASSERT_NE(black_val, 0);
  
  // Draw white pixel (should clear it)
  GprScreen::setColor(false);
  GprScreen::drawPixel(0, 0);
  Word white_val = GprMemory::peek(16384);
  ASSERT_EQ(white_val, 0);
}

