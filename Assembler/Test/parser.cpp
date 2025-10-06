#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <string_view>
#include "../Modules/Parser/parser.h"

using namespace testing;

class ParserTestObject : public ::testing::Test {
  protected: 
  std::filesystem::path filepath;
  std::ifstream filestream;
  std::unique_ptr<Parser> parser;

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

  void TearDown() override {
    filestream.close();
    std::filesystem::remove(filepath);
  }
};

TEST_F(ParserTestObject, canReadFirstToken) {
  ASSERT_TRUE(parser);

  std::string_view expected { "@2" };
  std::string_view command { parser->getCommand() };

  ASSERT_EQ(command, expected);
}

TEST_F(ParserTestObject, canReadLookAheadBuffer) {
  ASSERT_TRUE(parser);

  std::string_view expected { "D=A" };
  std::string_view buffer { parser->getLookAheadBuffer() };

  ASSERT_EQ(buffer, expected);
}

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

TEST_F(ParserTestObject, canReadSymbol) {
  ASSERT_TRUE(parser);
  ASSERT_TRUE(parser->hasMoreCommands());

  std::string_view expectedMnemo { "2" };
  std::string mnemo { parser->symbol() };

  ASSERT_EQ(expectedMnemo, mnemo);
}

TEST_F(ParserTestObject, canReadDest) {
  ASSERT_TRUE(parser);
  ASSERT_TRUE(parser->hasMoreCommands());

  while (parser->commandType() != CommandType::C_COMMAND)
    parser->advance();

  std::string_view expectedMnemo { "D" };
  std::string mnemo { parser->dest() };

  ASSERT_EQ(expectedMnemo, mnemo);
}

TEST_F(ParserTestObject, canReadComp) {
  ASSERT_TRUE(parser);
  ASSERT_TRUE(parser->hasMoreCommands());

  while (parser->commandType() != CommandType::C_COMMAND)
    parser->advance();

  std::string_view expectedMnemo { "A" };
  std::string mnemo { parser->comp() };

  ASSERT_EQ(expectedMnemo, mnemo);
}

TEST_F(ParserTestObject, canReadJmp) {
  ASSERT_TRUE(parser);
  ASSERT_TRUE(parser->hasMoreCommands());

  while (parser->hasMoreCommands())
    parser->advance();

  std::string_view expectedMnemo { "JMP" };
  std::string mnemo { parser->jump() };

  ASSERT_EQ(expectedMnemo, mnemo);
}
