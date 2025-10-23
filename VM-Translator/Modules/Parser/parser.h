#pragma once

/**
 * @file parser.h
 * @brief Interface for parsing VM commands from an input stream.
 */

#include <fstream>
#include "../Utils/CommandType.h"

/**
 * @brief Iterates over a VM source and exposes the current command and args.
 */
class Parser {
  private:
    /** @brief Backing stream for the VM source file. */
    std::ifstream& m_file;
    /** @brief Raw current line after normalization (comments/whitespace removed). */
    std::string m_currentLine;
    /** @brief Current command mnemonic or segment keyword. */
    std::string m_cmd;
    /** @brief First argument of the current command (if any). */
    std::string m_arg1;
    /** @brief Second argument of the current command (if any). */
    std::string m_arg2;

  public:
    /**
     * @brief Constructs a parser bound to an open VM input stream.
     * @param file Open input stream that remains valid for the parser lifetime.
     */
    Parser(std::ifstream& file);
    Parser(const Parser&) = delete;

    /**
     * @brief Indicates whether more commands remain in the input.
     * @return true if another command can be advanced to; otherwise false.
     */
    bool hasMoreLines();

    /**
     * @brief Reads the next command and updates the current state.
     *
     * Skips blank lines and comments, normalizing tokens for subsequent queries.
     */
    void advance();

    /**
     * @brief Returns the type of the current VM command.
     */
    CommandType commandType() const;
    
    /**
     * @brief Returns the first argument of the current command.
     *
     * For C_ARITHMETIC, returns the command itself; undefined if no argument.
     */
    std::string arg1() const;

    /**
     * @brief Returns the second argument of the current command.
     *
     * Only valid for C_PUSH, C_POP, C_FUNCTION, and C_CALL.
     */
    int arg2() const;
};
