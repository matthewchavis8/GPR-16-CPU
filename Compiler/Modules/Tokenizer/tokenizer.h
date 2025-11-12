#pragma once

#include "../Utils/keyword.h"
#include "../Utils/tokenType.h"
#include <cstdint>
#include <fstream>
#include <string_view>
#include <optional>
#include <string>

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
    Tokenizer(std::ifstream& file, const std::string& fileName);

    Tokenizer(const Tokenizer&) = delete;
    Tokenizer& operator=(const Tokenizer&) = delete;

    bool hasMoreTokens() const;

    void advance();

    Token tokenType() const;

    Keyword keyWord() const;

    char symbol() const;

    std::string identifier() const;

    uint32_t intVal() const;

    std::string stringVal() const;

    std::string_view getCurrentToken() const;
    std::string_view getNextToken() const;

    void close();
};
