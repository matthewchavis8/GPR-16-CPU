#pragma once

#include <cstdint>
#include <fstream>
#include <initializer_list>
#include <string_view>
#include "../Tokenizer/tokenizer.h"

class CompilationEngine {
  private:
    Tokenizer& m_tokenizer;
    std::ofstream& m_output_file;
    
    // XML helpers
    void openTag(std::string_view name);
    void closeTag(std::string_view name);

    void emitIdentifier(std::string_view id);
    void emitString(std::string_view str);

    void expectOneOf(std::initializer_list<std::string_view> wrds);
    void peekOneOf(std::initializer_list<std::string_view> wrds);
    
    
  public:
    CompilationEngine(Tokenizer& tokenizer, std::ofstream& output_file);
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

    uint32_t compileExpressionList();
};
