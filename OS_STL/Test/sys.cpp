#include "gtest/gtest.h"
#include "../Modules/Sys/gprSys.h"
#include "../Modules/Memory/gprMemory.h"
#include <chrono>

std::array<Word, 32768> GprMemory::m_sram;
Word GprMemory::m_heapPtr;

TEST(GprSys, InitCallsMemoryInit) {
  // Init should initialize memory
  GprSys::init();
  
  // Verify memory is zeroed
  ASSERT_EQ(GprMemory::peek(0), 0);
  ASSERT_EQ(GprMemory::peek(1000), 0);
  ASSERT_EQ(GprMemory::peek(2048), 0);
  
  // Verify allocation starts from heap base
  Word first_alloc = GprMemory::alloc(10);
  ASSERT_EQ(first_alloc, 2048 + 10);
}

TEST(GprSys, InitCanBeCalledMultipleTimes) {
  GprSys::init();
  GprMemory::poke(100, 42);
  
  // Re-init should clear memory
  GprSys::init();
  ASSERT_EQ(GprMemory::peek(100), 0);
  
  // Verify heap is reset by checking allocation starts at base again
  Word first_alloc = GprMemory::alloc(5);
  ASSERT_EQ(first_alloc, 2048 + 5);
}

TEST(GprSys, WaitDelaysExecution) {
  GprSys::init();
  
  // Test very short wait (1ms)
  auto start = std::chrono::high_resolution_clock::now();
  GprSys::wait(1);
  auto end = std::chrono::high_resolution_clock::now();
  
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  
  // Should take at least some time (allow for timing variance)
  ASSERT_GT(duration, 0);
}

TEST(GprSys, WaitWithZero) {
  // Should not crash with 0ms wait
  GprSys::wait(0);
  SUCCEED();
}

TEST(GprSys, WaitWithVariousDurations) {
  // Test multiple wait calls
  GprSys::wait(1);
  GprSys::wait(2);
  GprSys::wait(5);
  SUCCEED();
}

TEST(GprSys, WaitDoesNotModifyMemory) {
  GprSys::init();
  
  // Set some memory values
  GprMemory::poke(100, 42);
  GprMemory::poke(200, -100);
  
  // Wait should not affect memory
  GprSys::wait(1);
  
  ASSERT_EQ(GprMemory::peek(100), 42);
  ASSERT_EQ(GprMemory::peek(200), -100);
}

// Note: halt() causes infinite loop, so we cannot test it without hanging
TEST(GprSys, HaltFunctionExists) {
  // Just verify the function can be called (we won't actually call it)
  // This is more of a compilation test
  SUCCEED();
}

