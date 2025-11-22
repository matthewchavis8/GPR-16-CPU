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

void VmWriter::writePush(Segment segment, uint32_t idx) {
  m_vmFile << "push " << segmentToStr(segment) << " " << idx << "\n";
}

void VmWriter::writePop(Segment segment, uint32_t idx) {
  m_vmFile << "pop " << segmentToStr(segment) << " " << idx << "\n";
}

void VmWriter::writeArithmetic(Command cmd) {
  m_vmFile << cmdToStr(cmd) << '\n';
}

void VmWriter::writeLabel(std::string_view label) {
  m_vmFile << "label " << label << '\n';
}

void VmWriter::writeGoto(std::string_view label) {
  m_vmFile << "goto " << label << '\n';
}

void VmWriter::writeIf(std::string_view label) {
  m_vmFile << "if-goto " << label << '\n';
}

void VmWriter::writeCall(std::string_view fnName, uint32_t nArgs) {
  m_vmFile << "call " << fnName << " " << nArgs << '\n';
}

void VmWriter::writeFunction(std::string_view fnName, uint32_t nArgs) {
  m_vmFile << "function " << fnName << " " << nArgs << '\n';
}

void VmWriter::writeReturn() {
  m_vmFile << "return" << '\n';
}

void VmWriter::close() { m_vmFile.close(); }
