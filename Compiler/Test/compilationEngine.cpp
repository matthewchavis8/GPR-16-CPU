/** @file
 *  @brief GoogleTest harness for the `CompilationEngine` component.
 */

#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include "../Modules/Tokenizer/tokenizer.h"
#include "../Modules/CompilationEngine/compileEngine.h"

using namespace testing;

/**
 * @brief Test fixture that creates a temporary Jack source file and runs it
 *        through the `CompilationEngine` to produce XML output.
 */
class CompilationEngineTestObject : public ::testing::Test {
  protected:
    std::filesystem::path jackPath;
    std::filesystem::path xmlPath;
    std::ifstream input;
    std::ofstream output;
    std::unique_ptr<Tokenizer> tokenizer;
    std::unique_ptr<CompilationEngine> engine;

    /**
     * @brief Rebuilds the temporary Jack and XML files and reinitializes
     *        `Tokenizer` and `CompilationEngine` from the provided source.
     */
    void rebuildWithSource(std::initializer_list<std::string_view> lines) {
      if (tokenizer)
        tokenizer->close();
      tokenizer.reset();

      if (input.is_open())
        input.close();
      if (output.is_open())
        output.close();

      {
        std::ofstream jack(jackPath);
        for (auto line : lines)
          jack << line << '\n';
      }

      input.open(jackPath, std::ios::in);
      output.open(xmlPath, std::ios::out | std::ios::trunc);

      tokenizer = std::make_unique<Tokenizer>(input, jackPath.string());
      engine = std::make_unique<CompilationEngine>(*tokenizer, output);
    }

    void SetUp() override {
      jackPath = std::filesystem::temp_directory_path() / "ce_tmp.jack";
      xmlPath  = std::filesystem::temp_directory_path() / "ce_tmp.xml";

      rebuildWithSource({
        "class Main {",
        "  function void main ( ) {",
        "    return ;",
        "  }",
        "}"
      });
    }

    void TearDown() override {
      if (tokenizer)
        tokenizer->close();
      if (output.is_open())
        output.close();
      if (!jackPath.empty())
        std::filesystem::remove(jackPath);
      if (!xmlPath.empty())
        std::filesystem::remove(xmlPath);
    }
};

/** @test
 *  @brief Compiles a minimal class and asserts that non-empty XML containing
 *         `<class>` and the `class` keyword is produced.
 */
TEST_F(CompilationEngineTestObject, CompilationEngine_CompilesSimpleClassToXml) {
  ASSERT_TRUE(tokenizer);
  ASSERT_TRUE(engine);

  EXPECT_NO_THROW(engine->compileClass());

  output.flush();
  output.close();

  std::ifstream xml(xmlPath);
  ASSERT_TRUE(xml.is_open());

  std::string xmlContent((std::istreambuf_iterator<char>(xml)),
                         std::istreambuf_iterator<char>());

  EXPECT_FALSE(xmlContent.empty());
  EXPECT_NE(xmlContent.find("<class>"), std::string::npos);
  EXPECT_NE(xmlContent.find("</class>"), std::string::npos);
  EXPECT_NE(xmlContent.find("<keyword> class </keyword>"), std::string::npos);
}

/** @test
 *  @brief Exercises `var` declaration and a simple `let` assignment and checks
 *         that the corresponding XML nodes are present.
 */
TEST_F(CompilationEngineTestObject, CompilationEngine_ParsesVarAndLetStatement) {
  rebuildWithSource({
    "class Main {",
    "  function void main ( ) {",
    "    var int x ;",
    "    let x = 1 ;",
    "    return ;",
    "  }",
    "}"
  });

  ASSERT_TRUE(tokenizer);
  ASSERT_TRUE(engine);

  EXPECT_NO_THROW(engine->compileClass());

  output.flush();
  output.close();

  std::ifstream xml(xmlPath);
  ASSERT_TRUE(xml.is_open());

  std::string xmlContent((std::istreambuf_iterator<char>(xml)),
                         std::istreambuf_iterator<char>());

  EXPECT_NE(xmlContent.find("<varDec>"), std::string::npos);
  EXPECT_NE(xmlContent.find("<letStatement>"), std::string::npos);
  EXPECT_NE(xmlContent.find("<keyword> let </keyword>"), std::string::npos);
  EXPECT_NE(xmlContent.find("<identifier> x </identifier>"), std::string::npos);
  EXPECT_NE(xmlContent.find("<intConstant> 1 </intConstant>"), std::string::npos);
}

/** @test
 *  @brief Verifies that `if`/`else`, `while`, `do` calls, and `return`
 *         statements are compiled into the expected XML elements.
 */
TEST_F(CompilationEngineTestObject, CompilationEngine_ParsesIfWhileDoAndReturn) {
  rebuildWithSource({
    "class Main {",
    "  function void main ( ) {",
    "    var int x ;",
    "    let x = 0 ;",
    "    if ( x ) {",
    "      let x = 1 ;",
    "    } else {",
    "      let x = 2 ;",
    "    }",
    "    while ( x ) {",
    "      do Output . printInt ( x ) ;",
    "      let x = x - 1 ;",
    "    }",
    "    do Output . println ( ) ;",
    "    return ;",
    "  }",
    "}"
  });

  ASSERT_TRUE(tokenizer);
  ASSERT_TRUE(engine);

  EXPECT_NO_THROW(engine->compileClass());

  output.flush();
  output.close();

  std::ifstream xml(xmlPath);
  ASSERT_TRUE(xml.is_open());

  std::string xmlContent((std::istreambuf_iterator<char>(xml)),
                         std::istreambuf_iterator<char>());

  EXPECT_NE(xmlContent.find("<ifStatement>"), std::string::npos);
  EXPECT_NE(xmlContent.find("<whileStatement>"), std::string::npos);
  EXPECT_NE(xmlContent.find("<doStatement>"), std::string::npos);
  EXPECT_NE(xmlContent.find("<returnStatement>"), std::string::npos);
}

/** @test
 *  @brief Confirms that a malformed `let` statement missing a semicolon causes
 *         `compileClass()` to throw `std::runtime_error`.
 */
TEST_F(CompilationEngineTestObject, CompilationEngine_ReportsErrorOnMissingSemicolonInLet) {
  rebuildWithSource({
    "class Main {",
    "  function void main ( ) {",
    "    let x = 1",
    "  }",
    "}"
  });

  ASSERT_TRUE(tokenizer);
  ASSERT_TRUE(engine);

  EXPECT_THROW(engine->compileClass(), std::runtime_error);
}

