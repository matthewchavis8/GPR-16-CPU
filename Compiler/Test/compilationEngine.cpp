/** @file
 *  @brief GoogleTest harness for the `CompilationEngine` component.
 */

#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <string_view>
#include "../Modules/Tokenizer/tokenizer.h"
#include "../Modules/CompilationEngine/compileEngine.h"
#include "../Modules/VMWriter/vmWriter.h"

using namespace testing;

/**
 * @brief Test fixture that creates a temporary Jack source file and runs it
 *        through the `CompilationEngine` to produce VM output.
 */
class CompilationEngineTestObject : public ::testing::Test {
  protected:
    std::filesystem::path jackPath;
    std::filesystem::path vmPath;
    std::ifstream input;
    std::ofstream output;
    std::unique_ptr<Tokenizer> tokenizer;
    std::unique_ptr<VmWriter> vmWriter;
    std::unique_ptr<CompilationEngine> engine;

    /**
     * @brief Rebuilds the temporary Jack and VM files and reinitializes
     *        `Tokenizer` and `CompilationEngine` from the provided source.
     */
    void rebuildWithSource(std::initializer_list<std::string_view> lines) {
      if (tokenizer)
        tokenizer->close();
      tokenizer.reset();
      vmWriter.reset();
      engine.reset();

      if (input.is_open())
        input.close();
      if (output.is_open())
        output.close();
      
      input.clear();
      output.clear();

      {
        std::ofstream jack(jackPath);
        for (auto line : lines)
          jack << line << '\n';
        jack.flush();
        jack.close();
      }

      input.open(jackPath, std::ios::in);
      if (!input.is_open())
        throw std::runtime_error("Failed to open jack file for reading");
      
      output.open(vmPath, std::ios::out | std::ios::trunc);
      if (!output.is_open())
        throw std::runtime_error("Failed to open vm file for writing");

      tokenizer = std::make_unique<Tokenizer>(input);
      vmWriter = std::make_unique<VmWriter>(output);
      engine = std::make_unique<CompilationEngine>(*tokenizer, *vmWriter);
    }

    void SetUp() override {
      jackPath = std::filesystem::temp_directory_path() / "ce_tmp.jack";
      vmPath  = std::filesystem::temp_directory_path() / "ce_tmp.vm";

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
      if (!vmPath.empty())
        std::filesystem::remove(vmPath);
    }
};

/** @test
 *  @brief Compiles a minimal class and asserts that non-empty VM code containing
 *         a function declaration and return statement is produced.
 */
TEST_F(CompilationEngineTestObject, CompilationEngine_CompilesSimpleClassToVM) {
  ASSERT_TRUE(tokenizer);
  ASSERT_TRUE(engine);

  EXPECT_NO_THROW(engine->compileClass());

  output.flush();
  output.close();

  std::ifstream vm(vmPath);
  ASSERT_TRUE(vm.is_open());

  std::string vmContent((std::istreambuf_iterator<char>(vm)),
                         std::istreambuf_iterator<char>());

  EXPECT_FALSE(vmContent.empty());
  EXPECT_NE(vmContent.find("function Main.main"), std::string::npos);
  EXPECT_NE(vmContent.find("push constant 0"), std::string::npos);
  EXPECT_NE(vmContent.find("return"), std::string::npos);
}

/** @test
 *  @brief Exercises `var` declaration and a simple `let` assignment and checks
 *         that the corresponding VM code is present.
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

  std::ifstream vm(vmPath);
  ASSERT_TRUE(vm.is_open());

  std::string vmContent((std::istreambuf_iterator<char>(vm)),
                         std::istreambuf_iterator<char>());

  EXPECT_NE(vmContent.find("function Main.main 1"), std::string::npos);
  EXPECT_NE(vmContent.find("push constant 1"), std::string::npos);
  EXPECT_NE(vmContent.find("pop local 0"), std::string::npos);
  EXPECT_NE(vmContent.find("return"), std::string::npos);
}

/** @test
 *  @brief Verifies that `if`/`else`, `while`, `do` calls, and `return`
 *         statements are compiled into the expected VM code with labels and calls.
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

  std::ifstream vm(vmPath);
  ASSERT_TRUE(vm.is_open());

  std::string vmContent((std::istreambuf_iterator<char>(vm)),
                         std::istreambuf_iterator<char>());

  EXPECT_NE(vmContent.find("if-goto"), std::string::npos);
  EXPECT_NE(vmContent.find("label"), std::string::npos);
  EXPECT_NE(vmContent.find("goto"), std::string::npos);
  EXPECT_NE(vmContent.find("call Output.printInt"), std::string::npos);
  EXPECT_NE(vmContent.find("call Output.println"), std::string::npos);
  EXPECT_NE(vmContent.find("return"), std::string::npos);
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

