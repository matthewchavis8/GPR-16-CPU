/** @file
 *  @brief Symbol table abstraction for Jack identifiers and their VM metadata.
 */
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include "../Utils/identifier.h"

/** @brief Metadata associated with a single Jack identifier. */
struct Symbol {
  std::string    type;  /**< Declared type (e.g., `int`, class name). */
  IdentifierKind kind;  /**< Kind / scope of the identifier (static, field, arg, var). */
  uint32_t       idx;   /**< Running index within its kind. */
};

/** @brief Convenience alias for the underlying symbol lookup table. */
using Table = std::unordered_map<std::string, Symbol>;

/**
 * @brief Maintains separate counts and mappings for static/field/arg/var symbols
 *        within a Jack class or subroutine scope.
 */
class SymbolTable {
  private:
    Table    m_table;      /**< Map from identifier name to symbol metadata. */

    uint32_t m_staticCnt{};
    uint32_t m_fieldCnt{};
    uint32_t m_argCnt{};
    uint32_t m_varCnt{};
    uint32_t m_Idx{};

  public:
    SymbolTable() = default;
    SymbolTable& operator=(const SymbolTable&) = delete;

    /** @brief Clear all symbols and reset kind counters to zero. */
    void reset();

    /**
     * @brief Define a new identifier in the table.
     * @param name Identifier name.
     * @param type Declared type.
     * @param kind Kind / scope of the identifier.
     */
    void define(std::string_view name, std::string_view type, IdentifierKind kind);

    /**
     * @brief Get the number of variables of the given kind currently defined.
     * @param kind Identifier kind.
     */
    uint32_t varCount(IdentifierKind kind) const;

    /**
     * @brief Look up the kind of the given identifier, or `IdentifierKind::None`
     *        if it is not present.
     */
    IdentifierKind kindOf(std::string_view name) const;

    /**
     * @brief Look up the declared type of the given identifier.
     * @throws std::runtime_error if the name is not present.
     */
    std::string typeOf(std::string_view name) const;

    /**
     * @brief Look up the index of the given identifier within its kind.
     * @throws std::runtime_error if the name is not present.
     */
    uint32_t indexOf(std::string_view name) const;
};
