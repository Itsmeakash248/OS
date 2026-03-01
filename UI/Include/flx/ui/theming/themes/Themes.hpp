#pragma once
#include "lvgl.h"
#include <cstdint>

/**
 * @brief Theme index — matches registration order in ThemeRegistry::registerBuiltins().
 *
 * Legacy: Previously an enum class with only 2 values. Now expanded to 4 themes
 * and kept as an integer alias so observer subjects (int32_t) continue to work.
 */
enum class ThemeType : int32_t {
	HYPRLAND = 0,
	MATERIAL_LIGHT = 1,
	MATERIAL_DARK = 2,
	NORD = 3,

	// Legacy alias: "MATERIAL" mapped to MATERIAL_LIGHT for backward compat
	MATERIAL = MATERIAL_LIGHT,
};

static constexpr int32_t THEME_COUNT = 4;

/**
 * @brief [DEPRECATED] Legacy ThemeConfig — kept for backward compatibility with
 * apply_glass() and FocusManager.
 *
 * @deprecated New code should use ThemeDefinition.tokens.colors directly via
 * StyleStore::getTokens(). This struct will be removed once all consumers
 * are migrated to the new token-based system.
 */
struct ThemeConfig {
	lv_color_t primary;
	lv_color_t secondary;
	lv_color_t surface;
	lv_color_t on_primary;
	lv_color_t text_primary;
	lv_color_t text_secondary;
	lv_color_t error;
	bool dark;
	lv_theme_apply_cb_t apply_cb;
};

namespace flx::ui::theming {
class ThemeRegistry;
}

namespace Themes {
void setRegistry(flx::ui::theming::ThemeRegistry* reg);
ThemeConfig GetConfig(ThemeType type);
const char* ToString(ThemeType type);
void ApplyGlobal(lv_theme_t* th, lv_obj_t* obj);
} // namespace Themes
