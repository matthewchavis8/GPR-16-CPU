#pragma once

#include <cstdint>
#include <fstream>

class CompilationEngine {
  private:
    std::ifstream& m_input_file;
    std::ofstream& m_output_file;
    
  public:
    CompilationEngine(std::ifstream& input_file, std::ofstream& output_file);
    CompilationEngine operator=(CompilationEngine&) = delete;
    CompilationEngine(CompilationEngine&) = delete;

    void compileClass();

    void compileClassVarDec();

    void compileSubroutine();

    void compileParameterList();

    void compileSubroutineBody();

    void compileVarDec();

    void compileStatements();

    void compileLet();

    void compileIf();

    void compileWhile();

    void compileDo();

    void compileReturn();

    void compileExpression();

    void compileTerm();

    void compileExpressionList(uint32_t);
};
