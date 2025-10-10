#pragma once

/**
 * @brief Classification of assembly instruction types.
 */
enum CommandType {
  A_COMMAND,  ///< Address or constant instruction (@xxx)
  C_COMMAND,  ///< Compute instruction (dest=comp;jump)
  L_COMMAND,  ///< Label pseudo-instruction (LABEL)
};
