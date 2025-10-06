#include "code.h"
#include <string_view>

constexpr uint16_t Code::comp(std::string_view mnemo) noexcept { return lookup(mnemo, m_comp_map); }

constexpr uint8_t Code::dest(std::string_view mnemo) noexcept { return lookup(mnemo, m_dest_map); }

constexpr uint8_t Code::jump(std::string_view mnemo) noexcept { return lookup(mnemo, m_jump_map); }
