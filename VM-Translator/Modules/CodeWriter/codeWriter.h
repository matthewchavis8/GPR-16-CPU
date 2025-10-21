#pragma once

#include <fstream>
#include "../Utils/CommandType.h"


class CodeWriter {
  private:
    std::ifstream& m_file; //std move the file stream from the parser into here
    std::ofstream m_output_file; // Output file
    std::string m_file_name;     // output filename
    int m_labelCounter{};
  
  public:
    CodeWriter(std::ifstream& file);
    CodeWriter(const CodeWriter&) = delete;

    void setFileName(const std::string& file_name);

    void writeArithmetic(const std::string& cmd);
    
    void writePushPop(CommandType cmdType, const std::string& segment, int idx);

    void close();
};
