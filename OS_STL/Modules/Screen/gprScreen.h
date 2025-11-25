#pragma once

#include <cstdint>

class GprScreen {
  private:
    static constexpr uint16_t m_WIDTH        { 512 };
    static constexpr uint16_t m_HEIGHT       { 256 };
    static constexpr uint16_t m_SCREEN_BASE { 16384 };
    static inline bool m_s_color { true };
    
    static void setPixelRaw(uint16_t dx, uint16_t dy, bool isOn);

  public:
    static void clearScreen();
    static void setColor(bool black);
    static void drawPixel(uint16_t dx, uint16_t dy);
    static void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    static void drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    static void drawCircle(uint16_t x, uint16_t y, uint16_t r);
};
