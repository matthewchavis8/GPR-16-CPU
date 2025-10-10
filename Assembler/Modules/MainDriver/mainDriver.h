#pragma once

#include "../Parser/parser.h"
#include "../Code/code.h"
#include "../SymbolTable/symbolTable.h"
#include <fstream>
#include <string_view>

/**
 * @brief Orchestrates the two-pass assembly process for generating machine code.
 *
 * Coordinates parser, code generator, and symbol table to convert assembly
 * source into binary output.
 */
class MainDriver {
  private:
    Parser        m_parser;
    Code          m_code;
    SymbolTable   m_symbolTable;
    std::string   m_file_name;

    /**
     * @brief Builds the symbol table by scanning all label declarations.
     */
    void firstPass();

    /**
     * @brief Translates instructions into machine code using the symbol table.
     */
    void secondPass();
  public:
    /**
     * @brief Constructs the main driver and initializes the parser with input file.
     * @param file Input assembly file stream.
     * @param file_name Name of the source file (used for output generation).
     */
    MainDriver(std::ifstream& file, const std::string& file_name);
    MainDriver& operator=(const MainDriver&) = delete;

    /**
     * @brief Executes the complete two-pass assembly process and writes output.
     */
    void run();

};
