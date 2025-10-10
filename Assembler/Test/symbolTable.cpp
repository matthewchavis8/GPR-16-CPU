#include "../Modules/SymbolTable/symbolTable.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <memory>
#include <string>

/**
 * @brief Verifies that a symbol can be added and subsequently found in the table.
 */
TEST(SymbolTableHarness, canAddEntry) {
  auto symbolTable = std::make_unique<SymbolTable>();

  ASSERT_TRUE(symbolTable);

  std::pair<std::string, uint16_t> pair { "=", 0x1243 };

  symbolTable->addEntry(pair.first, pair.second);

  ASSERT_TRUE(symbolTable->contains("="));
}

/**
 * @brief Verifies that contains() correctly returns false for unregistered symbols.
 */
TEST(SymbolTableHarness, canContain) {
  auto symbolTable = std::make_unique<SymbolTable>();

  ASSERT_TRUE(symbolTable);

  std::pair<std::string, uint16_t> pair { "=", 0x1243 };

  symbolTable->addEntry(pair.first, pair.second);

  ASSERT_FALSE(symbolTable->contains(":)"));
}

/**
 * @brief Verifies that getAddress() retrieves the correct address for a registered symbol.
 */
TEST(SymbolTableHarness, canGetAddress) {
  auto symbolTable = std::make_unique<SymbolTable>();

  ASSERT_TRUE(symbolTable);

  std::pair<std::string, uint16_t> pair { "=", 0x1243 };

  symbolTable->addEntry(pair.first, pair.second);

  ASSERT_EQ(symbolTable->getAddress("="), 0x1243);
}

