#pragma once

#include "../CompilationEngine/compileEngine.h"
#include "../Tokenizer/tokenizer.h"
#include <fstream>

class CompilerAnalyzer {
  private:
    Tokenizer         m_tokenizer;
    std::ofstream&    m_xmlFile;
    CompilationEngine m_engine;
    
  public:
   /*
    * Okay so this is how it should go so basically we take in a JackFile 
    * we Intialize the internal m_jackFile; and m_xmlFile; After doing that we 
    * then Intialize both the tokenizer and Compilation Engine consturctor
    *
    * then we let them handle the rest and we control
    * */
  CompilerAnalyzer(std::ifstream& jackFile, std::ofstream& xmlFile, const std::string& fileName);
    CompilerAnalyzer& operator=(CompilerAnalyzer&) = delete;
    CompilerAnalyzer(CompilerAnalyzer&) = delete;

    // @brief runs the Compiler Loop
    void run();
};
