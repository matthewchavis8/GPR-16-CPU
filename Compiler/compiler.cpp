#include "Modules/CompilerAnalyzer/compilerAnalyzer.h"
#include <fstream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
  if (argc != 2)
    throw std::runtime_error("[ERROR] Argument Count is not made\n");

  std::string fileName(argv[1]);

  std::ifstream jackFile(fileName);

  std::string xmlFileName = fileName;
  std::size_t dotPos { xmlFileName.rfind(".") };
  if (dotPos != std::string::npos)
    xmlFileName.replace(dotPos, std::string::npos, ".xml");
  else
    xmlFileName += ".xml";

  std::ofstream xmlFile(xmlFileName);
  CompilerAnalyzer analyzer(jackFile, xmlFile, fileName);
  analyzer.run();

  return 0;
}
