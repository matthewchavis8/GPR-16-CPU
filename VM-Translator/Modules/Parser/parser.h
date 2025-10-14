#pragma once

#include <fstream>
#include "../Utils/CommandType.h"

class Parser {
  private:
    std::ifstream m_file;

  public:
    Parser() = default;
    Parser(const Parser&) = delete;

    bool hasMoreLines();

    void advance();

    CommandType commandType();
    
    std::string arg1(const std::string& cmd) const;

    int arg2(int) const;
};
