#include "mainDriver.h"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <bitset>
#include <iostream>

MainDriver::MainDriver(std::ifstream& file, const std::string& file_name)
 : m_parser { file, file_name }
 , m_file_name { file_name }
 { };

void MainDriver::firstPass() {
  int romAddress {};

  while (m_parser.hasMoreCommands()) {
    m_parser.advance();

    switch (m_parser.commandType()) {

      case CommandType::L_COMMAND: {
        std::cout << "[LOG] L_command detected\n";
        std::string symbol = m_parser.symbol();

        if (!m_symbolTable.contains(symbol))
          m_symbolTable.addEntry(symbol, romAddress);

        break;
      }

      case CommandType::A_COMMAND:

      case CommandType::C_COMMAND:
        std::cout << "[LOG] C_command detected\n";
        ++romAddress;
        break;

      default:
        break;
    }
  }
}

void MainDriver::secondPass() {
  m_parser.reset();

  std::string hackFile { m_file_name.substr(0, m_file_name.find(".asm")) + ".hack" };
  std::ofstream outputFile(hackFile);

  if (!outputFile.is_open())
    throw std::runtime_error("[ERROR] Could not open output file\n");

  int nextRAMAddress { 16 };

  while (m_parser.hasMoreCommands()) {
    m_parser.advance();

    switch (m_parser.commandType()) {
      case CommandType::A_COMMAND: {
        std::cout << "[LOG] A_command Second pass detected\n";
        std::string symbol { m_parser.symbol()};
        int address {};

        if (std::all_of(symbol.begin(), symbol.end(), ::isdigit))
          address = std::stoi(symbol);
        else {
          if (!m_symbolTable.contains(symbol))
            m_symbolTable.addEntry(symbol, nextRAMAddress++);
          address = m_symbolTable.getAddress(symbol).value();
        }
        outputFile << '0' << std::bitset<15>(address).to_string() << '\n';
        break;
      }

      case CommandType::C_COMMAND: {
        std::cout << "[LOG] C_command Second pass detected\n";
        uint8_t compBits { m_code.comp(m_parser.comp()) };
        uint8_t destBits { m_code.dest(m_parser.dest()) };
        uint8_t jumpBits { m_code.jump(m_parser.jump()) };

        uint16_t instruction = (0b111 << 13)
                             | (compBits << 6)
                             | (destBits << 3)
                             | (jumpBits);
        outputFile << std::bitset<16>(instruction).to_string() << '\n';
        break;
      }

      default:
       break;

    }
  }
}

void MainDriver::run() {
  firstPass();
  std::cout << "[LOG] First pass was able to run\n";
  secondPass();
  std::cout << "[LOG] Second pass was able to run\n";
}
