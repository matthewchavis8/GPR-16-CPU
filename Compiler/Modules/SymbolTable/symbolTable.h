#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include "../Utils/identifier.h"

struct Symbol {
  std::string    type;
  IdentifierKind kind;
  uint32_t       idx;
};

using Table = std::unordered_map<std::string, Symbol>;

class SymbolTable {
  private:
    Table    m_table;

    uint32_t m_staticCnt{};
    uint32_t m_fieldCnt{};
    uint32_t m_argCnt{};
    uint32_t m_varCnt{};
    uint32_t m_Idx{};

  public:
    SymbolTable() = default;
    SymbolTable& operator=(const SymbolTable&) = delete;

    void reset();

    void define(std::string_view name, std::string_view type, IdentifierKind kind);

    uint32_t varCount(IdentifierKind kind) const;

    IdentifierKind kindOf(std::string_view name) const;

    std::string typeOf(std::string_view name) const;

    uint32_t indexOf(std::string_view name) const;
};
