#pragma once
#include "../themes/Themes.hpp"
#include "lvgl.h"

namespace flx::ui::theming {
class ThemeRegistry;
}

class ThemeEngine {
public:

	static void init();
	static void set_theme(ThemeType theme, lv_display_t* disp = nullptr);
	static ThemeType get_current_theme();
	static void cycle_theme();

	/// Access the shared ThemeRegistry (for registering user themes, etc.)
	static flx::ui::theming::ThemeRegistry& getRegistry();

private:

	static ThemeType current_theme;
	static void apply_theme(ThemeType theme, lv_display_t* disp);

	static void cleanup_previous_theme(lv_display_t* disp);
};
