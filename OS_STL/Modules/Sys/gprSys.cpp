
#include "gprSys.h"
#include "../Memory/gprMemory.h"
#include <cstdint>

void GprSys::init() {  GprMemory::init(); }

void GprSys::halt() {
  // Spin forever
  for (;;) {}
}

void GprSys::wait(uint16_t ms) {
  for (uint16_t i{}; i < ms; ++i) {
    for (uint16_t j{}; j < SPIN_PER_MS; ++j) {
      // Prevent agressive optimization
      asm volatile("" ::: "memory");
    }
  }
}
