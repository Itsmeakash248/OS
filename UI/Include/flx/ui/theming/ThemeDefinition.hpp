#pragma once

#include <flx/ui/theming/tokens/DesignTokens.hpp>
#include <string>

namespace flx::ui::theming {

/**
 * @brief Complete theme definition — tokens + metadata.
 *
 * Each built-in theme is a constexpr-populated `ThemeDefinition`.
 * User themes (loaded from SD JSON) populate the same struct.
 * The ThemeEngine feeds `definition.tokens` into `StyleStore`.
 */
struct ThemeDefinition {
	std::string name; ///< Human-readable name ("Hyprland", "Nord", ...)
	std::string id; ///< Machine id ("hyprland", "nord", "custom_nord", ...)
	bool dark = true; ///< Dark mode flag — drives LVGL default theme
	tokens::DesignTokens tokens {}; ///< Full design token set
};

} // namespace flx::ui::theming
