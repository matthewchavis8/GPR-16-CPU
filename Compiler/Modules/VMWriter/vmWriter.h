/** @file
 *  @brief Writer for Nand2Tetris VM commands.
 */
#pragma once

#include <cstdint>
#include <fstream>
#include <string_view>
#include "../Utils/segment.h"
#include "../Utils/command.h"

/**
 * @brief Convenience wrapper that emits well-formed Nand2Tetris VM commands
 *        to a backing output stream.
 */
class VmWriter {
  private:
    std::ofstream& m_vmFile;

    /** @brief Convert a VM segment enum to its textual name. */
    std::string segmentToStr(Segment segment) {
      switch (segment) {
        case Segment::Argument: return "argument";
        case Segment::Constant: return "constant";
        case Segment::Local:    return "local";
        case Segment::Pointer:  return "pointer";
        case Segment::Static:   return "static";
        case Segment::Temp:     return "temp";
        case Segment::This:     return "this";
        case Segment::That:     return "that";
      }
      return "";
    }
    
    /** @brief Convert an arithmetic/logical command enum to its textual name. */
    std::string cmdToStr(Command cmd) {
      switch (cmd) {
        case Command::Add: return "add";
        case Command::And: return "and";
        case Command::Eq:  return "eq";
        case Command::Gt:  return "gt";
        case Command::Neg: return "neg";
        case Command::Lt:  return "lt";
        case Command::Not: return "not";
        case Command::Or:  return "or";
        case Command::Sub: return "sub";
      }
      return "";
    }

  public:
    /**
     * @brief Construct a VM writer bound to the given output file stream.
     * @param vmFile Open output stream for writing VM commands.
     */
    VmWriter(std::ofstream& vmFile);
    VmWriter& operator=(VmWriter&) = delete;
    VmWriter(VmWriter&) = delete;

    /** @brief Emit a `push segment index` command. */
    void writePush(Segment segment, uint32_t idx);
    
    /** @brief Emit a `pop segment index` command. */
    void writePop(Segment segment, uint32_t idx);

    /** @brief Emit a single arithmetic/logical VM command (e.g., `add`). */
    void writeArithmetic(Command cmd);

    /** @brief Emit a `label name` command. */
    void writeLabel(std::string_view label);

    /** @brief Emit a `goto name` command. */
    void writeGoto(std::string_view label);

    /** @brief Emit an `if-goto name` command. */
    void writeIf(std::string_view label);

    /** @brief Emit a `call name nArgs` command. */
    void writeCall(std::string_view name, uint32_t nArgs);
    
    /** @brief Emit a `function name nLocals` declaration. */
    void writeFunction(std::string_view name, uint32_t nArgs);

    /** @brief Emit a `return` command. */
    void writeReturn();

    /** @brief Close the underlying VM output stream. */
    void close();
};
