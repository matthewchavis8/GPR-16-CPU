#include "vmtranslator.h"
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <stdexcept>

#include "../Parser/parser.h"
#include "../CodeWriter/codeWriter.h"
#include "../Utils/CommandType.h"

namespace fs = std::filesystem;

std::vector<std::string> VMTranslator::collectVmFiles(const std::string& inPath) const {
  std::vector<std::string> files;
  fs::path p(inPath);

  if (!fs::exists(p)) throw std::runtime_error("Input path does not exist: " + inPath);

  if (fs::is_directory(p)) {
    for (auto& e : fs::directory_iterator(p)) {
      if (e.is_regular_file() && e.path().extension() == ".vm")
        files.push_back(e.path().string());
    }
    std::sort(files.begin(), files.end());
  } else {
    if (p.extension() == ".vm") files.push_back(p.string());
    else throw std::runtime_error("Input must be a .vm file or a directory.");
  }

  if (files.empty()) throw std::runtime_error("No .vm files found.");
  return files;
}

std::string VMTranslator::computeOutAsm(const std::string& inPath) const {
  fs::path p(inPath);
  if (fs::is_directory(p)) {
    auto base = p.filename().string();
    return (p / (base + ".asm")).string();
  }
  return (p.parent_path() / (p.stem().string() + ".asm")).string();
}

void VMTranslator::translateFile(const std::string& vmPath, CodeWriter& cw) const {
  // Static symbol base = file stem
  cw.setCurrentFile(fs::path(vmPath).stem().string());

  std::ifstream infile(vmPath);
  if (!infile) throw std::runtime_error("Failed to open: " + vmPath);

  Parser parser(infile); // your Parser takes std::ifstream&

  while (parser.hasMoreLines()) {
    parser.advance();

    switch (parser.commandType()) {
      case CommandType::C_ARITHMETIC:
        cw.writeArithmetic(parser.arg1());
        break;
      case CommandType::C_PUSH:
      case CommandType::C_POP:
        cw.writePushPop(parser.commandType(), parser.arg1(), static_cast<uint32_t>(parser.arg2()));
        break;
      case CommandType::C_LABEL:
        cw.writeLabel(parser.arg1());
        break;
      case CommandType::C_GOTO:
        cw.writeGoto(parser.arg1());
        break;
      case CommandType::C_IF:
        cw.writeIf(parser.arg1());
        break;
      case CommandType::C_FUNCTION:
        cw.writeFunction(parser.arg1(), static_cast<uint32_t>(parser.arg2()));
        break;
      case CommandType::C_CALL:
        cw.writeCall(parser.arg1(), static_cast<uint32_t>(parser.arg2()));
        break;
      case CommandType::C_RETURN:
        cw.writeReturn();
        break;
      default:
        // no-op / or throw if you prefer strictness
        break;
    }
  }
}

void VMTranslator::translate(const std::string& inPath, const std::string& outAsmPath) {
  const auto vmFiles = collectVmFiles(inPath);
  const std::string out = outAsmPath.empty() ? computeOutAsm(inPath) : outAsmPath;

  CodeWriter cw(out); // opens the .asm

  for (const auto& f : vmFiles) translateFile(f, cw);

  cw.close();
}
