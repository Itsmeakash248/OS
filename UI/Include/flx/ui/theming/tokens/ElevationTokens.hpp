#pragma once

#include "lvgl.h"
#include <cstdint>

namespace flx::ui::tokens {

/**
 * @brief Material 3 elevation tokens — shadow parameters per level.
 *
 * LVGL's shadow system uses width, Y-offset, spread, and opacity values.
 * These mirror the six Material 3 elevation levels (0–5) and are designed
 * so that `StyleStore` can apply them directly via `lv_style_set_shadow_*()`.
 *
 * On dark themes, shadows are less visible; consider pairing with tinted
 * surfaces from `ColorTokens` (surfaceContainer hierarchy) for visual depth.
 *
 * @see https://m3.material.io/styles/elevation/overview
 */
struct ElevationLevel {
	int32_t shadowWidth; ///< Shadow blur radius (px)
	int32_t shadowOffsetX; ///< Horizontal shadow offset (px)
	int32_t shadowOffsetY; ///< Vertical shadow offset (px)
	int32_t shadowSpread; ///< Shadow spread (px)
	lv_opa_t shadowOpa; ///< Shadow opacity (0–255)
};

struct ElevationTokens {
	ElevationLevel level0; ///< No elevation — flat surfaces
	ElevationLevel level1; ///< Cards at rest, search bar (1 dp equiv.)
	ElevationLevel level2; ///< Raised buttons, navigation drawer (3 dp equiv.)
	ElevationLevel level3; ///< FAB, navigation bar, top app bar (6 dp equiv.)
	ElevationLevel level4; ///< Modal bottom sheet, side sheet (8 dp equiv.)
	ElevationLevel level5; ///< Dialogs, full-screen modal (12 dp equiv.)
};

/**
 * @brief Returns the default elevation token set.
 *
 * Values are tuned for ~320×480 embedded displays. Larger displays
 * (desktop targets) may want proportionally larger values.
 */
inline constexpr ElevationTokens defaultElevationTokens() {
	return ElevationTokens {
		.level0 = {.shadowWidth = 0, .shadowOffsetX = 0, .shadowOffsetY = 0, .shadowSpread = 0, .shadowOpa = LV_OPA_TRANSP},
		.level1 = {.shadowWidth = 4, .shadowOffsetX = 0, .shadowOffsetY = 2, .shadowSpread = 0, .shadowOpa = LV_OPA_20},
		.level2 = {.shadowWidth = 8, .shadowOffsetX = 0, .shadowOffsetY = 4, .shadowSpread = 1, .shadowOpa = LV_OPA_20},
		.level3 = {.shadowWidth = 12, .shadowOffsetX = 0, .shadowOffsetY = 6, .shadowSpread = 2, .shadowOpa = LV_OPA_20},
		.level4 = {.shadowWidth = 16, .shadowOffsetX = 0, .shadowOffsetY = 8, .shadowSpread = 3, .shadowOpa = LV_OPA_20},
		.level5 = {.shadowWidth = 24, .shadowOffsetX = 0, .shadowOffsetY = 12, .shadowSpread = 4, .shadowOpa = LV_OPA_20},
	};
}

} // namespace flx::ui::tokens
