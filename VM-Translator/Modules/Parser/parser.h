#pragma once

#include <fstream>
#include <optional>
#include "../Utils/CommandType.h"

class Parser {
  private:
    std::ifstream& m_file;
    std::string m_cmd;
    std::optional<std::string> m_lookaheadBuffer;

  public:
    Parser(std::ifstream& file);
    Parser(const Parser&) = delete;

    bool hasMoreLines();

    void advance();

    CommandType commandType() const;
    
    std::string arg1() const;

    int arg2() const;
};
