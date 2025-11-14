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
    std::string m_fileName;
    
    bool isValidInteger(std::string_view token) const;
    bool isValidSequence(std::string_view token) const;
    bool isValidIdentifier(std::string_view token) const;
    
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
  
  inline static constexpr std::string_view kSymbols[] = {
    "{","}","(",")","[","]",".",",",";",
    "+","-","*","/","&","|","<",">","=","~"
  };
   
  constexpr bool lookUpKeySymbols(std::string_view keySymbol) const {
    for (auto symbol : kSymbols) {
      if (symbol == keySymbol)
        return true;
    }
    return false;
  }
  

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
     * @param file      Open input stream positioned at the start of a Jack file.
     * @param fileName  Human-readable name of the file for diagnostics.
     */
    Tokenizer(std::ifstream& file, const std::string& fileName);

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
