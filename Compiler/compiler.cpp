#include "Modules/CompilerAnalyzer/compilerAnalyzer.h"
#include <filesystem>
#include <stdexcept>

int main(int argc, char* argv[]) {
  if (argc != 2)
    throw std::runtime_error("[ERROR] Usage: Compiler <input.jack>");

  std::filesystem::path filePath { argv[1] };

  CompilerAnalyzer analyzer(filePath);
  analyzer.run();

  return 0;
}
