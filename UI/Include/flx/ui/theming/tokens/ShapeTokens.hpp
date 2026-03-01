#pragma once

#include "draw/lv_draw_rect.h" // LV_RADIUS_CIRCLE
#include "lvgl.h"
#include <cstdint>

namespace flx::ui::tokens {

/**
 * @brief Material 3 shape tokens — corner radii per category.
 *
 * LVGL stores corner radii as `int32_t` (in pixels). The special value
 * `LV_RADIUS_CIRCLE` (0x7FFF) produces fully-rounded ends (pill shape).
 *
 * These tokens replace the scattered `RADIUS_*` constants in `UiConstants.hpp`
 * and give every component a consistent shape language.
 *
 * @see https://m3.material.io/styles/shape/overview
 */
struct ShapeTokens {
	int32_t none; ///< 0 dp  — square corners (images, full-bleed cards)
	int32_t extraSmall; ///< 4 dp  — small chips, badges
	int32_t small; ///< 8 dp  — text fields, filled cards
	int32_t medium; ///< 12 dp — elevated cards, dialogs
	int32_t large; ///< 16 dp — FABs, navigation bars
	int32_t extraLarge; ///< 28 dp — bottom sheets, modals
	int32_t full; ///< LV_RADIUS_CIRCLE — pill buttons, circular icons
};

/**
 * @brief Returns the default Material 3 shape token set.
 */
inline constexpr ShapeTokens defaultShapeTokens() {
	return ShapeTokens {
		.none = 0,
		.extraSmall = 4,
		.small = 8,
		.medium = 12,
		.large = 16,
		.extraLarge = 28,
		.full = LV_RADIUS_CIRCLE,
	};
}

} // namespace flx::ui::tokens
