#pragma once

#include "lvgl.h"
#include <cstdint>

namespace flx::ui::tokens {

/**
 * @brief Describes a single typographic style.
 *
 * Bundles a font reference with its letter-spacing and line-height so that
 * every text element can be styled with a single token lookup.
 */
struct TypeStyle {
	const lv_font_t* font; ///< LVGL font pointer (built-in or runtime-loaded)
	int32_t letterSpacing; ///< Letter spacing in 0.1 px units (LVGL convention)
	int32_t lineHeight; ///< Line height in px
};

/**
 * @brief Material 3 type scale — 15 style slots.
 *
 * Maps the full Material Design 3 type-scale hierarchy:
 *   display  (large / medium / small)
 *   headline (large / medium / small)
 *   title    (large / medium / small)
 *   body     (large / medium / small)
 *   label    (large / medium / small)
 *
 * On resource-constrained targets (ESP32) most slots will share
 * `LV_FONT_DEFAULT`; on desktop targets they can use distinct sizes.
 *
 * @see https://m3.material.io/styles/typography/type-scale-tokens
 */
struct TypographyTokens {
	// ── Display ──────────────────────────────────────────────────────
	TypeStyle displayLarge; ///< Hero text (57sp equiv.)
	TypeStyle displayMedium; ///< Large display (45sp equiv.)
	TypeStyle displaySmall; ///< Small display (36sp equiv.)

	// ── Headline ─────────────────────────────────────────────────────
	TypeStyle headlineLarge; ///< Page headline (32sp equiv.)
	TypeStyle headlineMedium; ///< Section headline (28sp equiv.)
	TypeStyle headlineSmall; ///< Sub-headline (24sp equiv.)

	// ── Title ────────────────────────────────────────────────────────
	TypeStyle titleLarge; ///< Top app bar (22sp equiv.)
	TypeStyle titleMedium; ///< Card / dialog title (16sp equiv.)
	TypeStyle titleSmall; ///< Tab label (14sp equiv.)

	// ── Body ─────────────────────────────────────────────────────────
	TypeStyle bodyLarge; ///< Primary body text (16sp equiv.)
	TypeStyle bodyMedium; ///< Default body text (14sp equiv.)
	TypeStyle bodySmall; ///< Supporting text (12sp equiv.)

	// ── Label ────────────────────────────────────────────────────────
	TypeStyle labelLarge; ///< Buttons, navigation (14sp equiv.)
	TypeStyle labelMedium; ///< Chips, badges (12sp equiv.)
	TypeStyle labelSmall; ///< Status bar, captions (11sp equiv.)
};

/**
 * @brief Create default typography tokens using built-in Montserrat fonts.
 *
 * Maps the 5 enabled Montserrat sizes (10, 12, 14, 16, 18) across the
 * Material 3 type scale:
 *
 *   18px → Display, Headline
 *   16px → Title
 *   14px → Body, Label Large
 *   12px → Label Medium
 *   10px → Label Small
 *
 * Letter spacing values follow Material 3 guidelines (in LVGL 0.1px units).
 */
inline TypographyTokens defaultTypographyTokens() {
	TypographyTokens t {};

	// ── Display ──────────────────────────────────────────────────────
	// On a small embedded display (240-320px), 18px is already "large"
	t.displayLarge = {&lv_font_montserrat_18, 0, 22};
	t.displayMedium = {&lv_font_montserrat_18, 0, 22};
	t.displaySmall = {&lv_font_montserrat_18, 0, 22};

	// ── Headline ─────────────────────────────────────────────────────
	t.headlineLarge = {&lv_font_montserrat_18, 0, 22};
	t.headlineMedium = {&lv_font_montserrat_16, 0, 20};
	t.headlineSmall = {&lv_font_montserrat_16, 0, 20};

	// ── Title ────────────────────────────────────────────────────────
	t.titleLarge = {&lv_font_montserrat_16, 0, 20};
	t.titleMedium = {&lv_font_montserrat_14, 1, 18}; // +0.1px letter spacing
	t.titleSmall = {&lv_font_montserrat_14, 1, 18};

	// ── Body ─────────────────────────────────────────────────────────
	t.bodyLarge = {&lv_font_montserrat_14, 0, 18};
	t.bodyMedium = {&lv_font_montserrat_14, 2, 18}; // +0.2px letter spacing
	t.bodySmall = {&lv_font_montserrat_12, 3, 16}; // +0.3px letter spacing

	// ── Label ────────────────────────────────────────────────────────
	t.labelLarge = {&lv_font_montserrat_14, 1, 18};
	t.labelMedium = {&lv_font_montserrat_12, 4, 16}; // +0.4px — tighter tracking
	t.labelSmall = {&lv_font_montserrat_10, 4, 14}; // +0.4px — captions/status

	return t;
}

} // namespace flx::ui::tokens
