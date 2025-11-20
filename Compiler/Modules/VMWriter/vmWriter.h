#pragma once

#include <cstdint>
#include <fstream>
#include <string_view>
#include "../Utils/segment.h"
#include "../Utils/command.h"

class VmWriter {
  private:
    std::ofstream& m_vmFile;

    std::string segmentToStr(Segment segment) {
      switch (segment) {
        case Segment::Argument: return "arg";
        case Segment::Constant: return "constant";
        case Segment::Local:    return "local";
        case Segment::Pointer:  return "pointer";
        case Segment::Static:   return "static";
        case Segment::Temp:     return "temp";
        case Segment::This:     return "this";
        case Segment::That:     return "that";
      }
    }
    
    std::string cmdToStr(Command cmd) {
      switch (cmd) {
        case Command::Add: return "add";
        case Command::And: return "and";
        case Command::Eq:  return "eq";
        case Command::Gt:  return "gt";
        case Command::Neg: return "neg";
        case Command::Lt:  return "lt";
        case Command::Not: return "not";
        case Command::Or:  return "or";
        case Command::Sub: return "sub";
      }
    }

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
