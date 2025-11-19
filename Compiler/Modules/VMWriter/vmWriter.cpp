
#include "vmWriter.h"
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string_view>
#include "../Utils/segment.h"
#include "../Utils/command.h"

VmWriter::VmWriter(std::ofstream& vmFile)
 : m_vmFile(vmFile)
{
  if (!m_vmFile.is_open())
    throw std::runtime_error("[ERROR] VM file is not open");
}

void VmWriter::writePush(Segment segment, uint32_t idx) {}

void VmWriter::writePop(Segment segment, uint32_t idx) {}

void VmWriter::writeArithmetic(Command cmd) {}

void VmWriter::writeLabel(std::string_view label) {}

void VmWriter::writeGoto(std::string_view label) {}

void VmWriter::writeIf(std::string_view label) {}

void VmWriter::writeCall(std::string_view name, uint32_t nArgs) {}

void VmWriter::writeFunction(std::string_view name, uint32_t nArgs) {}

void VmWriter::writeReturn() {}

void VmWriter::close() { m_vmFile.close(); }
