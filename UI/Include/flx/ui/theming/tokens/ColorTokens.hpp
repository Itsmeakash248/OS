#pragma once

#include "lvgl.h"

namespace flx::ui::tokens {

/**
 * @brief Material 3 semantic color roles.
 *
 * Maps to the full Material Design 3 color system, providing 35+ semantic
 * color slots. Every UI component should reference these roles rather than
 * hard-coding palette colors, enabling full theme portability.
 *
 * @see https://m3.material.io/styles/color/roles
 */
struct ColorTokens {
    // ── Primary ──────────────────────────────────────────────────────
    lv_color_t primary;              ///< Key color used for prominent elements (FABs, active states)
    lv_color_t onPrimary;            ///< Content color on top of primary
    lv_color_t primaryContainer;     ///< Standout container for key components
    lv_color_t onPrimaryContainer;   ///< Content color on top of primaryContainer

    // ── Secondary ────────────────────────────────────────────────────
    lv_color_t secondary;            ///< Accent for less prominent elements (chips, toggles)
    lv_color_t onSecondary;          ///< Content color on top of secondary
    lv_color_t secondaryContainer;   ///< Container for secondary elements
    lv_color_t onSecondaryContainer; ///< Content color on top of secondaryContainer

    // ── Tertiary ─────────────────────────────────────────────────────
    lv_color_t tertiary;             ///< Contrasting accent to balance primary/secondary
    lv_color_t onTertiary;           ///< Content color on top of tertiary
    lv_color_t tertiaryContainer;    ///< Container for tertiary elements
    lv_color_t onTertiaryContainer;  ///< Content color on top of tertiaryContainer

    // ── Surface ──────────────────────────────────────────────────────
    lv_color_t surface;              ///< Default background for cards, sheets, menus
    lv_color_t surfaceDim;           ///< Dimmer variant of surface (scrollable areas)
    lv_color_t surfaceBright;        ///< Brighter variant of surface (elevated areas)
    lv_color_t surfaceContainerLowest;  ///< Lowest-emphasis container
    lv_color_t surfaceContainerLow;     ///< Low-emphasis container
    lv_color_t surfaceContainer;        ///< Default container
    lv_color_t surfaceContainerHigh;    ///< High-emphasis container (cards, tiles)
    lv_color_t surfaceContainerHighest; ///< Highest-emphasis container (text fields)
    lv_color_t onSurface;            ///< Content color on surface backgrounds
    lv_color_t onSurfaceVariant;     ///< Secondary content color (icons, captions)

    // ── Outline ──────────────────────────────────────────────────────
    lv_color_t outline;              ///< Important boundaries (text fields, dividers)
    lv_color_t outlineVariant;       ///< Decorative borders (cards, sections)

    // ── Error ────────────────────────────────────────────────────────
    lv_color_t error;                ///< Error state color
    lv_color_t onError;              ///< Content color on error
    lv_color_t errorContainer;       ///< Background for error-related content
    lv_color_t onErrorContainer;     ///< Content color on error container

    // ── Inverse ──────────────────────────────────────────────────────
    lv_color_t inverseSurface;       ///< Surface for snackbars, tooltips (contrasting)
    lv_color_t inverseOnSurface;     ///< Content on inverse surface
    lv_color_t inversePrimary;       ///< Primary on inverse surface

    // ── Fixed ────────────────────────────────────────────────────────
    lv_color_t scrim;                ///< Overlay behind modal dialogs
    lv_color_t shadow;               ///< Shadow color (typically near-black)

    // ── Background ───────────────────────────────────────────────────
    lv_color_t background;           ///< Top-level background (behind all content)
    lv_color_t onBackground;         ///< Content color on top of background
};

} // namespace flx::ui::tokens
