#pragma once

#include <cstdint>

class GprSys {
  private:
    static constexpr uint16_t SPIN_PER_MS { 5000 };

  public:
   static void init();

   static void halt();

   static void wait(uint16_t ms);
};
