/** @file
 *  @brief Tokenizer for Jack source files, producing a stream of lexical tokens.
 */

#pragma once

#include "../Utils/keyword.h"
#include "../Utils/tokenType.h"
#include <cstdint>
#include <fstream>
#include <string_view>
#include <optional>
#include <string>

/**
 * @brief Lexical analyzer that reads a Jack source file and exposes tokens
 *        one at a time with simple lookahead support.
 */
class Tokenizer {
  private:
    std::ifstream& m_file;
    std::string m_currentToken;
    std::optional<std::string> m_lookaheadBuff;
    
    /**
     * @brief Low-level scanner: read the next raw token from the input stream.
     *        Handles skipping whitespace and comments, and splits symbols
     *        from identifiers/integers according to the Jack specification.
     */
    std::optional<std::string> nextTokenFromStream();
    
    /** @brief Return true if the given text is a valid Jack integer constant. */
    bool isValidInteger(std::string_view token) const;

    /** @brief Return true if the given text is a valid Jack identifier name. */
    bool isValidIdentifier(std::string_view token) const;
    
    /** @brief Lookup table mapping keyword strings to `Keyword` enum values. */
    inline static constexpr std::pair<std::string_view, Keyword> table[] = {
      { "class",       Keyword::Class },
      { "constructor", Keyword::Constructor },
      { "function",    Keyword::Function },
      { "method",      Keyword::Method },
      { "field",       Keyword::Field },
      { "static",      Keyword::Static },
      { "var",         Keyword::Var },
      { "int",         Keyword::Int },
      { "char",        Keyword::Char },
      { "boolean",     Keyword::Boolean },
      { "void",        Keyword::Void },
      { "true",        Keyword::True },
      { "false",       Keyword::False },
      { "null",        Keyword::Null },
      { "this",        Keyword::This },
      { "let",         Keyword::Let },
      { "do",          Keyword::Do },
      { "if",          Keyword::If },
      { "else",        Keyword::Else },
      { "while",       Keyword::While },
      { "return",      Keyword::Return },
    };
  
  /** @brief Set of all single-character Jack symbol tokens. */
  inline static constexpr std::string_view kSymbols[] = {
    "{","}","(",")","[","]",".",",",";",
    "+","-","*","/","&","|","<",">","=","~"
  };
   
  /** @brief Check whether the given string matches a Jack symbol. */
  constexpr bool lookUpKeySymbols(std::string_view keySymbol) const {
    for (auto symbol : kSymbols) {
      if (symbol == keySymbol)
        return true;
    }
    return false;
  }
  

    /** @brief Look up the `Keyword` enum value for a given keyword string. */
    constexpr std::optional<Keyword> lookUpKeyWord(std::string_view keyword) const {
      for (const auto& [key, keywrd] : table) {
        if (key == keyword)
          return keywrd;
      }
      return std::nullopt;
  }

  public:
    /**
     * @brief Construct a tokenizer for the given input stream.
     * @param file Open input stream positioned at the start of a Jack file.
     */
    Tokenizer(std::ifstream& file);
    Tokenizer(const Tokenizer&) = delete;
    Tokenizer& operator=(const Tokenizer&) = delete;

    /** @brief Check whether more tokens are available in the input stream. */
    bool hasMoreTokens() const;

    /** @brief Advance to the next token in the input stream. */
    void advance();

    /** @brief Get the type of the current token. */
    Token tokenType() const;

    /** @brief Interpret the current token as a keyword (only valid for keyword tokens). */
    Keyword keyWord() const;

    /** @brief Interpret the current token as a symbol character. */
    char symbol() const;

    /** @brief Interpret the current token as an identifier string. */
    std::string identifier() const;

    /** @brief Interpret the current token as an integer constant value. */
    uint32_t intVal() const;

    /** @brief Interpret the current token as a string constant (without quotes). */
    std::string stringVal() const;

    /** @brief Get a view of the raw current token text. */
    std::string_view getCurrentToken() const;

    /** @brief Peek at the raw text of the next token without consuming it. */
    std::string_view getNextToken() const;

    /** @brief Close the underlying file stream when tokenization is complete. */
    void close();
};
