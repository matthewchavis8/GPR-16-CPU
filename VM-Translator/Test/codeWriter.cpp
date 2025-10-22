#include "gtest/gtest.h"
#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string_view>
#include "../Modules/CodeWriter/codeWriter.h"

using namespace testing;

/**
 * @class ParserTestObject
 * @brief Test fixture for Parser class unit tests.
 *
 * Creates a temporary assembly file with representative commands and
 * initializes a Parser instance for testing command parsing operations.
 */
class CodeWriterTestObject : public ::testing::Test {
  protected:
    // @brief Path to the temporary assembly file used for testing
    std::filesystem::path vm_filepath;
    std::filesystem::path asm_filepath;

    // @brief Input file stream for reading the test assembly file.
    std::ifstream filestream;

    std::unique_ptr<CodeWriter> codeWriter;
  

    // @brief Parser instance under test.

    /**
     * @brief Initializes test environment before each test case.
     *
     * Creates a temporary `.asm` file with a sample assembly program containing
     * A-commands, C-commands, and label declarations. Opens the file stream and
     * constructs the Parser instance.
     */
    void SetUp() override {
    // Setting up temporary directory
    vm_filepath = std::filesystem::temp_directory_path() / "tmp.vm";
    asm_filepath = std::filesystem::temp_directory_path() / "asm.asm";

   // Setting up file path with text
   {
    std::ofstream file(vm_filepath);
    file << "push constant 2" << '\n';
    file << "push constant 2" << '\n';
    file << "add" << '\n';
    file << "pop local 0" << '\n';
    file << "label LOOP" << '\n';
    file << "goto LOOP" << '\n';
   }
    filestream.open(vm_filepath, std::ios::in);

    codeWriter = std::make_unique<CodeWriter>(asm_filepath.string());
   }

    /**
     * @brief Cleans up test environment after each test case.
     *
     * Closes the file stream and removes the temporary assembly file.
     */
    void TearDown() override {
      filestream.close();
      std::filesystem::remove(vm_filepath);
      std::filesystem::remove(asm_filepath);
    }
};


TEST_F(CodeWriterTestObject, canIntializeCodeWriter) {
    ASSERT_TRUE(codeWriter);
}

TEST_F(CodeWriterTestObject, canWriteArithmetric) {
  ASSERT_TRUE(codeWriter);

  std::array<std::string, 9> cmds {
    "add",
    "sub",
    "neg",
    "eq",
    "gt",
    "lt",
    "and",
    "or",
    "not",
  };

  for (const std::string& cmd : cmds)
    codeWriter->writeArithmetic(cmd);

  codeWriter->close();

  std::ifstream asmFile(asm_filepath);
  ASSERT_TRUE(asmFile.is_open());
  
  std::string expected (std::istreambuf_iterator<char>(asmFile), {});

  EXPECT_NE(expected.find("M=M+D"), std::string::npos);
  EXPECT_NE(expected.find("M=M-D"), std::string::npos);
  EXPECT_NE(expected.find("M=-M"), std::string::npos);
  EXPECT_NE(expected.find("D;JEQ"), std::string::npos);
  EXPECT_NE(expected.find("D;JGT"), std::string::npos);
  EXPECT_NE(expected.find("D;JLT"), std::string::npos);
  EXPECT_NE(expected.find("M=M&D"), std::string::npos);
  EXPECT_NE(expected.find("M=M|D"), std::string::npos);
  EXPECT_NE(expected.find("M=!M"), std::string::npos);
}

TEST_F(CodeWriterTestObject, canWritePushPopCommands) {
    ASSERT_TRUE(codeWriter);

    std::array<std::string, 7> segments = {
        "local", "argument", "this",
        "that", "temp", "pointer", "static"
    };

    codeWriter->writePushPop(CommandType::C_PUSH, "constant", 0);
    for (const std::string& seg : segments) {
        codeWriter->writePushPop(CommandType::C_POP, seg, 0);
        codeWriter->writePushPop(CommandType::C_PUSH, seg, 0);
    }
  
    

    codeWriter->close();

    std::ifstream asmFile(asm_filepath);
    ASSERT_TRUE(asmFile.is_open());

    std::string content((std::istreambuf_iterator<char>(asmFile)), {});

    EXPECT_NE(content.find("@SP"), std::string::npos);
    EXPECT_NE(content.find("D=M"), std::string::npos);
    EXPECT_NE(content.find("M=D"), std::string::npos);
}
