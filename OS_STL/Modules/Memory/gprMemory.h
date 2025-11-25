#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>

using Word = uint16_t;
using Addr = uint16_t;

class GprMemory {
  private:
    static constexpr size_t RAM_SIZE  { 32768 };
    static constexpr Addr STACK_BASE  { 256 };
    static constexpr Addr HEAP_BASE   { 2048 };
    static constexpr Addr SCREEN_BASE { 16384 };
    static constexpr Addr KBD_ADDR    { 24576 };

    static std::array<Word, RAM_SIZE> s_ram;
   
    // This is a static util only
    GprMemory () = delete;
    ~GprMemory() = delete;
  public:
    static Word peek(Addr address);
    static void poke(Addr address, Word value);
    static void clear();
    static std::array<Word, RAM_SIZE>& data();
};
