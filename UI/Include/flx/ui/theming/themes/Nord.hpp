#pragma once
#include <flx/ui/theming/ThemeDefinition.hpp>
#include <flx/ui/theming/tokens/DesignTokens.hpp>

namespace flx::ui::theming::themes {

/// Nord theme — Minimal Scandinavian palette
/// https://www.nordtheme.com
inline ThemeDefinition nord() {
	using namespace tokens;
	ThemeDefinition def;
	def.name = "Nord";
	def.id = "nord";
	def.dark = true;

	auto& c = def.tokens.colors;
	// Nord Frost for accents
	c.primary = lv_color_hex(0x88C0D0); // nord8
	c.onPrimary = lv_color_hex(0x2E3440); // nord0
	c.primaryContainer = lv_color_hex(0x5E81AC); // nord10
	c.onPrimaryContainer = lv_color_hex(0xECEFF4); // nord6

	c.secondary = lv_color_hex(0x81A1C1); // nord9
	c.onSecondary = lv_color_hex(0x2E3440);
	c.secondaryContainer = lv_color_hex(0x434C5E); // nord2
	c.onSecondaryContainer = lv_color_hex(0xD8DEE9); // nord4

	c.tertiary = lv_color_hex(0xA3BE8C); // nord14 green
	c.onTertiary = lv_color_hex(0x2E3440);
	c.tertiaryContainer = lv_color_hex(0x3B4252); // nord1
	c.onTertiaryContainer = lv_color_hex(0xA3BE8C);

	// Nord Polar Night for surfaces
	c.surface = lv_color_hex(0x3B4252); // nord1
	c.surfaceDim = lv_color_hex(0x2E3440); // nord0
	c.surfaceBright = lv_color_hex(0x4C566A); // nord3
	c.surfaceContainerLowest = lv_color_hex(0x242933);
	c.surfaceContainerLow = lv_color_hex(0x2E3440); // nord0
	c.surfaceContainer = lv_color_hex(0x3B4252); // nord1
	c.surfaceContainerHigh = lv_color_hex(0x434C5E); // nord2
	c.surfaceContainerHighest = lv_color_hex(0x4C566A); // nord3
	c.onSurface = lv_color_hex(0xECEFF4); // nord6
	c.onSurfaceVariant = lv_color_hex(0xD8DEE9); // nord4

	c.outline = lv_color_hex(0x4C566A); // nord3
	c.outlineVariant = lv_color_hex(0x434C5E); // nord2

	// Nord Aurora red for error
	c.error = lv_color_hex(0xBF616A); // nord11
	c.onError = lv_color_hex(0x2E3440);
	c.errorContainer = lv_color_hex(0x3B4252);
	c.onErrorContainer = lv_color_hex(0xBF616A);

	c.inverseSurface = lv_color_hex(0xECEFF4); // nord6
	c.inverseOnSurface = lv_color_hex(0x2E3440); // nord0
	c.inversePrimary = lv_color_hex(0x5E81AC); // nord10

	c.scrim = lv_color_hex(0x2E3440);
	c.shadow = lv_color_black();
	c.background = lv_color_hex(0x2E3440); // nord0
	c.onBackground = lv_color_hex(0xECEFF4); // nord6

	def.tokens.shapes = defaultShapeTokens();
	def.tokens.motion = defaultMotionTokens();
	def.tokens.elevation = defaultElevationTokens();
	def.tokens.stateLayer = defaultStateLayerTokens();

	return def;
}

} // namespace flx::ui::theming::themes
