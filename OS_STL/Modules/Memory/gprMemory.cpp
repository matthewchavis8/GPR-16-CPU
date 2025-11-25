#include "gprMemory.h"
#include <cassert>
#include <cstddef>

void GprMemory::init() {
  m_heapPtr = HEAP_BASE;
  m_sram.fill(0);
}

Word GprMemory::peek(Word addr) { return m_sram[addr]; }

void GprMemory::poke(Word addr, Word value) { m_sram[addr] = value; }

Word* GprMemory::data() { return m_sram.data(); }

Word GprMemory::alloc(size_t size) {
  size_t new_base = static_cast<size_t>(m_heapPtr) + size;
  assert(new_base <= RAM_SIZE && "[ERROR] Exhaused all memory");

  m_heapPtr = static_cast<Word>(new_base);
  Word base = m_heapPtr;
  return base;
}
