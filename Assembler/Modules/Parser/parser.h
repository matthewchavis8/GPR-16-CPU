#pragma once

#include <fstream>
#include <string_view>
#include "../Utils/commandType.cpp"

class Parser {
  public:
    Parser(std::ifstream& file, std::string_view file_name);
    
    bool hasMoreCommands();

    void advance();

    CommandType commandType();
    
    std::string_view symbol();

    std::string_view dest();

    std::string_view comp();

    std::string_view jump();
};
