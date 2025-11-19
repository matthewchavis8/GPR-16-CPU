
#include "gtest/gtest.h"
#include <cstdint>
#include <stdexcept>
#include <string>
#include "../Modules/SymbolTable/symbolTable.h"
#include "../Modules/Utils/identifier.h"

using namespace testing;

class SymbolTable_F : public ::testing::Test {
  protected:
    SymbolTable m_table;
    
    void TearDown() override {
      m_table.reset();
    }
};

TEST_F(SymbolTable_F, SymbolTable_Define_Works_Correctly) {
  // Add Key1
  m_table.define("this", "Point", IdentifierKind::Arg);
  std::string_view name1 { "this" };

  IdentifierKind kind1   { m_table.kindOf(name1) };
  uint32_t idx1          { m_table.indexOf(name1) };
  std::string type1      { m_table.typeOf(name1) };
  
  // Check if table correctly added entry
  ASSERT_EQ(kind1, IdentifierKind::Arg);
  ASSERT_EQ(idx1, 0);
  ASSERT_EQ(type1, "Point");
  
  // Add key2
  m_table.define("other", "Point", IdentifierKind::Arg);
  std::string_view name2 { "other" };
  IdentifierKind kind2   { m_table.kindOf(name2) };
  uint32_t idx2          { m_table.indexOf(name2) };
  std::string type2      { m_table.typeOf(name2) };
  
  // Check if table correctly added entry
  ASSERT_EQ(kind2, IdentifierKind::Arg);
  ASSERT_EQ(idx2, 1);
  ASSERT_EQ(type2, "Point");
}

TEST_F(SymbolTable_F, does_symbolTable_properly_reset) {
  m_table.define("other", "Point", IdentifierKind::Arg);
  m_table.define("this", "Point", IdentifierKind::Arg);
  
  // check if argument count is correct
  uint32_t currentArgCnt { m_table.varCount(IdentifierKind::Arg) };
  ASSERT_EQ(currentArgCnt, 2);

  m_table.reset();
  currentArgCnt = m_table.varCount(IdentifierKind::Arg);
  // Arg count should be 0 after reset
  ASSERT_EQ(currentArgCnt, 0);
  
  // Table should handle exceptions gracifully
  ASSERT_EQ(m_table.kindOf("other"), IdentifierKind::None);
  EXPECT_THROW(m_table.typeOf("other"), std::runtime_error);
  EXPECT_THROW(m_table.indexOf("other"), std::runtime_error);
}

TEST_F(SymbolTable_F, check_table_count_is_correct) {
  m_table.define("m_x", "func", IdentifierKind::Static);
  m_table.define("m_y", "func", IdentifierKind::Static);
  m_table.define("m_z", "func", IdentifierKind::Static);
  m_table.define("m_l", "func", IdentifierKind::Static);

  m_table.define("ax", "int", IdentifierKind::Arg);
  m_table.define("ay", "int", IdentifierKind::Arg);
  m_table.define("az", "int", IdentifierKind::Arg);

  m_table.define("name", "Array", IdentifierKind::Var);
  m_table.define("x", "int", IdentifierKind::Var);

  m_table.define("objx", "Obj", IdentifierKind::Field);

  uint32_t staticCnt   { m_table.varCount(IdentifierKind::Static) };
  uint32_t argCnt      { m_table.varCount(IdentifierKind::Arg) };
  uint32_t varCnt      { m_table.varCount(IdentifierKind::Var) };
  uint32_t fieldCnt   { m_table.varCount(IdentifierKind::Field) };

  ASSERT_EQ(staticCnt, 4);
  ASSERT_EQ(argCnt, 3);
  ASSERT_EQ(varCnt, 2);
  ASSERT_EQ(fieldCnt, 1);
  EXPECT_THROW(m_table.varCount(IdentifierKind::None), std::runtime_error);
}
