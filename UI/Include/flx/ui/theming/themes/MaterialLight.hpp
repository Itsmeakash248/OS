#pragma once
#include <flx/ui/theming/ThemeDefinition.hpp>
#include <flx/ui/theming/tokens/DesignTokens.hpp>

namespace flx::ui::theming::themes {

/// Material Light theme — clean Material 3 light mode
inline ThemeDefinition materialLight() {
	using namespace tokens;
	ThemeDefinition def;
	def.name = "Material Light";
	def.id = "material_light";
	def.dark = false;

	auto& c = def.tokens.colors;
	c.primary = lv_palette_main(LV_PALETTE_INDIGO);
	c.onPrimary = lv_color_white();
	c.primaryContainer = lv_palette_lighten(LV_PALETTE_INDIGO, 4);
	c.onPrimaryContainer = lv_palette_darken(LV_PALETTE_INDIGO, 3);

	c.secondary = lv_palette_main(LV_PALETTE_AMBER);
	c.onSecondary = lv_color_black();
	c.secondaryContainer = lv_palette_lighten(LV_PALETTE_AMBER, 4);
	c.onSecondaryContainer = lv_palette_darken(LV_PALETTE_AMBER, 3);

	c.tertiary = lv_palette_main(LV_PALETTE_TEAL);
	c.onTertiary = lv_color_white();
	c.tertiaryContainer = lv_palette_lighten(LV_PALETTE_TEAL, 4);
	c.onTertiaryContainer = lv_palette_darken(LV_PALETTE_TEAL, 3);

	c.surface = lv_color_white();
	c.surfaceDim = lv_palette_lighten(LV_PALETTE_GREY, 4);
	c.surfaceBright = lv_color_white();
	c.surfaceContainerLowest = lv_color_white();
	c.surfaceContainerLow = lv_palette_lighten(LV_PALETTE_GREY, 5);
	c.surfaceContainer = lv_palette_lighten(LV_PALETTE_GREY, 4);
	c.surfaceContainerHigh = lv_palette_lighten(LV_PALETTE_GREY, 3);
	c.surfaceContainerHighest = lv_palette_lighten(LV_PALETTE_GREY, 2);
	c.onSurface = lv_palette_darken(LV_PALETTE_GREY, 4);
	c.onSurfaceVariant = lv_palette_main(LV_PALETTE_GREY);

	c.outline = lv_palette_main(LV_PALETTE_GREY);
	c.outlineVariant = lv_palette_lighten(LV_PALETTE_GREY, 3);

	c.error = lv_palette_main(LV_PALETTE_RED);
	c.onError = lv_color_white();
	c.errorContainer = lv_palette_lighten(LV_PALETTE_RED, 4);
	c.onErrorContainer = lv_palette_darken(LV_PALETTE_RED, 3);

	c.inverseSurface = lv_palette_darken(LV_PALETTE_GREY, 4);
	c.inverseOnSurface = lv_color_white();
	c.inversePrimary = lv_palette_lighten(LV_PALETTE_INDIGO, 3);

	c.scrim = lv_color_black();
	c.shadow = lv_color_black();
	c.background = lv_color_white();
	c.onBackground = lv_palette_darken(LV_PALETTE_GREY, 4);

	def.tokens.shapes = defaultShapeTokens();
	def.tokens.motion = defaultMotionTokens();
	def.tokens.elevation = defaultElevationTokens();
	def.tokens.stateLayer = defaultStateLayerTokens();

	return def;
}

} // namespace flx::ui::theming::themes
