
/** @file
 *  @brief GoogleTest harness for the `Tokenizer` component.
 */

#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include "../Modules/Tokenizer/tokenizer.h"

using namespace testing;

/**
 * @brief Test fixture that creates a temporary Jack file and tokenizer
 *        instance used across multiple tokenizer tests.
 */
class TokenizerTestObject : public ::testing::Test {
  protected:
    std::filesystem::path filepath;
    std::ifstream filestream;
    std::unique_ptr<Tokenizer>tokenizer;

    void SetUp() override {
      filepath = std::filesystem::temp_directory_path() / "tmp.jack";
      
      {
        std::ofstream file(filepath);
        file << "/** Tokenizer Torture Test: covers keywords, symbols, ints, strings, comments. */" << '\n';
        file << "// Symbols to see (ignored here because in comment): {}()[] . , ; + - * / & | < > = ~" << '\n';
        file << "/* Multi-line comment:" << '\n';
        file << "   Make sure the tokenizer skips all of this, even with symbols like [ ] { } ( ) . , ; + - * / & | < > = ~" << '\n';
        file << "*/" << '\n';
        file << "" << '\n';
        file << "class TokenZoo {" << '\n';
        file << "    static boolean FLAG;               // keyword 'static', type 'boolean', identifier, symbol ';'" << '\n';
        file << "    field int count, i;                // multiple field decls" << '\n';
        file << "    field Array arr;                   // user-defined type" << '\n';
        file << "" << '\n';
        file << "    constructor TokenZoo new(int size) {" << '\n';
        file << "        var int j;                     // 'var' decl" << '\n';
        file << "        let count = 0;                 // intConstant 0" << '\n';
        file << "        let i = -1;                    // unary '-'" << '\n';
        file << "        let arr = Array.new(size);     // subroutine call with dot" << '\n';
        file << "        return this;                   // 'this' keyword" << '\n';
        file << "    }" << '\n';
        file << "" << '\n';
        file << "    method void set(int index, int value) {" << '\n';
        file << "        // array write with arithmetic and symbols" << '\n';
        file << "        let arr[index] = value + 1;" << '\n';
        file << "        return;" << '\n';
        file << "    }" << '\n';
        file << "" << '\n';
        file << "    function int sum() {" << '\n';
        file << "        var int s;" << '\n';
        file << "        let s = 0;" << '\n';
        file << "        let i = 0;" << '\n';
        file << "        while (i < arr.length()) {     // 'while', relational '<', dot call, parens/braces" << '\n';
        file << "            let s = s + arr[i];" << '\n';
        file << "            let i = i + 1;" << '\n';
        file << "        }" << '\n';
        file << "        return s;" << '\n';
        file << "    }" << '\n';
        file << "" << '\n';
        file << "    method void demo() {" << '\n';
        file << "        var int a, b;" << '\n';
        file << "        var boolean ok;" << '\n';
        file << "        var char c;" << '\n';
        file << "" << '\n';
        file << "        let a = 32767;                 // max Jack int" << '\n';
        file << "        let b = 0;" << '\n';
        file << "        let ok = true;                 // 'true' keyword" << '\n';
        file << "        let c = 65;                    // 'A' as int (Jack char is int under the hood)" << '\n';
        file << "        let this.count = a;            // field via 'this'" << '\n';
        file << "        let TokenZoo.FLAG = false;     // static via class name" << '\n';
        file << "" << '\n';
        file << "        // if/else with unary '~', bitwise '|', and grouping" << '\n';
        file << "        if (~ok | (a > b)) {" << '\n';
        file << "            do Output.printString(\"hello, world <>&\");" << '\n';
        file << "            do Output.println();" << '\n';
        file << "        } else {" << '\n';
        file << "            do Output.printInt(a * (b + 2));" << '\n';
        file << "            do Output.println();" << '\n';
        file << "        }" << '\n';
        file << "" << '\n';
        file << "        // Use 'null' and array indexing read" << '\n';
        file << "        if (arr = null) {" << '\n';
        file << "            let arr = Array.new(3);" << '\n';
        file << "            let arr[0] = 10;" << '\n';
        file << "            let arr[1] = 20;" << '\n';
        file << "            let arr[2] = 30;" << '\n';
        file << "        }" << '\n';
        file << "" << '\n';
        file << "        // String with spaces and punctuation (no embedded quotes)" << '\n';
        file << "        do Output.printString(\"Tokenizer test: symbols .,;+-*/&|<>=~ and brackets []{}()\");" << '\n';
        file << "        do Output.println();" << '\n';
        file << "" << '\n';
        file << "        // Call some OS functions to create more tokens" << '\n';
        file << "        do Output.printChar(c);        // should print 'A'" << '\n';
        file << "        do Output.println();" << '\n';
        file << "        do Output.printInt(Math.multiply(2, 3));" << '\n';
        file << "        do Output.println();" << '\n';
        file << "" << '\n';
        file << "        return;" << '\n';
        file << "    }" << '\n';
        file << "}" << '\n';
        file << "" << '\n';
        file << "// trailing single-line comment with symbols: {}()[].,;+-*/&|<>=~ and spaces" << '\n';
      }
      filestream.open(filepath, std::ios::in);

      tokenizer = std::make_unique<Tokenizer>(filestream, filepath.string());
    }

    void TearDown() override {
      tokenizer->close();
      std::filesystem::remove(filepath);
    }
};

/** @test
 *  @brief Asserts that the tokenizer can reconstruct the first comment line
 *         when tokens are concatenated with spaces.
 */
TEST_F(TokenizerTestObject, canReadFirstLine) {
  ASSERT_TRUE(tokenizer);

  std::string_view expected
    { "/** Tokenizer Torture Test: covers keywords, symbols, ints, strings, comments. */" };

  std::string cmds;
  for (int i{}; i < 11; ++i) {
    std::string token { tokenizer->getCurrentToken() };
    if (i != 0)
      cmds.append(" ");
    cmds += token;
    tokenizer->advance();
  }
  ASSERT_EQ(expected, cmds);
}

/** @test
 *  @brief Verifies that a `class` keyword token is followed by an identifier
 *         token for the class name.
 */
TEST_F(TokenizerTestObject, Tokenizer_Keyword_ClassThenIdentifier) {
  ASSERT_TRUE(tokenizer);

  int steps = 0;
  while (tokenizer->getCurrentToken() != "class" && tokenizer->hasMoreTokens() && steps++ < 5000)
    tokenizer->advance();

  ASSERT_EQ(tokenizer->getCurrentToken(), "class");
  EXPECT_EQ(tokenizer->tokenType(), Token::Keyword);
  EXPECT_EQ(tokenizer->keyWord(), Keyword::Class);

  tokenizer->advance(); // expect class name next
  EXPECT_EQ(tokenizer->tokenType(), Token::Identifier);
  EXPECT_EQ(tokenizer->identifier(), "TokenZoo");
}

/** @test
 *  @brief Finds the `{` symbol following the class name and checks symbol access.
 */
TEST_F(TokenizerTestObject, Tokenizer_Symbol_OpenBraceAfterClassName) {
  ASSERT_TRUE(tokenizer);

  int steps = 0;
  while (tokenizer->getCurrentToken() != "{" && tokenizer->hasMoreTokens() && steps++ < 8000)
    tokenizer->advance();

  ASSERT_EQ(tokenizer->getCurrentToken(), "{");
  EXPECT_EQ(tokenizer->tokenType(), Token::Symbol);
  EXPECT_EQ(tokenizer->symbol(), '{');
}

/** @test
 *  @brief Locates the `32767` literal and validates that it is reported as an
 *         `IntConst` with the correct value.
 */
TEST_F(TokenizerTestObject, Tokenizer_IntConst_Max32767) {
  ASSERT_TRUE(tokenizer);

  int safety = 200000;
  while (safety-- > 0) {
    auto tt = tokenizer->tokenType();
    if (tt == Token::IntConst && tokenizer->intVal() == 32767u)
      break;

    if (!tokenizer->hasMoreTokens())
      FAIL() << "32767 not found in input";
    tokenizer->advance();
  }

  // If we got here, current token is the 32767 int constant.
  EXPECT_EQ(tokenizer->tokenType(), Token::IntConst);
  EXPECT_EQ(tokenizer->intVal(), 32767u);
}

/** @test
 *  @brief Confirms that calling the wrong accessor for a token type throws
 *         `std::runtime_error`, while the correct accessor succeeds.
 */
TEST_F(TokenizerTestObject, Tokenizer_Accessors_ThrowOnWrongType) {
  ASSERT_TRUE(tokenizer);

  // Find a '+' symbol
  int steps = 0;
  while (tokenizer->getCurrentToken() != "+" && tokenizer->hasMoreTokens() && steps++ < 20000)
    tokenizer->advance();

  ASSERT_EQ(tokenizer->getCurrentToken(), "+");
  EXPECT_EQ(tokenizer->tokenType(), Token::Symbol);

  EXPECT_NO_THROW((void)tokenizer->symbol());
  EXPECT_THROW(tokenizer->intVal(), std::runtime_error);
  EXPECT_THROW(tokenizer->identifier(), std::runtime_error);
  EXPECT_THROW(tokenizer->stringVal(), std::runtime_error);
}

/** @test
 *  @brief Advances to EOF and verifies that `hasMoreTokens()` becomes false and
 *         a further `advance()` call throws.
 */
TEST_F(TokenizerTestObject, Tokenizer_Advance_EOF_Throws) {
  ASSERT_TRUE(tokenizer);

  int safety = 100000;
  while (tokenizer->hasMoreTokens() && safety-- > 0)
    tokenizer->advance();

  EXPECT_FALSE(tokenizer->hasMoreTokens());
  EXPECT_THROW(tokenizer->advance(), std::runtime_error);
}

/** @test
 *  @brief Ensures that `keyWord()` throws when invoked on a non-keyword token.
 */
TEST_F(TokenizerTestObject, Tokenizer_KeyWord_ThrowsOnNonKeyword) {
  ASSERT_TRUE(tokenizer);

  // Move to "class", then one step to "TokenZoo" (identifier)
  int steps = 0;
  while (tokenizer->getCurrentToken() != "class" && tokenizer->hasMoreTokens() && steps++ < 5000)
    tokenizer->advance();
  ASSERT_EQ(tokenizer->getCurrentToken(), "class");
  tokenizer->advance();

  EXPECT_EQ(tokenizer->tokenType(), Token::Identifier);
  EXPECT_THROW(tokenizer->keyWord(), std::runtime_error);
}

/** @test
 *  @brief Sanity check that `getCurrentToken()` reflects changes after
 *         `advance()`.
 */
TEST_F(TokenizerTestObject, Tokenizer_GetCurrentToken_EchoChangesAfterAdvance) {
  ASSERT_TRUE(tokenizer);

  auto first = tokenizer->getCurrentToken();
  EXPECT_FALSE(first.empty());

  if (tokenizer->hasMoreTokens()) {
    tokenizer->advance();
    auto second = tokenizer->getCurrentToken();
    EXPECT_FALSE(second.empty());
    EXPECT_NE(first, second);
  }
}

/** @test
 *  @brief Verifies that `stringVal()` throws when called on a non-string token.
 */
TEST_F(TokenizerTestObject, Tokenizer_StringVal_ThrowsWhenNotString) {
  ASSERT_TRUE(tokenizer);

  // Use a known non-string token (e.g., '+')
  int steps = 0;
  while (tokenizer->getCurrentToken() != "+" && tokenizer->hasMoreTokens() && steps++ < 20000)
    tokenizer->advance();

  ASSERT_EQ(tokenizer->getCurrentToken(), "+");
  EXPECT_THROW(tokenizer->stringVal(), std::runtime_error);

  // NOTE: add a positive stringVal test later when your tokenizer lexes quoted strings as one token.
}

