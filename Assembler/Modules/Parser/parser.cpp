#include <iosfwd>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include "parser.h"

Parser::Parser(std::ifstream& file, const std::string_view file_name) 
  : m_file { file } 
  , m_file_name { file_name }
{ 
    if (m_file_name.length() < 1)
      throw std::invalid_argument("[ERROR] File does not exit\n");

    int size = m_file_name.length();
    
    // Check if file is assembly or not    
    if (m_file_name.substr(size - 3) != "asm")
      throw std::invalid_argument("[Error] File is not an assembly\n");
    
    // Check if file is open or not if not open it
    if (!m_file.is_open())
      m_file.open(m_file_name);
  
    if (!m_file)
      throw std::runtime_error("[ERROR] unable to open file\n");

    m_file >> m_command;          // Load Current Comamand
    std::string lookahead;
    if (m_file >> lookahead)
      m_lookahead_buffer  = std::move(lookahead); // Load Next Command
}

bool Parser::hasMoreCommands() {
  if (!m_file.is_open())
    throw std::runtime_error("[ERROR] File is not open\n");
  
  return m_lookahead_buffer.has_value();
}

void Parser::advance() {
  if (!hasMoreCommands())
    return;

  if (!m_file.is_open())
    throw std::runtime_error("[ERROR] File is not open\n");
  
  // Move the buffer into the current command 
  m_command = std::move(*m_lookahead_buffer);

  // Move the next token into lookahead buffer if not leave it null
  std::string next_token;
  if (m_file >> next_token) 
    m_lookahead_buffer = std::move(next_token);
  else 
    m_lookahead_buffer.reset();
}

CommandType Parser::commandType() const {
  if (m_command.empty())
    throw std::runtime_error("[ERROR] Empty command encountered\n");

  if (m_command[0] == '@')
    return CommandType::A_COMMAND;
  if (m_command.front() == '(' && m_command.back() == ')')
    return CommandType::L_COMMAND;

  return CommandType::C_COMMAND;
}

std::string Parser::symbol() const {
  CommandType cmd_type = this->commandType();

  if (cmd_type == CommandType::A_COMMAND)
    return m_command.substr(1);
  if (cmd_type == CommandType::L_COMMAND)
    return m_command.substr(1, m_command.size() - 2);
  return "";
}
std::string Parser::dest() const {
  CommandType cmd_type = commandType();

  // Safety check should only be called on C_COMMAND
  if (cmd_type != CommandType::C_COMMAND)
    throw std::runtime_error("[ERROR] cmd_type is not a C_COMMAND\n");

  std::size_t eq_sign { m_command.find("=") };
  if (eq_sign != std::string::npos)
    return m_command.substr(0, eq_sign);

  return "";
}
std::string Parser::comp() const {
  CommandType cmd_type = this->commandType();

  // Safety check should only be called on C_COMMAND
  if (cmd_type != CommandType::C_COMMAND)
    throw std::runtime_error("[ERROR] cmd_type is not a C_COMMAND\n");

  std::size_t eq_sign { m_command.find("=") };
  std::size_t semi_sign { m_command.find(";") };

  size_t comp_start = (eq_sign != std::string::npos) ? eq_sign + 1 : 0;
  size_t comp_end = (semi_sign != std::string::npos) ? semi_sign : m_command.size();

  return m_command.substr(comp_start, comp_end - comp_start);
}
std::string Parser::jump() const {
  CommandType cmd_type = this->commandType();

  // Safety check should only be called on C_COMMAND
  if (cmd_type != CommandType::C_COMMAND)
    throw std::runtime_error("[ERROR] cmd_type is not a C_COMMAND\n");
  
  size_t semi_pos = m_command.find(';');
  if (semi_pos != std::string::npos)
    return m_command.substr(semi_pos + 1);

  return "";
}

const std::string& Parser::getCommand() const { return m_command; }
const std::string& Parser::getLookAheadBuffer() const { return *m_lookahead_buffer; }
