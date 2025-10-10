#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

/**
 * @brief Maps symbolic labels to their corresponding memory addresses.
 *
 * Maintains the symbol-to-address bindings discovered during assembly.
 */
class SymbolTable {
  private:
    std::unordered_map<std::string, uint16_t> m_hashTable;
  public:
    SymbolTable() = default;
    SymbolTable& operator=(const SymbolTable&) = delete;

    /**
     * @brief Registers a new symbol with its associated memory address.
     * @param symbol The label or identifier to register.
     * @param address The 16-bit memory address for this symbol.
     */
    void addEntry(std::string_view symbol, uint16_t address);

    /**
     * @brief Checks whether a symbol exists in the table.
     * @param symbol The label to query.
     * @return True if the symbol is registered, false otherwise.
     */
    bool contains(std::string_view symbol) const;

    /**
     * @brief Retrieves the address bound to a symbol.
     * @param symbol The label to look up.
     * @return The associated address, or std::nullopt if not found.
     */
    std::optional<uint16_t> getAddress(std::string_view symbol) const;
};
