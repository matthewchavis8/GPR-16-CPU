#pragma once

#include <fstream>
#include <optional>
#include <string>
#include <string_view>
#include "../Utils/commandType.h"

/**
 * @class Parser
 * @brief Encapsulates reading and parsing of Hack assembly commands.
 *
 * Provides methods to classify commands and extract symbols, destinations,
 * computations, and jump mnemonics according to the Hack assembler specification.
 * Uses a lookahead buffer strategy to enable single-pass command processing.
 */
class Parser {
  private:
    // @brief Reference to the input file stream containing assembly code.
    std::ifstream& m_file;

    // @brief Name of the source assembly file.
    std::string m_file_name;

    // @brief The current assembly command being processed.
    std::string m_command;

    // @brief Lookahead buffer holding the next command, if available.
    std::optional<std::string> m_lookahead_buffer;

  public:
    /**
     * @brief Constructs a Parser instance bound to an input file stream.
     *
     * Validates that the file name ends with `.asm` and opens the stream if necessary.
     * Preloads the first command and initializes the lookahead buffer with the next token.
     *
     * @param file Reference to an input file stream (may be pre-opened or closed).
     * @param file_name Path to the assembly source file (must end in `.asm`).
     * @throw std::invalid_argument If file_name is empty or does not end in `.asm`.
     * @throw std::runtime_error If the file cannot be opened or read.
     */
    Parser(std::ifstream& file, const std::string& file_name);

    /**
     * @brief Deleted copy assignment operator.
     *
     * Prevents assignment to ensure single ownership of the file stream reference.
     */
    Parser& operator=(Parser const&) = delete;

    /**
     * @brief Checks whether additional commands remain in the input stream.
     *
     * @return True if the lookahead buffer contains a command, false if end of file reached.
     * @throw std::runtime_error If the file stream is not open.
     */
    bool hasMoreCommands();

    /**
     * @brief Advances to the next command in the input stream.
     *
     * Moves the lookahead buffer into the current command and reads the next token
     * into the buffer. If no more commands exist, the operation has no effect.
     *
     * @throw std::runtime_error If the file stream is not open.
     * @pre hasMoreCommands() must return true.
     * @post m_command contains the next command; m_lookahead_buffer is updated or cleared.
     */
    void advance();

    /**
     * @brief Determines the type of the current command.
     *
     * Classifies the current command as an A-instruction, C-instruction, or label.
     *
     * @return CommandType enum: A_COMMAND (@xxx), L_COMMAND ((xxx)), or C_COMMAND (dest=comp;jump).
     * @throw std::runtime_error If the current command is empty.
     */
    CommandType commandType() const;

    /**
     * @brief Extracts the symbol from an A-instruction or label command.
     *
     * For A-instructions (@symbol), returns the symbol without the '@' prefix.
     * For label declarations ((symbol)), returns the symbol without parentheses.
     *
     * @return The extracted symbol string, or empty string if not applicable.
     * @pre commandType() must return A_COMMAND or L_COMMAND.
     */
    std::string symbol() const;

    /**
     * @brief Extracts the destination mnemonic from a C-instruction.
     *
     * Parses the left side of the '=' operator in a C-instruction (e.g., "D" in "D=A+1").
     *
     * @return The destination mnemonic, or empty string if no destination specified.
     * @throw std::runtime_error If commandType() is not C_COMMAND.
     */
    std::string dest() const;

    /**
     * @brief Extracts the computation mnemonic from a C-instruction.
     *
     * Parses the computation part between '=' and ';' in a C-instruction (e.g., "A+1" in "D=A+1;JGT").
     *
     * @return The computation mnemonic.
     * @throw std::runtime_error If commandType() is not C_COMMAND.
     */
    std::string comp() const;

    /**
     * @brief Extracts the jump mnemonic from a C-instruction.
     *
     * Parses the right side of the ';' operator in a C-instruction (e.g., "JGT" in "D;JGT").
     *
     * @return The jump mnemonic, or empty string if no jump specified.
     * @throw std::runtime_error If commandType() is not C_COMMAND.
     */
    std::string jump() const;

    /**
     * @brief Returns the current command string.
     *
     * @return Const reference to the current command being processed.
     */
    const std::string& getCommand() const;

    /**
     * @brief Returns the next command from the lookahead buffer.
     *
     * @return Const reference to the lookahead buffer content.
     * @pre m_lookahead_buffer must contain a value.
     */
    const std::string& getLookAheadBuffer() const;

    /**
     * @brief Rewinds the input stream and reinitializes parser state.
     * @post m_command is set to the first token; m_lookahead_buffer holds the next token if available.
     */
    void reset();
};
