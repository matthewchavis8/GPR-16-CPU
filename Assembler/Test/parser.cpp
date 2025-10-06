#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <string_view>
#include "../Modules/Parser/parser.h"

using namespace testing;

/**
 * @class ParserTestObject
 * @brief Test fixture for Parser class unit tests.
 *
 * Creates a temporary assembly file with representative commands and 
 * initializes a Parser instance for testing command parsing operations.
 */
class ParserTestObject : public ::testing::Test {
  protected:
    // @brief Path to the temporary assembly file used for testing
    std::filesystem::path filepath;
    
    // @brief Input file stream for reading the test assembly file.
    std::ifstream filestream;
    
    // @brief Parser instance under test.
    std::unique_ptr<Parser> parser;

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

/**
 * @brief Verifies that the parser correctly loads the first command on initialization.
 * @test Ensures getCommand() returns the first token from the input file.
 */
TEST_F(ParserTestObject, canReadFirstToken) {
  ASSERT_TRUE(parser);

  std::string_view expected { "@2" };
  std::string_view command { parser->getCommand() };

  ASSERT_EQ(command, expected);
}

/**
 * @brief Verifies that the lookahead buffer contains the second command after initialization.
 * @test Ensures getLookAheadBuffer() returns the next token queued for processing.
 */
TEST_F(ParserTestObject, canReadLookAheadBuffer) {
  ASSERT_TRUE(parser);

  std::string_view expected { "D=A" };
  std::string_view buffer { parser->getLookAheadBuffer() };

  ASSERT_EQ(buffer, expected);
}

/**
 * @brief Verifies that advance() correctly moves through multiple commands.
 * @test Confirms that both current command and lookahead buffer update properly
 *       after each call to advance(), validating sequential command processing.
 */
TEST_F(ParserTestObject, canAdvance) {
  ASSERT_TRUE(parser);
  ASSERT_TRUE(parser->hasMoreCommands());

  parser->advance();

  std::string_view expected_command { "D=A" };
  std::string_view command_1 { parser->getCommand() };

  std::string_view expected_buffer { "D=D+A" };
  std::string_view buffer_1 { parser->getLookAheadBuffer() };

  ASSERT_EQ(expected_command, command_1);
  ASSERT_EQ(expected_buffer, buffer_1);

  parser->advance();
  
  std::string_view expected_next_command { "D=D+A" };
  std::string_view command_2 { parser->getCommand() };

  std::string_view expected_next_buffer { "@0" };
  std::string_view buffer_2 { parser->getLookAheadBuffer() };

  ASSERT_EQ(expected_next_command, command_2);
  ASSERT_EQ(expected_next_buffer, buffer_2);
}

/**
 * @brief Verifies behavior when reaching the end of the input file.
 * @test Advances through all commands and confirms that hasMoreCommands() returns false
 *       and the lookahead buffer is empty when no more tokens remain.
 */
TEST_F(ParserTestObject, canHandleNoMoreCommands) {
  ASSERT_TRUE(parser);
  ASSERT_TRUE(parser->hasMoreCommands());

  parser->advance();
  parser->advance();
  parser->advance();
  parser->advance();
  parser->advance();
  parser->advance();
  parser->advance();
  
  std::string_view expected_command1 { "0;JMP" };
  std::string_view command_1 { parser->getCommand() };

  std::string_view expected_buffer1 { "" };
  std::string_view buffer_1 { parser->getLookAheadBuffer() };

  ASSERT_EQ(expected_command1, command_1);
  ASSERT_EQ(expected_buffer1, buffer_1);
}

/**
 * @brief Verifies extraction of symbols from A-commands.
 * @test Confirms that symbol() correctly strips the '@' prefix and returns
 *       the numeric or symbolic value from an A-command.
 */
TEST_F(ParserTestObject, canReadSymbol) {
  ASSERT_TRUE(parser);
  ASSERT_TRUE(parser->hasMoreCommands());

  std::string_view expectedMnemo { "2" };
  std::string mnemo { parser->symbol() };

  ASSERT_EQ(expectedMnemo, mnemo);
}

/**
 * @brief Verifies extraction of destination mnemonics from C-commands.
 * @test Advances to a C-command and confirms that dest() correctly returns
 *       the destination register(s) specified before the '=' operator.
 */
TEST_F(ParserTestObject, canReadDest) {
  ASSERT_TRUE(parser);
  ASSERT_TRUE(parser->hasMoreCommands());

  while (parser->commandType() != CommandType::C_COMMAND)
    parser->advance();

  std::string_view expectedMnemo { "D" };
  std::string mnemo { parser->dest() };

  ASSERT_EQ(expectedMnemo, mnemo);
}

/**
 * @brief Verifies extraction of computation mnemonics from C-commands.
 * @test Advances to a C-command and confirms that comp() correctly returns
 *       the computation expression between '=' and ';' operators.
 */
TEST_F(ParserTestObject, canReadComp) {
  ASSERT_TRUE(parser);
  ASSERT_TRUE(parser->hasMoreCommands());

  while (parser->commandType() != CommandType::C_COMMAND)
    parser->advance();

  std::string_view expectedMnemo { "A" };
  std::string mnemo { parser->comp() };

  ASSERT_EQ(expectedMnemo, mnemo);
}

/**
 * @brief Verifies extraction of jump mnemonics from C-commands.
 * @test Advances through all commands to the last C-command containing a jump
 *       instruction and confirms that jump() correctly returns the mnemonic after ';'.
 */
TEST_F(ParserTestObject, canReadJmp) {
  ASSERT_TRUE(parser);
  ASSERT_TRUE(parser->hasMoreCommands());

  while (parser->hasMoreCommands())
    parser->advance();

  std::string_view expectedMnemo { "JMP" };
  std::string mnemo { parser->jump() };

  ASSERT_EQ(expectedMnemo, mnemo);
}
