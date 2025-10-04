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
    file << "The Cat is Black" << '\n';
    file << "But the Dog is White" << '\n';
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

  std::string_view expected { "The" };
  std::string_view command { parser->getCommand() };

  ASSERT_EQ(command, expected);
}

TEST_F(ParserTestObject, canReadLookAheadBuffer) {
  ASSERT_TRUE(parser);

  std::string_view expected { "Cat" };
  std::string_view buffer { parser->getLookAheadBuffer() };

  ASSERT_EQ(buffer, expected);
}

TEST_F(ParserTestObject, canAdvance) {
  ASSERT_TRUE(parser);
  ASSERT_TRUE(parser->hasMoreCommands());

  parser->advance();

  std::string_view expected_command { "Cat" };
  std::string_view command_1 { parser->getCommand() };

  std::string_view expected_buffer { "is" };
  std::string_view buffer_1 { parser->getLookAheadBuffer() };

  ASSERT_EQ(expected_command, command_1);
  ASSERT_EQ(expected_buffer, buffer_1);

  parser->advance();
  
  std::string_view expected_next_command { "is" };
  std::string_view command_2 { parser->getCommand() };

  std::string_view expected_next_buffer { "Black" };
  std::string_view buffer_2 { parser->getLookAheadBuffer() };

  ASSERT_EQ(expected_next_command, command_2);
  ASSERT_EQ(expected_next_buffer, buffer_2);
}

TEST_F(ParserTestObject, canAdvanceToNextLine) {
  ASSERT_TRUE(parser);
  ASSERT_TRUE(parser->hasMoreCommands());

  parser->advance();
  parser->advance();
  parser->advance();
  parser->advance();
  
  std::string_view expected_command1 { "But" };
  std::string_view command_1 { parser->getCommand() };

  std::string_view expected_buffer1 { "the" };
  std::string_view buffer_1 { parser->getLookAheadBuffer() };

  ASSERT_EQ(expected_command1, command_1);
  ASSERT_EQ(expected_buffer1, buffer_1);
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
  parser->advance();
  
  std::string_view expected_command1 { "White" };
  std::string_view command_1 { parser->getCommand() };

  std::string_view expected_buffer1 { "" };
  std::string_view buffer_1 { parser->getLookAheadBuffer() };

  ASSERT_EQ(expected_command1, command_1);
  ASSERT_EQ(expected_buffer1, buffer_1);
}
