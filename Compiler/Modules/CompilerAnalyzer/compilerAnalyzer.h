/** @file
 *  @brief High-level façade that owns compiler modules and drives compilation.
 */
#pragma once

#include "../CompilationEngine/compileEngine.h"
#include "../Tokenizer/tokenizer.h"
#include "../VMWriter/vmWriter.h"
#include <filesystem>
#include <fstream>

/**
 * @brief Validates input paths, owns I/O streams and modules, and invokes
 *        the `CompilationEngine` to translate Jack source to VM code.
 */
class CompilerAnalyzer {
  private:
    std::ifstream     m_jackFile;   /**< Open Jack source file stream. */
    std::ofstream     m_vmFile;     /**< VM output file stream. */
    Tokenizer         m_tokenizer;  /**< Lexical analyzer over `m_jackFile`. */
    VmWriter          m_vmWriter;   /**< VM writer bound to `m_vmFile`. */
    CompilationEngine m_engine;     /**< Core compilation engine. */
    
  public:
    /**
     * @brief Construct a compiler analyzer for the given Jack source file path.
     * @param filePath Path to a `.jack` source file.
     */
    CompilerAnalyzer(const std::filesystem::path& filePath);
    CompilerAnalyzer& operator=(CompilerAnalyzer&) = delete;
    CompilerAnalyzer(CompilerAnalyzer&) = delete;

    /** @brief Run the compilation pipeline from Jack source to VM output. */
    void run();
};
