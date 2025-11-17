#include "../Tokenizer/tokenizer.h"
#include "../CompilationEngine/compileEngine.h"
#include <fstream>
#include "../CompilerAnalyzer/compilerAnalyzer.h"

CompilerAnalyzer::CompilerAnalyzer(std::ifstream& jackFile, std::ofstream& xmlFile, const std::string& fileName)
: m_tokenizer(jackFile, fileName)
, m_xmlFile(xmlFile)
, m_engine(m_tokenizer, m_xmlFile)
{}

void CompilerAnalyzer::run() {
  m_engine.compileClass();
  m_xmlFile.flush();
}
