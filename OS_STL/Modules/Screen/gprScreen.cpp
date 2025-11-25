#include "gprScreen.h"
#include "../Memory/gprMemory.h"
#include "../Math/gprMath.h"
#include <cstdint>

void GprScreen::setPixelRaw(uint16_t dx, uint16_t dy, bool isOn) {
  if (dx >= WIDTH || dy >= HEIGHT)
    return;

  uint16_t wordIdx = (dy * (WIDTH / 16)) + (dx / 16);
  uint16_t addr = SCREEN_BASE + static_cast<uint16_t>(wordIdx);
  uint16_t word = GprMemory::peek(addr);

  uint16_t bit = dx % 16;
  uint16_t mask = static_cast<uint16_t>(1u << bit);

  if (isOn)
   word |= mask;
  else
   word &= ~mask;

  GprMemory::poke(addr, word);
}

void GprScreen::clearScreen() {
  for (int i{}; i < 8192; ++i)
    GprMemory::poke(SCREEN_BASE + i, 0);
}
void GprScreen::setColor(bool black) { m_s_color = black; }

void GprScreen::drawPixel(uint16_t dx, uint16_t dy) { setPixelRaw(dx, dy, m_s_color); }

void GprScreen::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  uint16_t dx = GprMath::abs(x2 - x1);
  uint16_t dy = GprMath::abs(y2 - y1);

  uint16_t sx = (x1 < x2) ? 1 : -1;
  uint16_t sy = (y1 < y2) ? 1 : -1;

  uint16_t err = dx + dy;

  while (true) {
    drawPixel(x1, x2);

    if (x1 == x2 && y1 == y2)
      break;

    uint16_t e2 = 2 * err;

    if (e2 >= dy) {
      err += dy;
      x1 += sx;
    } 

    if (e2 <= dx) {
      err += dx;
      y1 += sy;
    }
  }

}

void GprScreen::drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  uint16_t left  = GprMath::min(x1, x2);
  uint16_t right = GprMath::max(x1, x2);
  uint16_t top   = GprMath::min(y1, y2);
  uint16_t down  = GprMath::max(y1, y2);

  for (int i = top; i <= down; ++i)
    drawLine(left, i, right, i);
}

void GprScreen::drawCircle(uint16_t x, uint16_t y, uint16_t r) {
  uint16_t cx {};
  uint16_t cy { r };
  uint16_t di = 3 - 2 * r;

  auto drawOctants = [&](uint16_t cx, uint16_t cy) {
    drawPixel(x + cx, y + cy);
    drawPixel(x - cx, y + cy);
    drawPixel(x + cx, y - cy);
    drawPixel(x - cx, y - cy);
    drawPixel(x + cx, y + cy);
    drawPixel(x - cx, y + cy);
    drawPixel(x + cx, y - cy);
    drawPixel(x - cx, y - cy);
  };

  drawOctants(cx, cy);

  while (cy >= cx) {
    cx++;
    if (di > 0) {
      cy--;
      di = di + 4 * (cx - cy) + 10;
    } else {
      di = di + 4 * cx + 6;
    }
    drawOctants(cx, cy);
  }
}
