/** @file
 *  @brief Declaration of the `CompilationEngine` which parses Jack tokens
 *         into an XML representation of the program structure.
 */
#pragma once

#include <cstdint>
#include <fstream>
#include <initializer_list>
#include <string_view>
#include "../Tokenizer/tokenizer.h"

/**
 * @brief Recursive-descent compiler that consumes tokens from a `Tokenizer`
 *        and emits the corresponding XML parse tree.
 */
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

    void emitKeyWordAndAdvance();
    void emitSymbolAndAdvance();
    void emitIdentifierAndAdvance();
    void emitIntAndAdvance();
    void emitStringAndAdvance();

    bool isTypeToken() const;
    bool isKeywordConstant() const;
    bool isUnaryOp() const;
    static bool isOp(char c);
  
  public:
    /** @brief Construct a compilation engine bound to a tokenizer and output stream. */
    CompilationEngine(Tokenizer& tokenizer, std::ofstream& output_file);
    CompilationEngine operator=(CompilationEngine&) = delete;
    CompilationEngine(CompilationEngine&) = delete;

    /** @brief Compile an entire class starting at the `class` keyword. */
    void compileClass();

    /** @brief Compile a class-level variable declaration (`static` / `field`). */
    void compileClassVarDec();

    /** @brief Compile a subroutine declaration (`constructor`, `function`, `method`). */
    void compileSubroutine();

    /** @brief Compile a (possibly empty) comma-separated parameter list. */
    void compileParameterList();

    /** @brief Compile the body of a subroutine, including local vars and statements. */
    void compileSubroutineBody();

    /** @brief Compile a local variable declaration (`var`). */
    void compileVarDec();

    /** @brief Compile a sequence of statements until a closing delimiter is reached. */
    void compileStatements();

    /** @brief Compile a `let` assignment statement (with optional array indexing). */
    void compileLet();

    /** @brief Compile an `if` statement, including optional `else` branch. */
    void compileIf();

    /** @brief Compile a `while` loop statement. */
    void compileWhile();

    /** @brief Compile a `do` statement (a subroutine call as a statement). */
    void compileDo();

    /** @brief Compile a `return` statement, with or without an expression. */
    void compileReturn();

    /** @brief Compile an expression, including binary operators. */
    void compileExpression();

    /** @brief Compile a single term within an expression. */
    void compileTerm();

    /**
     * @brief Compile a (possibly empty) comma-separated list of expressions.
     * @return Number of expressions compiled.
     */
    uint32_t compileExpressionList();
};
