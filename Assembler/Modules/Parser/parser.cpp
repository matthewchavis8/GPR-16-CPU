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
      std::invalid_argument("[ERROR] File does not exit\n");

    int n = m_file_name.length();
    
    // Check if file is assembly or not    
    if (m_file_name[n - 3] == 'a' && m_file_name[n - 2] == 's' && m_file_name[n - 1] == 'm')
      std::invalid_argument("[Error] File is not an assembly\n");
    
    // Check if file is open or not if not open it
    if (!m_file.is_open())
      m_file.open(m_file_name);
  
    if (!m_file)
      std::runtime_error("[ERROR] unable to open file\n");

    m_file >> m_command;          // Load Current Comamand
    m_file >> *m_lookahead_buffer; // Load Next Command
}

bool Parser::hasMoreCommands() {
  if (!m_file.is_open())
    std::runtime_error("[ERROR] File is not open\n");
  
  return m_lookahead_buffer.has_value();
}

void Parser::advance() {
  if (!hasMoreCommands())
    return;

  if (!m_file.is_open())
    std::runtime_error("[ERROR] File is not open\n");
  
  // Move the buffer into the current command 
  m_command = std::move(*m_lookahead_buffer);

  // Move the next token into lookahead buffer if not leave it null
  std::string next_token;
  if (m_file >> next_token) 
    m_lookahead_buffer = std::move(next_token);
  else 
    m_lookahead_buffer.reset();
}

const std::string& Parser::getCommand() { return m_command; }
const std::string& Parser::getLookAheadBuffer() { return *m_lookahead_buffer; }
