#pragma once

#include "ColorTokens.hpp"
#include "ElevationTokens.hpp"
#include "MotionTokens.hpp"
#include "ShapeTokens.hpp"
#include "StateLayerTokens.hpp"
#include "TypographyTokens.hpp"

namespace flx::ui::tokens {

/**
 * @brief Aggregate of all design token categories.
 *
 * A single `DesignTokens` instance describes a complete visual language.
 * `ThemeDefinition` (Phase 2) will embed one of these, and `StyleStore`
 * (Phase 1) will consume it to build pre-baked `lv_style_t` objects.
 *
 * Usage:
 * @code
 *   DesignTokens tokens;
 *   tokens.colors    = myColorTokens;
 *   tokens.typography = myTypographyTokens;
 *   tokens.shapes    = defaultShapeTokens();
 *   tokens.motion    = defaultMotionTokens();
 *   tokens.elevation = defaultElevationTokens();
 *   tokens.stateLayer = defaultStateLayerTokens();
 *
 *   StyleStore::getInstance().init(tokens);
 * @endcode
 */
struct DesignTokens {
	ColorTokens colors;
	TypographyTokens typography;
	ShapeTokens shapes;
	MotionTokens motion;
	ElevationTokens elevation;
	StateLayerTokens stateLayer;
};

} // namespace flx::ui::tokens
