#include "compileEngine.h"
#include "../Tokenizer/tokenizer.h"
#include <fstream>

CompilationEngine::CompilationEngine(Tokenizer& tokenizer, std::ofstream& output_file)
  : m_tokenizer(tokenizer)
  , m_output_file(output_file)
{}


