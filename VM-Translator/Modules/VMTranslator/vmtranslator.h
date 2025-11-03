#pragma once

#include <string>
#include <vector>
#include "../CodeWriter/codeWriter.h"


class VMTranslator {
public:

  /**
   * @brief Translate a single .vm file or all .vm files in a directory.
   * @param inPath      Path to input .vm file or directory containing .vm files.
   * @param outAsmPath  Optional explicit output .asm path. If empty, a path is computed:
   *                    - input.vm  -> input.asm
   *                    - Dir/      -> Dir/Dir.asm
   * @throws std::runtime_error on invalid paths or I/O errors.
   */
  void translate(const std::string& inPath, const std::string& outAsmPath = "");

private:

  /// Collect all .vm files from a file-or-directory input (sorted for deterministic output).
  std::vector<std::string> collectVmFiles(const std::string& inPath) const;

  /// Compute default output .asm path based on input path.
  std::string computeOutAsm(const std::string& inPath) const;

  /// Translate a single .vm file using the provided CodeWriter (which owns the open .asm stream).
  void translateFile(const std::string& vmPath, CodeWriter& cw) const;
};
