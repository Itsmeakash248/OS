#pragma once

#include "lvgl.h"

namespace flx::ui::tokens {

/**
 * @brief Material 3 state-layer tokens — interaction opacity overlays.
 *
 * State layers are semi-transparent overlays applied on top of a component's
 * container to communicate interactive state. The color of the overlay is
 * typically `onSurface` (or the component's `on*` color) at the opacity
 * specified here.
 *
 * In LVGL these map to `lv_style_set_bg_opa()` on state-specific style
 * objects (e.g., `LV_STATE_PRESSED`, `LV_STATE_FOCUSED`).
 *
 * @see https://m3.material.io/foundations/interaction/states/overview
 */
struct StateLayerTokens {
	lv_opa_t hover; ///< 8% — pointer hovering over element
	lv_opa_t focus; ///< 10% — element has keyboard/encoder focus
	lv_opa_t pressed; ///< 10% — element is actively pressed
	lv_opa_t dragged; ///< 16% — element is being dragged
	lv_opa_t disabledContainer; ///< 12% — disabled container background
	lv_opa_t disabledContent; ///< 38% — disabled content (text, icons)
};

/**
 * @brief Returns the default Material 3 state-layer token set.
 */
inline constexpr StateLayerTokens defaultStateLayerTokens() {
	return StateLayerTokens {
		.hover = static_cast<lv_opa_t>(255 * 0.08f), // ~20
		.focus = static_cast<lv_opa_t>(255 * 0.10f), // ~26
		.pressed = static_cast<lv_opa_t>(255 * 0.10f), // ~26
		.dragged = static_cast<lv_opa_t>(255 * 0.16f), // ~41
		.disabledContainer = static_cast<lv_opa_t>(255 * 0.12f), // ~31
		.disabledContent = static_cast<lv_opa_t>(255 * 0.38f), // ~97
	};
}

} // namespace flx::ui::tokens
