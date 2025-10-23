/**
 * @file parser.cpp
 * @brief Unit tests for the Parser interface and VM command parsing.
 */
#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include "../Modules/Parser/parser.h"

using namespace testing;

/**
 * @class ParserTestObject
 * @brief Test fixture for Parser class unit tests.
 *
 * Creates a temporary VM file with representative commands and
 * initializes a Parser instance for testing VM command parsing operations.
 */
class ParserTestObject : public ::testing::Test {
  protected:
    /** @brief Path to the temporary VM file used for testing. */
    std::filesystem::path filepath;

    /** @brief Input file stream for reading the test VM file. */
    std::ifstream filestream;

    /** @brief Parser instance under test. */
    std::unique_ptr<Parser> parser;

    /**
     * @brief Initializes test environment before each test case.
     *
     * Creates a temporary `.vm` file with sample VM commands including
     * push, pop, arithmetic, and branching operations. Opens the file stream and
     * constructs the Parser instance.
     */
    void SetUp() override {
    // Setting up temporary directory
    filepath = std::filesystem::temp_directory_path() / "tmp.vm";

   // Setting up file path with text
   {
    std::ofstream file(filepath);
    file << "push constant 2" << '\n';
    file << "push constant 2" << '\n';
    file << "add" << '\n';
    file << "pop local 0" << '\n';
    file << "label LOOP" << '\n';
    file << "goto LOOP" << '\n';
   }

    filestream.open(filepath, std::ios::in);

    parser = std::make_unique<Parser>(filestream);
   }

    /**
     * @brief Cleans up test environment after each test case.
     *
     * Closes the file stream and removes the temporary VM file.
     */
    void TearDown() override {
      filestream.close();
      std::filesystem::remove(filepath);
    }
};

/**
 * @brief Verifies that the Parser can be successfully constructed.
 */
TEST_F(ParserTestObject, canInitializeParser) {
    ASSERT_TRUE(parser);
}

/**
 * @brief Tests advancing through all commands and verifying command types.
 *
 * Validates that the parser correctly identifies all six command types
 * (push, arithmetic, pop, label, goto) in sequence.
 */
TEST_F(ParserTestObject, canAdvanceThroughFile) {
    std::vector<std::string> expectedCommands = {
        "push", "push", "add", "pop", "label", "goto"
    };
    std::vector<CommandType> expectedTypes = {
        CommandType::C_PUSH,
        CommandType::C_PUSH,
        CommandType::C_ARITHMETIC,
        CommandType::C_POP,
        CommandType::C_LABEL,
        CommandType::C_GOTO
    };

    size_t idx { 0 };
    while (parser->hasMoreLines()) {
        parser->advance();
        ASSERT_EQ(parser->commandType(), expectedTypes[idx])
            << "Mismatch at line " << idx;
        ++idx;
    }

    ASSERT_EQ(idx, expectedCommands.size());
}

/**
 * @brief Tests that commandType() returns the correct type for each command.
 *
 * Steps through each command in the test file and validates the returned
 * CommandType matches the expected type.
 */
TEST_F(ParserTestObject, canReturnCorrectCommandType) {
    ASSERT_TRUE(parser->hasMoreLines());
    parser->advance();
    EXPECT_EQ(parser->commandType(), CommandType::C_PUSH);

    parser->hasMoreLines(); parser->advance();
    EXPECT_EQ(parser->commandType(), CommandType::C_PUSH);

    parser->hasMoreLines(); parser->advance();
    EXPECT_EQ(parser->commandType(), CommandType::C_ARITHMETIC);

    parser->hasMoreLines(); parser->advance();
    EXPECT_EQ(parser->commandType(), CommandType::C_POP);

    parser->hasMoreLines(); parser->advance();
    EXPECT_EQ(parser->commandType(), CommandType::C_LABEL);

    parser->hasMoreLines(); parser->advance();
    EXPECT_EQ(parser->commandType(), CommandType::C_GOTO);
}

/**
 * @brief Tests parsing the entire VM file and extracting all arguments.
 *
 * Validates that arg1() and arg2() return valid values for all commands.
 * Ensures push/pop commands have both arguments, and other commands have arg1.
 */
TEST_F(ParserTestObject, canReadEntireVMfile) {
    int lineCount { 0 };
    while (parser->hasMoreLines()) {
        parser->advance();
        CommandType type = parser->commandType();
        if (type == CommandType::C_PUSH || type == CommandType::C_POP) {
            ASSERT_NO_THROW({
                std::string a1 { parser->arg1() };
                int a2 = { parser->arg2() };
                ASSERT_FALSE(a1.empty());
                ASSERT_GE(a2, 0);
            });
        } else {
            ASSERT_NO_THROW({
                std::string a1 { parser->arg1() };
                ASSERT_FALSE(a1.empty());
            });
        }
        ++lineCount;
    }
    ASSERT_EQ(lineCount, 6);
}

/**
 * @brief Tests that arg2() throws logic_error for arithmetic commands.
 *
 * Verifies proper error handling when arg2() is called on a command type
 * that does not support a second argument (C_ARITHMETIC).
 */
TEST_F(ParserTestObject, throwsOnInvalidArg2Call) {
    parser->hasMoreLines(); 
    parser->advance();
    parser->hasMoreLines();
    parser->advance();
    parser->hasMoreLines();
    parser->advance();
    EXPECT_THROW(parser->arg2(), std::logic_error);
}
