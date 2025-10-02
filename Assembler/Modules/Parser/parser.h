#pragma once

#include <fstream>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>
#include "../Utils/commandType.cpp"

class Parser {
  private:
    std::ifstream& m_file;
    std::string_view m_file_name;
    std::string m_command;
    std::optional<std::string> m_lookahead_buffer;
    std::streampos m_position;

  public:
    Parser(std::ifstream& file, const std::string_view file_name);
    Parser& operator=(Parser const&) = delete;
    
    bool hasMoreCommands();

    void advance();

    CommandType commandType();
    
    std::string_view symbol();

    std::string_view dest();

    std::string_view comp();

    std::string_view jump();
};
