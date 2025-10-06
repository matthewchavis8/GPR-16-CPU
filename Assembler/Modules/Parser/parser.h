#pragma once

#include <fstream>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>
#include "../Utils/commandType.h"

class Parser {
  private:
    std::ifstream& m_file;
    std::string_view m_file_name;
    std::string m_command;
    std::optional<std::string> m_lookahead_buffer;

  public:
    Parser(std::ifstream& file, const std::string_view file_name);
    Parser& operator=(Parser const&) = delete;
    
    bool hasMoreCommands();

    void advance();

    CommandType commandType() const;
    
    std::string symbol() const;

    std::string dest() const;

    std::string comp() const;

    std::string jump() const;

    const std::string& getCommand() const;
    const std::string& getLookAheadBuffer() const;
};
