#include "parser.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>
#include "../Utils/CommandType.h"

Parser::Parser(std::ifstream& file)
  : m_file ( file )
{
  if (!m_file)
    throw std::runtime_error("[ERROR] File is not open\n");
}

bool Parser::hasMoreLines() {
  while (std::getline(m_file, m_currentLine)) {
    if (!m_currentLine.empty())
      return true;
  }
  return false;
}

void Parser::advance() {
  std::istringstream iss(m_currentLine);

  m_arg1.clear();
  m_arg2.clear();

  iss >> m_cmd >> m_arg1 >> m_arg2;
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
  CommandType cmd_type { commandType() };
  if (CommandType::C_RETURN == cmd_type)
    throw std::runtime_error("[ERROR] Should not be called on C_RETURN\n");
  if (CommandType::C_ARITHMETIC == cmd_type)
    return m_cmd;

  return m_arg1;
}

int Parser::arg2() const {
  CommandType cmd_type { commandType() };
  if (cmd_type != CommandType::C_PUSH      &&
      cmd_type != CommandType::C_POP       &&
      cmd_type != CommandType::C_FUNCTION  &&
      cmd_type != CommandType::C_CALL)
    throw std::runtime_error("[ERROR] Should not be called on C_PUSH, C_POP, C_FUNCTION, C_CALL");

  return stoi(m_arg2);
}
