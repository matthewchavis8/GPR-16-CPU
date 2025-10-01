#include <iostream>
#include <fstream>
#include "parser.h"

Parser::Parser(std::ifstream& file, std::string_view file_name) 
  : m_file { file } 
  , m_file_name { file_name }
  , m_position { 0 }
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
}

bool Parser::hasMoreCommands() {
  return true;
}
