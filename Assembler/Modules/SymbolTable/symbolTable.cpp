#include "symbolTable.h"
#include <cstdint>
#include <optional>
#include <string_view>

void SymbolTable::addEntry(std::string_view symbol, uint16_t address) {
  m_hashTable.try_emplace(std::string(symbol), address);
}

bool SymbolTable::contains(std::string_view symbol) const {
  auto key { m_hashTable.find(std::string(symbol)) };
  if (key != m_hashTable.end())
    return true;

  return false;
}

std::optional<uint16_t> SymbolTable::getAddress(std::string_view symbol) const {
  auto key { m_hashTable.find(std::string(symbol)) };
  if (key != m_hashTable.end())
    return key->second;

  return std::nullopt;
}
