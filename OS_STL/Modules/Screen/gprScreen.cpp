#include "gprScreen.h"
#include "../Memory/gprMemory.h"
#include <cstdint>

void GprScreen::setPixelRaw(uint16_t dx, uint16_t dy, bool isOn) {
  if (dx < 0 || dx >= m_WIDTH || dy < 0 || dy >= m_HEIGHT)
    return;
  uint16_t wordIdx = (dy * (m_WIDTH / 16)) + (dx / 16);
  uint16_t addr = m_SCREEN_BASE + static_cast<uint16_t>(wordIdx);
  uint16_t word = GprMemory::peek(addr);

  uint16_t bit = dx % 16;
  uint16_t mask = static_cast<uint16_t>(1u << bit);

  if (isOn)
    word |= mask;
  else
   word &= ~mask;

  GprMemory::poke(addr, word);
}
void GprScreen::clearScreen() {}
void setColor(bool black);
void drawPixel(uint16_t dx, uint16_t dy);
void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void drawCircle(uint16_t x, uint16_t y, uint16_t r);
