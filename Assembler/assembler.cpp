#include "Modules/Parser/parser.h"
#include "Modules/Code/code.h"
#include "Modules/Utils/commandType.h"
#include <cstdint>
#include <fstream>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <iostream>

int main(int argc, char* argv[]) {

  if (argc < 2)
    throw std::runtime_error("[ERROR] arg count must be atleast 2");


  std::ifstream file { argv[1] };
  std::string file_name { argv[1] };

  auto parser = std::make_unique<Parser>(file, file_name);
  auto code = std::make_unique<Code>();


  std::ostringstream hackFile;

  std::cout << "Does the parser have any commands" << parser->hasMoreCommands() << '\n';
  std::cout << "Does the parser buffer initalized" << parser->getLookAheadBuffer() << '\n';
  auto to_bits = [](std::uint8_t value, int bits) {
      std::string s(bits, '0');
      for (int i = bits - 1; i >= 0; --i)
          s[bits - 1 - i] = ((value >> i) & 1u) ? '1' : '0';
      return s;
  };
  while (parser->hasMoreCommands()) {
    parser->advance();
    switch (parser->commandType()) {
      case CommandType::A_COMMAND: {
        break;
      }
      case CommandType::C_COMMAND: {
        std::uint8_t dest = code->dest(parser->dest());
        std::uint8_t comp = code->comp(parser->comp());
        std::uint8_t jump = code->jump(parser->jump());

        hackFile << to_bits(dest, 3) << to_bits(comp, 7) 
          << to_bits(jump, 3) << '\n';
        break;
      }
      case CommandType::L_COMMAND: {
        break;
      }
      default:
        break;
    }
  };

  std::cout << hackFile.str() << '\n';
  
  return 0;
}
