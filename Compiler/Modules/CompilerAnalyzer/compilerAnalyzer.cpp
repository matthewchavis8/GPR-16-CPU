#include "../CompilerAnalyzer/compilerAnalyzer.h"
#include "../Utils/log.h"
#include <filesystem>
#include <stdexcept>

CompilerAnalyzer::CompilerAnalyzer(const std::filesystem::path& filePath)
  : m_jackFile(filePath)
  , m_vmFile([&filePath]() {
      std::filesystem::path vmPath { filePath };
      vmPath.replace_extension(".vm");
      return vmPath;
    }())
  , m_tokenizer(m_jackFile)
  , m_vmWriter(m_vmFile)
  , m_engine(m_tokenizer, m_vmWriter)
{
  if (filePath.extension() != ".jack")
    log<std::logic_error>("Input file is not a .jack file");
}

void CompilerAnalyzer::run() {
  m_engine.compileClass();
  m_vmFile.flush();
}

