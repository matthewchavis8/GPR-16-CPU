/** @file
 *  @brief Declaration of the `CompilationEngine` which parses Jack tokens
 *         and emits Nand2Tetris VM code.
 */
#pragma once

#include <cstdint>
#include <string>
#include "../Tokenizer/tokenizer.h"
#include "../VMWriter/vmWriter.h"
#include "../SymbolTable/symbolTable.h"

/**
 * @brief Recursive-descent compiler that consumes tokens from a `Tokenizer`
 *        and emits the corresponding Nand2Tetris VM code via `VmWriter`.
 */
class CompilationEngine {
  private:
    Tokenizer& m_tokenizer;
    VmWriter& m_VmWriter;
    SymbolTable m_classTable;
    SymbolTable m_subroutineTable;
    std::string m_className;
    std::string m_subroutineName;
    bool m_isMethod{false};
    bool m_isConstructor{false};
    uint32_t m_ifLabelIdx{0};
    uint32_t m_whileLabelIdx{0};

    /** @brief Consume and validate that the current token is the given keyword. */
    void expectKeyword(Keyword kw);

    /** @brief Consume and validate that the current token is the given symbol. */
    void expectSymbol(char ch);

    /** @brief Consume and return the current identifier token text. */
    std::string expectIdentifier();

    /** @brief Check whether the current token is the given symbol. */
    bool isSymbol(char ch) const;

    /** @brief Check whether the current token is the given keyword. */
    bool isKeyword(Keyword kw) const;

    /** @brief Map a symbol-table kind to its corresponding VM memory segment. */
    Segment kindToSegment(IdentifierKind kind) const;

    /** @brief Build a function-scoped label name of the form `Class.sub$baseN`. */
    std::string makeLabel(const std::string& base, uint32_t index) const;
    
  public:
    /** @brief Construct a compilation engine bound to a tokenizer and output stream. */
    CompilationEngine(Tokenizer& tokenizer, VmWriter& vmWriter);
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
