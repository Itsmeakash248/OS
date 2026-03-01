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

} // namespace flx::ui::tokens
