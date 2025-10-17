#pragma once

#include <fstream>
#include "../Utils/CommandType.h"

class Parser {
  private:
    std::ifstream& m_file;
    std::string m_currentLine;
    std::string m_cmd;
    std::string m_arg1;
    std::string m_arg2;

  public:
    Parser(std::ifstream& file);
    Parser(const Parser&) = delete;

    bool hasMoreLines();

    void advance();

    CommandType commandType() const;
    
    std::string arg1() const;

    int arg2() const;
};
