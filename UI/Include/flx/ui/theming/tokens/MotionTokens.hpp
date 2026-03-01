#pragma once

#include "lvgl.h"
#include "misc/lv_anim.h"
#include <cstdint>

namespace flx::ui::tokens {

/**
 * @brief Material 3 motion tokens — durations and easing curves.
 *
 * Duration slots follow a linear 50 ms step from short1 (50 ms) through
 * long2 (500 ms). Easing curves are stored as `lv_anim_bezier3_para_t`,
 * ready to be applied to `lv_anim_t::parameter.bezier3` when using the
 * `lv_anim_path_custom_bezier3` path callback.
 *
 * @see https://m3.material.io/styles/motion/overview
 */
struct MotionTokens {
	// ── Durations (ms) ───────────────────────────────────────────────
	uint32_t durationShort1; ///<  50 ms — micro-feedback (ripples)
	uint32_t durationShort2; ///< 100 ms — icon state changes
	uint32_t durationShort3; ///< 150 ms — small element transitions
	uint32_t durationShort4; ///< 200 ms — button press/release
	uint32_t durationMedium1; ///< 250 ms — card expand/collapse
	uint32_t durationMedium2; ///< 300 ms — panel slide, theme switch
	uint32_t durationMedium3; ///< 350 ms — page transition
	uint32_t durationMedium4; ///< 400 ms — complex transforms
	uint32_t durationLong1; ///< 450 ms — bottom sheet reveal
	uint32_t durationLong2; ///< 500 ms — full-screen transition

	// ── Easing curves (LVGL cubic-bezier params) ─────────────────────
	lv_anim_bezier3_para_t easingStandard; ///< (0.2, 0.0, 0.0, 1.0)  — general use
	lv_anim_bezier3_para_t easingStandardDecel; ///< (0.0, 0.0, 0.0, 1.0)  — enter/appear
	lv_anim_bezier3_para_t easingStandardAccel; ///< (0.3, 0.0, 1.0, 1.0)  — exit/disappear
	lv_anim_bezier3_para_t easingEmphasized; ///< (0.2, 0.0, 0.0, 1.0)  — emphasis (≈ standard)
	lv_anim_bezier3_para_t easingEmphasizedDecel; ///< (0.05, 0.7, 0.1, 1.0) — dramatic enter
	lv_anim_bezier3_para_t easingEmphasizedAccel; ///< (0.3, 0.0, 0.8, 0.15) — dramatic exit
};

/**
 * @brief Returns the default Material 3 motion token set.
 *
 * Bezier control points are converted via `LV_BEZIER_VAL_FLOAT` which maps
 * floating-point control points to LVGL's fixed-point int16_t representation.
 */
inline MotionTokens defaultMotionTokens() {
	return MotionTokens {
		// Durations
		.durationShort1 = 50,
		.durationShort2 = 100,
		.durationShort3 = 150,
		.durationShort4 = 200,
		.durationMedium1 = 250,
		.durationMedium2 = 300,
		.durationMedium3 = 350,
		.durationMedium4 = 400,
		.durationLong1 = 450,
		.durationLong2 = 500,

		// Easing curves
		.easingStandard = {
			LV_BEZIER_VAL_FLOAT(0.2f), LV_BEZIER_VAL_FLOAT(0.0f),
			LV_BEZIER_VAL_FLOAT(0.0f), LV_BEZIER_VAL_FLOAT(1.0f)
		},
		.easingStandardDecel = {LV_BEZIER_VAL_FLOAT(0.0f), LV_BEZIER_VAL_FLOAT(0.0f), LV_BEZIER_VAL_FLOAT(0.0f), LV_BEZIER_VAL_FLOAT(1.0f)},
		.easingStandardAccel = {LV_BEZIER_VAL_FLOAT(0.3f), LV_BEZIER_VAL_FLOAT(0.0f), LV_BEZIER_VAL_FLOAT(1.0f), LV_BEZIER_VAL_FLOAT(1.0f)},
		.easingEmphasized = {LV_BEZIER_VAL_FLOAT(0.2f), LV_BEZIER_VAL_FLOAT(0.0f), LV_BEZIER_VAL_FLOAT(0.0f), LV_BEZIER_VAL_FLOAT(1.0f)},
		.easingEmphasizedDecel = {LV_BEZIER_VAL_FLOAT(0.05f), LV_BEZIER_VAL_FLOAT(0.7f), LV_BEZIER_VAL_FLOAT(0.1f), LV_BEZIER_VAL_FLOAT(1.0f)},
		.easingEmphasizedAccel = {LV_BEZIER_VAL_FLOAT(0.3f), LV_BEZIER_VAL_FLOAT(0.0f), LV_BEZIER_VAL_FLOAT(0.8f), LV_BEZIER_VAL_FLOAT(0.15f)},
	};
}

} // namespace flx::ui::tokens
