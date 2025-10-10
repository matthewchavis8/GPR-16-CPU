#include "Modules/MainDriver/mainDriver.h"
#include <fstream>
#include <stdexcept>

int main(int argc, char* argv[]) {
  if (argc != 2)
    throw std::runtime_error("[LOG] Arg count does not match");

  std::ifstream asmFile(argv[1]);

  if (!asmFile.is_open())
    throw std::runtime_error("[LOG] Failed to open File");

  MainDriver mainDriver(asmFile, argv[1]);
  mainDriver.run();

  return 0;
}
