/**
 * @file codeWriter.cpp
 * @brief Unit tests for the CodeWriter interface and assembly emission.
 */
#include "gtest/gtest.h"
#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include "../Modules/CodeWriter/codeWriter.h"

using namespace testing;

/**
 * @class CodeWriterTestObject
 * @brief Test fixture for CodeWriter class unit tests.
 *
 * Creates temporary VM and assembly files to test assembly code generation
 * from VM commands via the CodeWriter interface.
 */
class CodeWriterTestObject : public ::testing::Test {
  protected:
    /** @brief Path to the temporary VM file used for testing. */
    std::filesystem::path vm_filepath;
    /** @brief Path to the output assembly file. */
    std::filesystem::path asm_filepath;

    /** @brief Input file stream for reading the test VM file. */
    std::ifstream filestream;

    /** @brief CodeWriter instance under test. */
    std::unique_ptr<CodeWriter> codeWriter;

    /**
     * @brief Initializes test environment before each test case.
     *
     * Creates a temporary `.vm` file with sample VM commands and an output
     * `.asm` file path. Constructs the CodeWriter instance.
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
     * Closes the file stream and removes the temporary VM and assembly files.
     */
    void TearDown() override {
      filestream.close();
      std::filesystem::remove(vm_filepath);
      std::filesystem::remove(asm_filepath);
    }
};

/**
 * @brief Verifies that the CodeWriter can be successfully constructed.
 */
TEST_F(CodeWriterTestObject, canIntializeCodeWriter) {
    ASSERT_TRUE(codeWriter);
}

/**
 * @brief Tests that all arithmetic/logic commands emit correct assembly.
 *
 * Validates writeArithmetic() for all nine operations (add, sub, neg, eq,
 * gt, lt, and, or, not) by checking for expected instruction patterns.
 */
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

/**
 * @brief Tests push/pop operations across all memory segments.
 *
 * Validates writePushPop() for all seven segments (local, argument, this,
 * that, temp, pointer, static, constant) by verifying emitted assembly contains
 * expected stack pointer and memory access instructions.
 */
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
