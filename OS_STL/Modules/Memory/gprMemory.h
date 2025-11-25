#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

/** @brief 16-bit signed integer type used throughout the OS_STL library */
using Word = int16_t;

/**
 * @brief Memory management class providing RAM simulation and allocation
 * 
 * Simulates 32KB RAM with heap starting at address 2048.
 * Memory Layout: 0-2047 (system), 2048-16383 (heap), 16384-24575 (screen), 24576 (keyboard)
 */
class GprMemory {
  private:
    static constexpr size_t RAM_SIZE  { 32768 };
    static constexpr Word HEAP_BASE   { 2048 };
    static std::array<Word, RAM_SIZE> m_sram;
    static Word m_heapPtr;

  public:
    /** @brief Initializes memory system and zeros all RAM */
    static void init();
    
    /**
     * @brief Reads a word from memory (no bounds checking)
     * @param addr The memory address
     * @return The value at the address
     */
    static Word peek(Word addr);
    
    /**
     * @brief Writes a word to memory (no bounds checking)
     * @param addr The memory address
     * @param value The value to write
     */
    static void poke(Word addr, Word value);
    
    /**
     * @brief Gets pointer to raw memory array
     * @return Pointer to RAM array
     */
    static Word* data();

    /**
     * @brief Allocates memory from heap (bump allocator, no deallocation)
     * @param size Number of words to allocate
     * @return Base address of allocated block
     */
    static Word alloc(size_t size);
};
