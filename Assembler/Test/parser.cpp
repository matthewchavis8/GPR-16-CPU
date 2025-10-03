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
   }

    filestream.open(filepath, std::ios::in);

    parser = std::make_unique<Parser>(filestream, filepath.string());

   }

  void TearDown() override {
    filestream.close();
    std::filesystem::remove(filepath);
  }
};

TEST_F(ParserTestObject, CanReadFirstToken) {
  ASSERT_TRUE(parser);

  std::string_view expected { "The" };
  std::string_view command { parser->getCommand() };

  ASSERT_EQ(command, expected);
}

TEST_F(ParserTestObject, canReadLookAheadBuffer) {
  ASSERT_TRUE(parser);

  std::string_view expected { "Cat" };
  std::string_view buffer { parser->getLookAheadBuffer() };

  std::cout << "[LOG] The Token in the Lookaheadbuffer is: " << parser->getLookAheadBuffer() << '\n';

  ASSERT_EQ(buffer, expected);
}
