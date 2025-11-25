#include <cstdint>

/**
 * @brief Graphics library for memory-mapped 512x256 monochrome screen
 * 
 * Screen mapped at address 16384. Coordinate system: (0,0) at top-left,
 * X increases right (0-511), Y increases down (0-255)
 */
class GprScreen {
  private:
    static inline bool m_s_color { true };
    
    /**
     * @brief Low-level pixel setter bypassing color state
     * @param dx X-coordinate
     * @param dy Y-coordinate
     * @param isOn true for black, false for white
     */
    static void setPixelRaw(uint16_t dx, uint16_t dy, bool isOn);

  public:
    static constexpr uint16_t WIDTH        { 512 };
    static constexpr uint16_t HEIGHT       { 256 };
    static constexpr uint16_t SCREEN_BASE { 16384 };

    /** @brief Clears entire screen to white */
    static void clearScreen();
    
    /**
     * @brief Sets drawing color for subsequent operations
     * @param black true for black, false for white
     */
    static void setColor(bool black);
    
    /**
     * @brief Draws pixel using current color (ignores out-of-bounds)
     * @param dx X-coordinate (0-511)
     * @param dy Y-coordinate (0-255)
     */
    static void drawPixel(uint16_t dx, uint16_t dy);
    
    /**
     * @brief Draws line using Bresenham's algorithm
     * @param x1 Start X-coordinate
     * @param y1 Start Y-coordinate
     * @param x2 End X-coordinate
     * @param y2 End Y-coordinate
     */
    static void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    
    /**
     * @brief Draws filled rectangle (corner order doesn't matter)
     * @param x1 First corner X
     * @param y1 First corner Y
     * @param x2 Opposite corner X
     * @param y2 Opposite corner Y
     */
    static void drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    
    /**
     * @brief Draws circle outline using Midpoint Circle Algorithm
     * @param x Center X-coordinate
     * @param y Center Y-coordinate
     * @param r Radius in pixels
     */
    static void drawCircle(uint16_t x, uint16_t y, uint16_t r);
};
