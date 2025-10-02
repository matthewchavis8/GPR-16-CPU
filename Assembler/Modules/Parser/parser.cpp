#include <iosfwd>
#include <iostream>
#include <fstream>
#include <utility>
#include "parser.h"

Parser::Parser(std::ifstream& file, std::string_view file_name) 
  : m_file { file } 
  , m_file_name { file_name }
{ 
    if (m_file_name.length() < 1)
      std::cerr << "[Error] file_name was not passed" << '\n';

    int n = m_file_name.length();
    
    // Check if file is assembly or not    
    if (m_file_name[n - 3] == 'a' && m_file_name[n - 2] == 's' && m_file_name[n - 3] == 'm')
      std::cerr << "[Error] File is not an assembly" << '\n';
    
    // Check if file is open or not if not open it
    if (!m_file.is_open())
      m_file.open(m_file_name);
  
    if (!m_file)
      std::cerr << "[ERROR] unable to open file" << '\n';

    m_file >> m_command;          // Load Current Comamand
    m_file >> *m_lookahead_buffer; // Load Next Command
}

bool Parser::hasMoreCommands() {
  if (!m_file.is_open())
    std::cerr << "[ERROR] File is not open" << '\n';
  
  // Check look ahead buffer if it has a value
  if (m_lookahead_buffer.has_value())
    return true;
  
  return false;
}

void Parser::advance() {
  if (!hasMoreCommands())
    return;

  if (!m_file.is_open())
    std::cerr << "[ERROR] File is not open" << '\n';
  
  // Move the buffer into the current command 
  m_command = std::move(*m_lookahead_buffer);

  // Move the next token into lookahead buffer if not leave it null
  std::string next_token;
  if (m_file >> next_token) 
    m_lookahead_buffer = std::move(next_token);
  else 
    m_lookahead_buffer.reset();
}
