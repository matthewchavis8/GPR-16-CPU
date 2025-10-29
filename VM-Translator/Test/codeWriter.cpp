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

/**
 * @brief Tests label scoping and jumps within a function.
 *
 * Ensures that labels and jump targets are qualified as func$label
 * once writeFunction has set the current scope.
 */
TEST_F(CodeWriterTestObject, canWriteQualifiedLabelsAndJumps) {
  ASSERT_TRUE(codeWriter);

  codeWriter->writeFunction("Foo", 0);
  codeWriter->writeLabel("LOOP");
  codeWriter->writeGoto("LOOP");
  codeWriter->writeIf("END");

  codeWriter->close();

  std::ifstream asmFile(asm_filepath);
  ASSERT_TRUE(asmFile.is_open());
  std::string content((std::istreambuf_iterator<char>(asmFile)), {});

  // Function label
  EXPECT_NE(content.find("(Foo)"), std::string::npos);

  // Qualified label creation and references
  EXPECT_NE(content.find("(Foo$LOOP)"), std::string::npos);
  EXPECT_NE(content.find("@Foo$LOOP"), std::string::npos);   // goto LOOP target
  EXPECT_NE(content.find("@Foo$END"),  std::string::npos);   // if-goto END target

  // Branch mechanics for goto / if-goto
  EXPECT_NE(content.find("0;JMP"), std::string::npos);
  EXPECT_NE(content.find("@SP\nAM=M-1\nD=M"), std::string::npos);
  EXPECT_NE(content.find("D;JNE"), std::string::npos);
}

/**
 * @brief Tests that writeFunction sets scope and initializes locals to 0.
 */
TEST_F(CodeWriterTestObject, writeFunctionInitializesLocalsAndSetsScope) {
  ASSERT_TRUE(codeWriter);

  const uint32_t nLocals { 3 };
  codeWriter->writeFunction("Main", nLocals);
  codeWriter->close();

  std::ifstream asmFile(asm_filepath);
  ASSERT_TRUE(asmFile.is_open());
  std::string content((std::istreambuf_iterator<char>(asmFile)), {});

  // Function header
  EXPECT_NE(content.find("(Main)"), std::string::npos);

  // Expect three pushes of 0 (pattern used by your codegen)
  // We check occurrences of the "@0\nD=A" snippet.
  size_t pos {};
  size_t hits {};
  const std::string pushZero { "@0\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n" };
  while ((pos = content.find("@0\nD=A", pos)) != std::string::npos) {
    ++hits;
    pos += 1;
  }
  EXPECT_EQ(hits, nLocals);
}

/**
 * @brief Tests writeCall emits the full VM call sequence.
 *
 * Validates: push return-address, save LCL/ARG/THIS/THAT, ARG/LCL setup,
 * jump to callee, and emits return label. Uses nArgs=2.
 */
TEST_F(CodeWriterTestObject, writeCallEmitsPrologueAndJump) {
  ASSERT_TRUE(codeWriter);

  // Set caller function scope first so return label is "Main$ret.N"
  codeWriter->writeFunction("Main", 0);
  codeWriter->writeCall("Foo", 2);

  codeWriter->close();

  std::ifstream asmFile(asm_filepath);
  ASSERT_TRUE(asmFile.is_open());
  std::string content((std::istreambuf_iterator<char>(asmFile)), {});

  // Return label should be qualified off current function (Main)
  // We don't know the exact index, but pattern "Main$ret." must appear
  EXPECT_NE(content.find("@Main$ret."), std::string::npos);
  EXPECT_NE(content.find("(Main$ret."), std::string::npos);

  // Saved caller frame registers pushed
  EXPECT_NE(content.find("@LCL"),  std::string::npos);
  EXPECT_NE(content.find("@ARG"),  std::string::npos);
  EXPECT_NE(content.find("@THIS"), std::string::npos);
  EXPECT_NE(content.find("@THAT"), std::string::npos);

  // ARG = SP - 5 - nArgs (nArgs=2)
  // Look for the key arithmetic sequence
  EXPECT_NE(content.find("@SP\nD=M\n@5\nD=D-A\n@2\nD=D-A\n@ARG\nM=D\n"),
            std::string::npos);

  // LCL = SP
  EXPECT_NE(content.find("@SP\nD=M\n@LCL\nM=D\n"), std::string::npos);

  // Jump to callee
  EXPECT_NE(content.find("@Foo\n0;JMP\n"), std::string::npos);
}

/**
 * @brief Tests writeReturn emits correct epilogue to restore caller and jump back.
 */
TEST_F(CodeWriterTestObject, writeReturnRestoresFrameAndJumpsToRet) {
  ASSERT_TRUE(codeWriter);

  // Emit a bare return sequence; we only check the mechanics/patterns
  codeWriter->writeReturn();
  codeWriter->close();

  std::ifstream asmFile(asm_filepath);
  ASSERT_TRUE(asmFile.is_open());
  std::string content((std::istreambuf_iterator<char>(asmFile)), {});

  // FRAME = LCL -> R13
  EXPECT_NE(content.find("@LCL\nD=M\n@R13\nM=D\n"), std::string::npos);

  // RET = *(FRAME-5) -> R14
  EXPECT_NE(content.find("@5\nA=D-A\nD=M\n@R14\nM=D\n"), std::string::npos);

  // *ARG = pop(); SP = ARG + 1
  EXPECT_NE(content.find("@SP\nAM=M-1\nD=M\n@ARG\nA=M\nM=D\n"), std::string::npos);
  EXPECT_NE(content.find("@ARG\nD=M+1\n@SP\nM=D\n"), std::string::npos);

  // Restore THAT/THIS/ARG/LCL from FRAME (walking R13 down)
  EXPECT_NE(content.find("@R13\nAM=M-1\nD=M\n@THAT\nM=D\n"), std::string::npos);
  EXPECT_NE(content.find("@R13\nAM=M-1\nD=M\n@THIS\nM=D\n"), std::string::npos);
  EXPECT_NE(content.find("@R13\nAM=M-1\nD=M\n@ARG\nM=D\n"),  std::string::npos);
  EXPECT_NE(content.find("@R13\nAM=M-1\nD=M\n@LCL\nM=D\n"),  std::string::npos);

  // Jump to return address
  EXPECT_NE(content.find("@R14\nA=M\n0;JMP\n"), std::string::npos);
}

/**
 * @brief Tests that writeLabel/Goto/If work both outside and inside a function.
 *
 * Outside a function, labels must be unqualified; inside, qualified.
 */
TEST_F(CodeWriterTestObject, labelsAreUnqualifiedOutsideFunctionQualifiedInside) {
  ASSERT_TRUE(codeWriter);

  // Outside any function
  codeWriter->writeLabel("TOP");
  codeWriter->writeGoto("TOP");
  codeWriter->writeIf("TOP");

  // Inside a function
  codeWriter->writeFunction("Bar", 0);
  codeWriter->writeLabel("LOOP");
  codeWriter->writeGoto("LOOP");
  codeWriter->writeIf("END");

  codeWriter->close();

  std::ifstream asmFile(asm_filepath);
  ASSERT_TRUE(asmFile.is_open());
  std::string content((std::istreambuf_iterator<char>(asmFile)), {});

  // Outside: no qualification
  EXPECT_NE(content.find("(TOP)"), std::string::npos);
  EXPECT_NE(content.find("@TOP"),  std::string::npos);

  // Inside: qualified as Bar$X
  EXPECT_NE(content.find("(Bar$LOOP)"), std::string::npos);
  EXPECT_NE(content.find("@Bar$LOOP"),  std::string::npos);
  EXPECT_NE(content.find("@Bar$END"),   std::string::npos);
}
