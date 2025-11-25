#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

using Word = uint16_t;

class GprMemory {
  private:
    static constexpr size_t RAM_SIZE  { 32768 };
    static constexpr Word HEAP_BASE   { 2048 };
    static std::array<Word, RAM_SIZE> m_sram;
    static Word m_heapPtr;

  public:
    static void init();
    static Word peek(Word addr);
    static void poke(Word addr, Word value);
    static Word* data();

    static Word alloc(size_t size);
};
