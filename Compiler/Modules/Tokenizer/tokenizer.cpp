#include "../Utils/keyword.h"
#include "../Utils/tokenType.h"
#include "tokenizer.h"
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <cstring>

static inline bool isSymbolChar(char c) {
  static constexpr const char* kSyms = "{}()[].,;+-*/&|<>=~";
  return std::strchr(kSyms, c) != nullptr;
}

Tokenizer::Tokenizer(std::ifstream& file, const std::string& fileName)
  : m_file { file }
  , m_fileName { fileName }
{
  if (!file.is_open())
    throw std::logic_error("[ERROR] Unable to open file\n");
  
  std::size_t fileNameLength = fileName.length();
  if (m_fileName.substr(fileNameLength - 5) != ".jack")
    throw std::logic_error("[ERROR] unable to open the jack file\n");

  auto firstToken = nextTokenFromStream();
  if (!firstToken)
    throw std::logic_error("[ERROR] empty Jack source file\n");

  m_currentToken = std::move(*firstToken);

  if (auto secondToken = nextTokenFromStream())
    m_lookaheadBuff = std::move(*secondToken);
}

bool Tokenizer::hasMoreTokens() const { return m_lookaheadBuff.has_value(); }

void Tokenizer::advance() {
  if (!hasMoreTokens())
    throw std::runtime_error("[ERROR] There are no more tokens\n");

  m_currentToken = std::move(*m_lookaheadBuff);

  if (auto nextToken = nextTokenFromStream())
    m_lookaheadBuff = std::move(*nextToken);
  else
    m_lookaheadBuff.reset();
}

bool Tokenizer::isValidInteger(std::string_view token) const {
  if (token.empty())
    return false;

  for (unsigned char c : token) {
    if (c < '0' || c > '9')
      return false;
  }
  return true;
}

bool Tokenizer::isValidIdentifier(const std::string_view token) const {
  if (token.empty())
    return false;

  unsigned char c0 { static_cast<unsigned char>(token[0]) };

  if (!(std::isalpha(c0) || c0 == '_'))
    return false;

  for (std::size_t i{1}; i < token.length(); ++i) {
    unsigned char ch { static_cast<unsigned char>(token[i]) };
    if (!std::isalnum(ch) && ch != '_')
      return false;
  }
  return true;
}

Token Tokenizer::tokenType() const {
  if (lookUpKeyWord(m_currentToken))
    return Token::Keyword;

  if (lookUpKeySymbols(m_currentToken))
    return Token::Symbol;

  if (isValidInteger(m_currentToken))
    return Token::IntConst;

  if (isValidIdentifier(m_currentToken))
    return Token::Identifier;

  return Token::StringConst;
}

Keyword Tokenizer::keyWord() const {
  auto keyword { lookUpKeyWord(m_currentToken) };

  if (!keyword.has_value())
    throw std::runtime_error("[ERROR] keyword() called on non-keyword token\n");

  return *keyword;
}

char Tokenizer::symbol() const {
  if (tokenType() != Token::Symbol)
    throw std::runtime_error("[ERROR] symbol() should only be called on tokenType is Symbol\n");

  return m_currentToken[0];
}

std::string Tokenizer::identifier() const {
  if (tokenType() != Token::Identifier)
    throw std::runtime_error("[ERROR] identifier() should only be called on tokenType is identifier\n");

  return m_currentToken;
}


uint32_t Tokenizer::intVal() const {
  if (tokenType() != Token::IntConst)
    throw std::runtime_error("[ERROR] intVal() should only be called on tokenType is IntConst\n");

  return std::stoi(m_currentToken);
}

std::string Tokenizer::stringVal() const {
  if (tokenType() != Token::StringConst)
    throw std::runtime_error("[ERROR] stringVal() should only be called on tokenType is stringConst\n");

  return m_currentToken;
}

std::string_view Tokenizer::getCurrentToken() const { return m_currentToken; }

std::string_view Tokenizer::getNextToken() const { 
  if (!m_lookaheadBuff.has_value())
    throw std::runtime_error("[ERROR] There is no next token\n");

  return *m_lookaheadBuff;
}

void Tokenizer::close() { m_file.close(); }

std::optional<std::string> Tokenizer::nextTokenFromStream() {
  char c;

  while (true) {
    // Skip whitespace
    while (m_file.get(c)) {
      if (!std::isspace(static_cast<unsigned char>(c)))
        break;
    }

    if (!m_file)
      return std::nullopt;

    // Handle comments or '/' symbol
    if (c == '/') {
      char d;
      if (!m_file.get(d)) {
        // Just '/' at EOF – treat as symbol
        return std::string(1, c);
      }

      if (d == '/') {
        // Single-line comment: skip until end of line
        while (m_file.get(c)) {
          if (c == '\n')
            break;
        }
        if (!m_file)
          return std::nullopt;
        continue; // restart scanning for next token
      } else if (d == '*') {
        // Block comment: skip until "*/"
        char prev = 0;
        while (m_file.get(c)) {
          if (prev == '*' && c == '/')
            break;
          prev = c;
        }
        if (!m_file)
          return std::nullopt;
        continue; // restart scanning
      } else {
        // Not a comment: put back the second character and return '/' as symbol
        m_file.unget();
        return std::string(1, c);
      }
    }

    // String constant: everything between double quotes (without quotes)
    if (c == '"') {
      std::string s;
      while (m_file.get(c)) {
        if (c == '"')
          break;
        s.push_back(c);
      }
      // If EOF is reached without closing quote, we still return what we have.
      return s;
    }

    // Single-character symbol
    if (isSymbolChar(c)) {
      return std::string(1, c);
    }

    // Identifier or integer: accumulate until whitespace, symbol, quote, or '/'
    std::string token;
    token.push_back(c);

    while (m_file.get(c)) {
      if (std::isspace(static_cast<unsigned char>(c)) || isSymbolChar(c) || c == '"' || c == '/') {
        // Put back the delimiter so it will be processed on the next call
        m_file.unget();
        break;
      }
      token.push_back(c);
    }

    return token;
  }
}
