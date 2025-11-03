#include <stdexcept>
#include "Modules/VMTranslator/vmtranslator.h"

int main(int argc, char** argv) {
  if (argc < 2 || argc > 3)
    std::logic_error("[ERROR] Usage: VmTranslator <input.vm | directory> [output.asm]\n");

  VMTranslator translator;
  translator.translate(argv[1], argc == 3 ? argv[2] : "");

  return 0;
}
