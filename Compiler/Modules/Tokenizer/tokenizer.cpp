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

static inline std::string_view trimTrailingSymbol(std::string_view s) {
  if (!s.empty() && isSymbolChar(s.back()))
    s.remove_suffix(1);
  return s;
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

  m_file >> m_currentToken;
  std::string nextToken;
  if (m_file >> nextToken)
    m_lookaheadBuff = std::move(nextToken);
}

bool Tokenizer::hasMoreTokens() const { return m_lookaheadBuff.has_value(); }

void Tokenizer::advance() {
  if (!hasMoreTokens())
    throw std::runtime_error("[ERROR] There are no more tokens\n");

  m_currentToken = std::move(*m_lookaheadBuff);

  std::string nextToken;
  if (m_file >> nextToken)
    m_lookaheadBuff = std::move(nextToken);
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

bool Tokenizer::isValidSequence(std::string_view token) const {
  for (unsigned char c : token) {
    if (c == '\n' || c == '\r' || c == '\'' || c == '\"')
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

  if (isValidIdentifier(m_currentToken))
    return Token::Identifier;
  
  std::string_view core = trimTrailingSymbol(m_currentToken);
  if (isValidInteger(core))
    return Token::IntConst;

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

  std::string core { trimTrailingSymbol(m_currentToken) };

  return std::stoi(core);
}

std::string Tokenizer::stringVal() const {
  if (tokenType() != Token::StringConst)
    throw std::runtime_error("[ERROR] stringVal() should only be called on tokenType is stringConst\n");

  return m_currentToken;
}

std::string Tokenizer::getCurrentToken() const { return m_currentToken; }

void Tokenizer::close() { m_file.close(); }
