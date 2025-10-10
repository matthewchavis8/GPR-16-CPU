#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <utility>

/**
 * @brief Translates assembly mnemonics into their binary machine code representations.
 *
 * Provides lookup tables and encoding methods for dest, comp, and jump fields
 * in C-instruction format.
 */
class Code {
  private:
     /** @brief Maps computation mnemonics to their 7-bit binary encodings. */
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

    /** @brief Maps destination mnemonics to their 3-bit binary encodings. */
    static constexpr std::array<std::pair<std::string_view, uint8_t>, 8> m_dest_map {
       std::pair {"M",   0b001},
       std::pair {"D",   0b010},
       std::pair {"MD",  0b011},
       std::pair {"A",   0b100},
       std::pair {"AM",  0b101},
       std::pair {"AD",  0b110},
       std::pair {"AMD", 0b111},
    };

    /** @brief Maps jump mnemonics to their 3-bit binary encodings. */
    static constexpr std::array<std::pair<std::string_view, uint8_t>, 8> m_jump_map {
       std::pair {"JGT", 0b001},
       std::pair {"JEQ", 0b010},
       std::pair {"JGE", 0b011},
       std::pair {"JLT", 0b100},
       std::pair {"JNE", 0b101},
       std::pair {"JLE", 0b110},
       std::pair {"JMP", 0b111},
    };

   /**
    * @brief Performs compile-time lookup of a mnemonic in a given encoding map.
    * @tparam T Value type stored in the map.
    * @tparam n Size of the map array.
    * @param key The mnemonic to search for.
    * @param map The encoding table to search.
    * @return The binary encoding, or 0 if not found.
    */
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

    /**
     * @brief Encodes a destination mnemonic into its 3-bit binary form.
     * @param mnemo The destination field (e.g., "D", "MD", "AMD").
     * @return The 3-bit encoding, or 0 if invalid.
     */
    uint8_t dest(std::string_view mnemo) noexcept;

    /**
     * @brief Encodes a computation mnemonic into its 7-bit binary form.
     * @param mnemo The computation field (e.g., "D+1", "M-D").
     * @return The 7-bit encoding, or 0 if invalid.
     */
    uint8_t comp(std::string_view mnemo) noexcept;

    /**
     * @brief Encodes a jump mnemonic into its 3-bit binary form.
     * @param mnemo The jump condition (e.g., "JGT", "JMP").
     * @return The 3-bit encoding, or 0 if invalid.
     */
    uint8_t jump(std::string_view mnemo) noexcept;
};
