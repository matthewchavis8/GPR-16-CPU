#pragma once

/**
 * @file codeWriter.h
 * @brief Interface for translating VM commands into Hack assembly.
 */
#include <cstdint>
#include <fstream>
#include "../Utils/CommandType.h"


/**
 * @brief Emits Hack assembly for a stream of VM commands.
 */
class CodeWriter {
  private:
    /** @brief Output stream for emitted assembly. */
    std::ofstream m_output_file;
    /** @brief Target output filename. */
    std::string m_file_name;
    /** @brief Monotonic counter used to generate unique labels. */
    int m_labelCounter{};

    std::string m_current_func;

    std::string qualifyLabel(const std::string& label) const;

    std::string m_static_base {"Static"};
  

  public:
    /**
     * @brief Constructs a writer that emits assembly to the given file.
     * @param fileName Path to the output `.asm` file.
     */
    CodeWriter(const std::string& fileName);
    CodeWriter(const CodeWriter&) = delete;

    /**
     * @brief Writes assembly for an arithmetic/logic VM command.
     * @param cmd One of: "add", "sub", "neg", "eq", "gt", "lt", "and", "or", "not".
     */
    void writeArithmetic(const std::string& cmd);
    
    /**
     * @brief Sets the current output file context (basename used for symbols).
     * @param file_name Output file name (used for static symbols).
     */
    void setFileName(const std::string& file_name);
    /**
     * @brief Writes assembly for a push or pop VM command.
     * @param cmdType Command kind (C_PUSH or C_POP).
     * @param segment Memory segment name (e.g., "local", "argument", "this", "that", "temp", "pointer", "static", "constant").
     * @param idx Segment index.
     */
    void writePushPop(CommandType cmdType, const std::string& segment, uint32_t idx);

    void writeLabel(const std::string& label);

    void writeGoto(const std::string& label);

    void writeIf(const std::string& label);

    void writeFunction(const std::string& functionName, uint32_t nVars);

    void writeCall(const std::string& functionName, uint32_t nArgs);

    void writeReturn();

    void setCurrentFile(std::string base);
    /**
     * @brief Flushes and closes the underlying output stream.
     */
    void close();
};
