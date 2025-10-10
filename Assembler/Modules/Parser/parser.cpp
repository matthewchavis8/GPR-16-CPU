#include <fstream>
#include <stdexcept>
#include <string>
#include "parser.h"

Parser::Parser(std::ifstream& file, const std::string& file_name)
  : m_file { file }
  , m_file_name { file_name }
{
    if (m_file_name.length() < 1)
      throw std::invalid_argument("[ERROR] File does not exit\n");

    int size = m_file_name.length();

    // Validate .asm file
    if (m_file_name.substr(size - 3) != "asm")
      throw std::invalid_argument("[Error] File is not an assembly\n");

    if (!m_file.is_open())
      m_file.open(m_file_name);

    if (!m_file)
      throw std::runtime_error("[ERROR] unable to open file\n");

    // Initialize with first command and lookahead buffer
    m_file >> m_command;
    std::string lookahead;
    if (m_file >> lookahead)
      m_lookahead_buffer = std::move(lookahead);
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

  m_command = std::move(*m_lookahead_buffer);

  // Read next token or clear buffer if at end of file
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

  if (cmd_type != CommandType::C_COMMAND)
    throw std::runtime_error("[ERROR] cmd_type is not a C_COMMAND\n");

  std::size_t eq_sign { m_command.find("=") };
  if (eq_sign != std::string::npos)
    return m_command.substr(0, eq_sign);

  return "";
}
std::string Parser::comp() const {
  CommandType cmd_type = this->commandType();

  if (cmd_type != CommandType::C_COMMAND)
    throw std::runtime_error("[ERROR] cmd_type is not a C_COMMAND\n");

  std::size_t eq_sign { m_command.find("=") };
  std::size_t semi_sign { m_command.find(";") };

  // Extract computation between '=' and ';'
  std::size_t comp_start = (eq_sign != std::string::npos) ? eq_sign + 1 : 0;
  std::size_t comp_end = (semi_sign != std::string::npos) ? semi_sign : m_command.size();

  return m_command.substr(comp_start, comp_end - comp_start);
}
std::string Parser::jump() const {
  CommandType cmd_type = this->commandType();

  if (cmd_type != CommandType::C_COMMAND)
    throw std::runtime_error("[ERROR] cmd_type is not a C_COMMAND\n");

  std::size_t semi_pos = m_command.find(';');
  if (semi_pos != std::string::npos)
    return m_command.substr(semi_pos + 1);

  return "";
}

const std::string& Parser::getCommand() const { return m_command; }
const std::string& Parser::getLookAheadBuffer() const { return *m_lookahead_buffer; }

void Parser::reset() {
  m_file.clear();
  m_file.seekg(0, std::ios::beg);
  m_command.clear();
  m_lookahead_buffer.reset();

  // Re-initialize with first command and lookahead buffer
  m_file >> m_command;
  std::string lookahead;
  if (m_file >> lookahead)
    m_lookahead_buffer = std::move(lookahead);
}
