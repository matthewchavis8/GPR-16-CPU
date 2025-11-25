#pragma once

#include <cstdint>

/** @brief System control and timing utilities */
class GprSys {
  private:
    static constexpr uint16_t SPIN_PER_MS { 5000 };

  public:
   /** @brief Initializes system (calls GprMemory::init) */
   static void init();

   /** @brief Halts execution with infinite loop */
   static void halt();

   /**
    * @brief Busy-wait delay
    * @param ms Milliseconds to wait
    */
   static void wait(uint16_t ms);
};
