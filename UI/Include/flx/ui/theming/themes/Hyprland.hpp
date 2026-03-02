#pragma once
#include <flx/ui/theming/ThemeDefinition.hpp>
#include <flx/ui/theming/tokens/DesignTokens.hpp>

namespace flx::ui::theming::themes {

/// Hyprland theme — Linux rice aesthetic (cyan/pink on dark grey)
inline ThemeDefinition hyprland() {
	using namespace tokens;
	ThemeDefinition def;
	def.name = "Hyprland";
	def.id = "hyprland";
	def.dark = true;

	auto& c = def.tokens.colors;
	c.primary = lv_palette_main(LV_PALETTE_CYAN);
	c.onPrimary = lv_color_white();
	c.primaryContainer = lv_palette_darken(LV_PALETTE_CYAN, 2);
	c.onPrimaryContainer = lv_palette_lighten(LV_PALETTE_CYAN, 4);

	c.secondary = lv_palette_main(LV_PALETTE_PINK);
	c.onSecondary = lv_color_white();
	c.secondaryContainer = lv_palette_darken(LV_PALETTE_PINK, 2);
	c.onSecondaryContainer = lv_palette_lighten(LV_PALETTE_PINK, 4);

	c.tertiary = lv_palette_main(LV_PALETTE_PURPLE);
	c.onTertiary = lv_color_white();
	c.tertiaryContainer = lv_palette_darken(LV_PALETTE_PURPLE, 2);
	c.onTertiaryContainer = lv_palette_lighten(LV_PALETTE_PURPLE, 4);

	c.surface = lv_palette_darken(LV_PALETTE_GREY, 4);
	c.surfaceDim = lv_color_hex(0x1A1A2E);
	c.surfaceBright = lv_color_hex(0x3A3A4E);
	c.surfaceContainerLowest = lv_color_hex(0x121226);
	c.surfaceContainerLow = lv_color_hex(0x1E1E32);
	c.surfaceContainer = lv_palette_darken(LV_PALETTE_GREY, 4);
	c.surfaceContainerHigh = lv_color_hex(0x2A2A3E);
	c.surfaceContainerHighest = lv_color_hex(0x343448);
	c.onSurface = lv_color_white();
	c.onSurfaceVariant = lv_palette_lighten(LV_PALETTE_GREY, 2);

	c.outline = lv_palette_main(LV_PALETTE_GREY);
	c.outlineVariant = lv_palette_darken(LV_PALETTE_GREY, 2);

	c.error = lv_palette_main(LV_PALETTE_RED);
	c.onError = lv_color_white();
	c.errorContainer = lv_palette_darken(LV_PALETTE_RED, 3);
	c.onErrorContainer = lv_palette_lighten(LV_PALETTE_RED, 3);

	c.inverseSurface = lv_color_white();
	c.inverseOnSurface = lv_palette_darken(LV_PALETTE_GREY, 4);
	c.inversePrimary = lv_palette_darken(LV_PALETTE_CYAN, 2);

	c.scrim = lv_color_black();
	c.shadow = lv_color_black();
	c.background = lv_color_hex(0x11111B);
	c.onBackground = lv_color_white();

	def.tokens.typography = defaultTypographyTokens();
	def.tokens.shapes = defaultShapeTokens();
	def.tokens.motion = defaultMotionTokens();
	def.tokens.elevation = defaultElevationTokens();
	def.tokens.stateLayer = defaultStateLayerTokens();

	return def;
}

} // namespace flx::ui::theming::themes
