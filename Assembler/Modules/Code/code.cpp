#include "code.h"
#include <cstdint>
#include <string_view>

uint8_t Code::comp(std::string_view mnemo) noexcept { return lookup(mnemo, m_comp_map); }

uint8_t Code::dest(std::string_view mnemo) noexcept { return lookup(mnemo, m_dest_map); }

uint8_t Code::jump(std::string_view mnemo) noexcept { return lookup(mnemo, m_jump_map); }
