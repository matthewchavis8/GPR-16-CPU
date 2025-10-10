#pragma once

#include "../Parser/parser.h"
#include "../Code/code.h"
#include "../SymbolTable/symbolTable.h"
#include <fstream>
#include <string_view>


class MainDriver {
  private:
    Parser        m_parser;
    Code          m_code;
    SymbolTable   m_symbolTable;
    std::string   m_file_name;
    std::ofstream m_outputFile;
    
    void firstPass();
    void secondPass();
  public:
    MainDriver(std::ifstream& file, const std::string& file_name);
    MainDriver& operator=(const MainDriver&) = delete;

    void run();
    
};
