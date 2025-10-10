#pragma once

#include "../Parser/parser.h"
#include "../Code/code.h"
#include "../SymbolTable/symbolTable.h"
#include <fstream>
#include <string_view>


class MainDriver {
  private:
    Parser      m_parser;
    Code        m_code;
    SymbolTable m_symbolTable;
    
    void firstPass();
    void secondPass();
  public:
    MainDriver(std::ifstream& file, std::string_view file_name);
    MainDriver& operator=(const MainDriver&) = delete;

    void run();
    
};
