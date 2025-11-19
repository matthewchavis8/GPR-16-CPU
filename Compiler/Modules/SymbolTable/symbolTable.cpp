
#include "symbolTable.h"
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <utility>
#include "../Utils/identifier.h"

void SymbolTable::reset() {
  m_table.clear();

  m_argCnt = 0;
  m_fieldCnt = 0;
  m_staticCnt = 0;
  m_varCnt = 0;
}

void SymbolTable::define(std::string_view name, std::string_view type, IdentifierKind identifier) {
  switch (identifier) {
    case IdentifierKind::Static:
      m_staticCnt++;
      break;
    case IdentifierKind::Arg:
      m_argCnt++;
      break;
    case IdentifierKind::Var:
      m_varCnt++;
      break;
    case IdentifierKind::Field:
      m_fieldCnt++;
      break;
    default:
      throw std::runtime_error("[ERROR] Invalid identifier kind in define()\n");
  }

  std::string symbolName {  static_cast<std::string>(name) };
  std::string symbolType {  static_cast<std::string>(type) };
  Symbol symbol { symbolType, identifier, m_Idx++};

  m_table.emplace(std::move(symbolName), std::move(symbol));
}

uint32_t SymbolTable::varCount(IdentifierKind identifier) const {
  switch (identifier) {
    case IdentifierKind::Static: return m_staticCnt;
    case IdentifierKind::Arg:    return m_argCnt;
    case IdentifierKind::Var:    return m_varCnt;
    case IdentifierKind::Field:  return m_fieldCnt;
    default:                     throw std::runtime_error("[ERROR] No Vars at all in the table\n");
  }
}

IdentifierKind SymbolTable::kindOf(std::string_view name) const {
  auto it { m_table.find(static_cast<std::string>(name)) };
  if (it != m_table.end())
    return it->second.kind;

  return IdentifierKind::None;
}

std::string SymbolTable::typeOf(std::string_view name) const {
  auto it { m_table.find(static_cast<std::string>(name)) };
  if (it != m_table.end())
    return it->second.type;

  throw std::runtime_error("[ERROR] Could not find the symbol type\n");
}

uint32_t SymbolTable::indexOf(std::string_view name) const {
  auto it { m_table.find(static_cast<std::string>(name)) };
  if (it != m_table.end())
    return it->second.idx;

  throw std::runtime_error("[ERROR] Could not find the symbol index\n");
}
