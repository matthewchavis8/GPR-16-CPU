#include "../Modules/SymbolTable/symbolTable.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <memory>
#include <string>

TEST(SymbolTableHarness, canAddEntry) {
  auto symbolTable = std::make_unique<SymbolTable>();

  ASSERT_TRUE(symbolTable);

  std::pair<std::string, uint16_t> pair { "=", 0x1243 };

  symbolTable->addEntry(pair.first, pair.second);

  ASSERT_TRUE(symbolTable->contains("="));
}

TEST(SymbolTableHarness, canContain) {
  auto symbolTable = std::make_unique<SymbolTable>();

  ASSERT_TRUE(symbolTable);

  std::pair<std::string, uint16_t> pair { "=", 0x1243 };

  symbolTable->addEntry(pair.first, pair.second);

  ASSERT_FALSE(symbolTable->contains(":)"));
}

TEST(SymbolTableHarness, canGetAddress) {
  auto symbolTable = std::make_unique<SymbolTable>();

  ASSERT_TRUE(symbolTable);

  std::pair<std::string, uint16_t> pair { "=", 0x1243 };

  symbolTable->addEntry(pair.first, pair.second);

  ASSERT_EQ(symbolTable->getAddress("="), 0x1243);
}

