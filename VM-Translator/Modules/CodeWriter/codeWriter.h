#pragma once

#include <fstream>
#include "../Utils/CommandType.h"


class CodeWriter {
  private:
    std::ofstream m_output_file; // Output file
    std::string m_file_name;     // output filename
    int m_labelCounter{};
  
    void setFileName(const std::string& file_name);

  public:
    CodeWriter(const std::string& fileName);
    CodeWriter(const CodeWriter&) = delete;

    void writeArithmetic(const std::string& cmd);
    
    void writePushPop(CommandType cmdType, const std::string& segment, int idx);

    void close();
};
