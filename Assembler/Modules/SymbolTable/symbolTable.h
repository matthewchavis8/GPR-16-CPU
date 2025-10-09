#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

class SymbolTable {
  private:
    std::unordered_map<std::string, uint16_t> m_hashTable;
  public:
    SymbolTable() = default;
    SymbolTable& operator=(const SymbolTable&) = delete;

    void addEntry(std::string_view, uint16_t);

    bool contains(std::string_view) const;

  std::optional<uint16_t> getAddress(std::string_view) const;
};
