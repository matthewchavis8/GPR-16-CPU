#include "parser.h"
#include <fstream>
#include <stdexcept>
#include "../Utils/CommandType.h"

Parser::Parser(std::ifstream& file)
  : m_file ( file )
{
  if (!m_file)
    throw std::runtime_error("[ERROR] File is not open\n");
  
  std::string lookAhead;
  
  m_file >> m_cmd;
  if (m_file >> lookAhead)
    m_lookaheadBuffer = std::move(lookAhead);
}

bool Parser::hasMoreLines() { return m_lookaheadBuffer.has_value(); }

void Parser::advance() { 
  if (!hasMoreLines())
    return;

  m_cmd = std::move(*m_lookaheadBuffer);

  std::string lookAhead;
  if (m_file >> lookAhead)
    m_lookaheadBuffer = std::move(lookAhead);
}

CommandType Parser::commandType() const {

  if (m_cmd == "push")
    return CommandType::C_PUSH;
  if (m_cmd == "pop")
    return CommandType::C_POP;
  if (m_cmd == "label")
    return CommandType::C_LABEL;
  if (m_cmd == "goto")
    return CommandType::C_GOTO;
  if (m_cmd == "if-goto")
    return CommandType::C_IF;
  if (m_cmd == "function")
    return CommandType::C_FUNCTION;
  if (m_cmd == "call")
    return CommandType::C_CALL;
  if (m_cmd == "return")
    return CommandType::C_RETURN;

  return CommandType::C_ARITHMETIC;
}

std::string Parser::arg1() const {
  if (CommandType::C_RETURN == commandType())
    throw std::runtime_error("[ERROR] Should not be called on C_RETURN\n");

  return m_cmd;
}

int Parser::arg2() const {
  if (commandType() != CommandType::C_PUSH || CommandType::C_POP || CommandType::C_FUNCTION || CommandType::C_CALL)
    throw std::runtime_error("[ERROR] Should not be called on C_PUSH, C_POP, C_FUNCTION, C_CALL");

  return stoi(m_cmd);
}
