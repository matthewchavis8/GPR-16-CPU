#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <utility>

class Code {
  private:
     static constexpr std::array<std::pair<std::string_view, uint8_t>, 25> m_comp_map {
       std::pair {"0",   0b0101010},
       std::pair {"1",   0b0111111},
       std::pair {"-1",  0b0111010},
       std::pair {"D",   0b0001100},
       std::pair {"A",   0b0110000},
       std::pair {"M",   0b1110000},
       std::pair {"!D",  0b0001101},
       std::pair {"!A",  0b0110001},
       std::pair {"!M",  0b1110001},
       std::pair {"D+1", 0b0011111},
       std::pair {"A+1", 0b0110111},
       std::pair {"M+1", 0b1110111},
       std::pair {"D-1", 0b0001110},
       std::pair {"A-1", 0b0110010},
       std::pair {"M-1", 0b1110010},
       std::pair {"D+A", 0b0000010},
       std::pair {"D+M", 0b1000010},
       std::pair {"D-A", 0b0010011},
       std::pair {"D-M", 0b1010011},
       std::pair {"A-D", 0b0000111},
       std::pair {"M-D", 0b1000111},
       std::pair {"D&A", 0b0000000},
       std::pair {"D&M", 0b1000000},
       std::pair {"D|A", 0b0010101},
       std::pair {"D|M", 0b1010101},
    };

    static constexpr std::array<std::pair<std::string_view, uint8_t>, 8> m_dest_map {
       std::pair {"M",   0b001},
       std::pair {"D",   0b010},
       std::pair {"MD",  0b011},
       std::pair {"A",   0b100},
       std::pair {"AM",  0b101},
       std::pair {"AD",  0b110},
       std::pair {"AMD", 0b111},
    };
    
    static constexpr std::array<std::pair<std::string_view, uint8_t>, 8> m_jump_map {
       std::pair {"JGT", 0b001},
       std::pair {"JEQ", 0b010},
       std::pair {"JGE", 0b011},
       std::pair {"JLT", 0b100},
       std::pair {"JNE", 0b101},
       std::pair {"JLE", 0b110},
       std::pair {"JMP", 0b111},
    };

   template <typename  T, std::size_t n>
   static constexpr uint8_t lookup (std::string_view key, const std::array<std::pair<std::string_view, T>, n>& map) noexcept {

    for (auto&& [mnemonic, bits] : map) {
      if (mnemonic == key)
        return bits;
    }

    return 0;
  }

  public:
    Code() = default;
    Code& operator=(const Code&) = delete;

    uint8_t dest(std::string_view mnemo) noexcept;
    uint8_t comp(std::string_view mnemo) noexcept;
    uint8_t jump(std::string_view mnemo) noexcept;
};
