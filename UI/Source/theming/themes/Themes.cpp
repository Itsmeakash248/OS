#include "core/lv_obj.h"
#include "misc/lv_types.h"
#include "widgets/textarea/lv_textarea.h"
#include <flx/core/Logger.hpp>
#include <flx/ui/keyboard/VirtualKeyboard.hpp>
#include <flx/ui/theming/ThemeRegistry.hpp>
#include <flx/ui/theming/themes/Themes.hpp>
#include <string_view>

static constexpr std::string_view TAG = "Themes";

// Shared registry instance (populated by ThemeEngine::init)
static flx::ui::theming::ThemeRegistry* s_registry = nullptr;

namespace Themes {

void setRegistry(flx::ui::theming::ThemeRegistry* reg) {
	s_registry = reg;
}

ThemeConfig GetConfig(ThemeType type) {
	ThemeConfig cfg {};
	cfg.apply_cb = nullptr;

	if (s_registry) {
		auto idx = static_cast<size_t>(static_cast<int32_t>(type));
		const auto* def = s_registry->getByIndex(idx);
		if (def) {
			const auto& c = def->tokens.colors;
			cfg.primary = c.primary;
			cfg.secondary = c.secondary;
			cfg.surface = c.surface;
			cfg.on_primary = c.onPrimary;
			cfg.text_primary = c.onSurface;
			cfg.text_secondary = c.onSurfaceVariant;
			cfg.error = c.error;
			cfg.dark = def->dark;
			return cfg;
		}
	}

	// Fallback: hardcoded defaults (shouldn't happen after init)
	Log::warn(TAG, "GetConfig fallback: registry not set or theme index %d not found", (int)type);
	cfg.primary = lv_palette_main(LV_PALETTE_CYAN);
	cfg.secondary = lv_palette_main(LV_PALETTE_PINK);
	cfg.surface = lv_palette_darken(LV_PALETTE_GREY, 4);
	cfg.on_primary = lv_color_white();
	cfg.text_primary = lv_color_white();
	cfg.text_secondary = lv_palette_lighten(LV_PALETTE_GREY, 2);
	cfg.error = lv_palette_main(LV_PALETTE_RED);
	cfg.dark = true;
	return cfg;
}

const char* ToString(ThemeType type) {
	if (s_registry) {
		auto idx = static_cast<size_t>(static_cast<int32_t>(type));
		const auto* def = s_registry->getByIndex(idx);
		if (def) return def->name.c_str();
	}
	return "Unknown";
}

void ApplyGlobal(lv_theme_t* th, lv_obj_t* obj) {
	LV_UNUSED(th);

	if (lv_obj_check_type(obj, &lv_textarea_class)) {
		Log::debug(TAG, "Registering textarea for virtual keyboard");
		VirtualKeyboard::getInstance().register_input_area(obj);
	}
}

} // namespace Themes
