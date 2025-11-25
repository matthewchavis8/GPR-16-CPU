#include "gtest/gtest.h"

#define private public
#include "../Modules/Memory/gprMemory.h"

std::array<Word, 32768> GprMemory::m_sram;
Word GprMemory::m_heapPtr;

TEST(GprMemory, InitializesMemory) {
  GprMemory::init();
  
  // Check heap pointer is at base
  ASSERT_EQ(GprMemory::m_heapPtr, 2048);
  
  // Check some memory is zeroed
  ASSERT_EQ(GprMemory::peek(0), 0);
  ASSERT_EQ(GprMemory::peek(100), 0);
  ASSERT_EQ(GprMemory::peek(2048), 0);
}

TEST(GprMemory, PeekAndPoke) {
  GprMemory::init();
  
  // Write and read from various addresses
  GprMemory::poke(100, 42);
  ASSERT_EQ(GprMemory::peek(100), 42);
  
  GprMemory::poke(2048, -100);
  ASSERT_EQ(GprMemory::peek(2048), -100);
  
  GprMemory::poke(16384, 12345);
  ASSERT_EQ(GprMemory::peek(16384), 12345);
}

TEST(GprMemory, DataPointerAccess) {
  GprMemory::init();
  
  Word* data_ptr = GprMemory::data();
  ASSERT_NE(data_ptr, nullptr);
  
  // Write through data pointer
  data_ptr[500] = 999;
  ASSERT_EQ(GprMemory::peek(500), 999);
}

TEST(GprMemory, AllocReturnsSequentialAddresses) {
  GprMemory::init();
  
  Word addr1 = GprMemory::alloc(10);
  ASSERT_EQ(addr1, 2048 + 10);
  
  Word addr2 = GprMemory::alloc(5);
  ASSERT_EQ(addr2, 2048 + 10 + 5);
  
  Word addr3 = GprMemory::alloc(20);
  ASSERT_EQ(addr3, 2048 + 10 + 5 + 20);
}

TEST(GprMemory, AllocBumpPointerAdvances) {
  GprMemory::init();
  
  Word initial_ptr = GprMemory::m_heapPtr;
  ASSERT_EQ(initial_ptr, 2048);
  
  GprMemory::alloc(100);
  ASSERT_EQ(GprMemory::m_heapPtr, 2048 + 100);
  
  GprMemory::alloc(50);
  ASSERT_EQ(GprMemory::m_heapPtr, 2048 + 100 + 50);
}

TEST(GprMemory, PeekPokeWithNegativeValues) {
  GprMemory::init();
  
  GprMemory::poke(1000, -1);
  ASSERT_EQ(GprMemory::peek(1000), -1);
  
  GprMemory::poke(1001, -32768);
  ASSERT_EQ(GprMemory::peek(1001), -32768);
  
  GprMemory::poke(1002, 32767);
  ASSERT_EQ(GprMemory::peek(1002), 32767);
}

