#pragma once
#include <flx/ui/theming/ThemeDefinition.hpp>
#include <flx/ui/theming/tokens/DesignTokens.hpp>

namespace flx::ui::theming::themes {

/// Material Dark theme — Material 3 dark mode
inline ThemeDefinition materialDark() {
	using namespace tokens;
	ThemeDefinition def;
	def.name = "Material Dark";
	def.id = "material_dark";
	def.dark = true;

	auto& c = def.tokens.colors;
	c.primary = lv_palette_lighten(LV_PALETTE_INDIGO, 2);
	c.onPrimary = lv_palette_darken(LV_PALETTE_INDIGO, 4);
	c.primaryContainer = lv_palette_darken(LV_PALETTE_INDIGO, 2);
	c.onPrimaryContainer = lv_palette_lighten(LV_PALETTE_INDIGO, 4);

	c.secondary = lv_palette_lighten(LV_PALETTE_AMBER, 2);
	c.onSecondary = lv_palette_darken(LV_PALETTE_AMBER, 4);
	c.secondaryContainer = lv_palette_darken(LV_PALETTE_AMBER, 2);
	c.onSecondaryContainer = lv_palette_lighten(LV_PALETTE_AMBER, 4);

	c.tertiary = lv_palette_lighten(LV_PALETTE_TEAL, 2);
	c.onTertiary = lv_palette_darken(LV_PALETTE_TEAL, 4);
	c.tertiaryContainer = lv_palette_darken(LV_PALETTE_TEAL, 2);
	c.onTertiaryContainer = lv_palette_lighten(LV_PALETTE_TEAL, 4);

	c.surface = lv_color_hex(0x1C1B1F);
	c.surfaceDim = lv_color_hex(0x141218);
	c.surfaceBright = lv_color_hex(0x3B383E);
	c.surfaceContainerLowest = lv_color_hex(0x0F0D13);
	c.surfaceContainerLow = lv_color_hex(0x1D1B20);
	c.surfaceContainer = lv_color_hex(0x211F26);
	c.surfaceContainerHigh = lv_color_hex(0x2B2930);
	c.surfaceContainerHighest = lv_color_hex(0x36343B);
	c.onSurface = lv_color_hex(0xE6E1E5);
	c.onSurfaceVariant = lv_color_hex(0xCAC4D0);

	c.outline = lv_color_hex(0x938F99);
	c.outlineVariant = lv_color_hex(0x49454F);

	c.error = lv_palette_lighten(LV_PALETTE_RED, 2);
	c.onError = lv_palette_darken(LV_PALETTE_RED, 4);
	c.errorContainer = lv_palette_darken(LV_PALETTE_RED, 2);
	c.onErrorContainer = lv_palette_lighten(LV_PALETTE_RED, 4);

	c.inverseSurface = lv_color_hex(0xE6E1E5);
	c.inverseOnSurface = lv_color_hex(0x313033);
	c.inversePrimary = lv_palette_main(LV_PALETTE_INDIGO);

	c.scrim = lv_color_black();
	c.shadow = lv_color_black();
	c.background = lv_color_hex(0x1C1B1F);
	c.onBackground = lv_color_hex(0xE6E1E5);

	def.tokens.shapes = defaultShapeTokens();
	def.tokens.motion = defaultMotionTokens();
	def.tokens.elevation = defaultElevationTokens();
	def.tokens.stateLayer = defaultStateLayerTokens();

	return def;
}

} // namespace flx::ui::theming::themes
