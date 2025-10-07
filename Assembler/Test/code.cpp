#include "gtest/gtest.h"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include "../Modules/Parser/parser.h"
#include "../Modules/Code/code.h"

using namespace testing;

/**
 * @class ParserTestObject
 * @brief Test fixture for Parser class unit tests.
 *
 * Creates a temporary assembly file with representative commands and 
 * initializes a Parser instance for testing command parsing operations.
 */
class CodeTestObject : public ::testing::Test {
  protected:
    // @brief Path to the temporary assembly file used for testing
    std::filesystem::path filepath;
    
    // @brief Input file stream for reading the test assembly file.
    std::ifstream filestream;
    
    // @brief Parser instance under test.
    std::unique_ptr<Parser> parser;
    // @brief Code instance under test.
    std::unique_ptr<Code> code = std::make_unique<Code>();

    /**
     * @brief Initializes test environment before each test case.
     *
     * Creates a temporary `.asm` file with a sample assembly program containing
     * A-commands, C-commands, and label declarations. Opens the file stream and
     * constructs the Parser instance.
     */
    void SetUp() override {
    // Setting up temporary directory
    filepath = std::filesystem::temp_directory_path() / "tmp.asm";
    
   // Setting up file path with text
   {
    std::ofstream file(filepath);
    file << "@2" << '\n';
    file << "D=A" << '\n';
    file << "D=D+A" << '\n';
    file << "@0" << '\n';
    file << "M=D" << '\n';
    file << "(LOOP)" << '\n';
    file << "@LOOP" << '\n';
    file << "0;JMP" << '\n';
   }

    filestream.open(filepath, std::ios::in);

    parser = std::make_unique<Parser>(filestream, filepath.string());

   }

    /**
     * @brief Cleans up test environment after each test case.
     *
     * Closes the file stream and removes the temporary assembly file.
     */
    void TearDown() override {
      filestream.close();
      std::filesystem::remove(filepath);
    }
};

TEST_F(CodeTestObject, canReadDestBits) {
  ASSERT_TRUE(parser);  

  while (parser->commandType() != CommandType::C_COMMAND)
    parser->advance();

  std::string mnemo { parser->dest() };
  //
  std::uint8_t expectedDestBits { 0b010 };
  std::uint8_t destBits { code->dest(mnemo) };

  ASSERT_EQ(expectedDestBits, destBits);
}

TEST_F(CodeTestObject, canReadCompBits) {
  ASSERT_TRUE(parser);  

  while (parser->commandType() != CommandType::C_COMMAND)
    parser->advance();

  std::string mnemo { parser->comp() };
  //
  std::uint8_t expectedCompBits { 0b0110000 };
  std::uint8_t compBits { code->comp(mnemo) };

  ASSERT_EQ(expectedCompBits, compBits);
}

TEST_F(CodeTestObject, canReadJmpBits) {
  ASSERT_TRUE(parser);  

  while (parser->hasMoreCommands())
    parser->advance();

  std::string mnemo { parser->jump() };
  //
  std::uint8_t expectedJmpBits { 0b111 };
  std::uint8_t jmpBits { code->jump(mnemo) };

  ASSERT_EQ(expectedJmpBits, jmpBits);
}
