#pragma once

#include <cstdint>
#include <fstream>
#include <string_view>
#include "../Utils/segment.h"
#include "../Utils/command.h"

class VmWriter {
  private:
    std::ofstream& m_vmFile;

  public:
    VmWriter(std::ofstream& vmFile);
    VmWriter& operator=(VmWriter&) = delete;
    VmWriter(VmWriter&) = delete;

    void writePush(Segment segment, uint32_t idx);
    
    void writePop(Segment segment, uint32_t idx);

    void writeArithmetic(Command cmd);

    void writeLabel(std::string_view label);

    void writeGoto(std::string_view label);

    void writeIf(std::string_view label);

    void writeCall(std::string_view name, uint32_t nArgs);
    
    void writeFunction(std::string_view name, uint32_t nArgs);

    void writeReturn();

    void close();
};
