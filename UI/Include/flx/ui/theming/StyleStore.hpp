#pragma once

#include "lvgl.h"
#include <flx/core/Singleton.hpp>
#include <flx/ui/theming/tokens/DesignTokens.hpp>

namespace flx::ui::theming {

/**
 * @brief Centralized registry of pre-built lv_style_t objects.
 *
 * StyleStore eliminates scattered `lv_obj_set_style_*()` calls by providing
 * a complete set of themed lv_style_t objects constructed from DesignTokens.
 *
 * Components apply styles via `lv_obj_add_style(obj, ss.statusBar(), 0)`
 * instead of manually setting individual style properties.
 *
 * Call `init()` once during startup, then `rebuild()` on theme changes.
 *
 * @code
 *   auto& ss = StyleStore::getInstance();
 *   ss.init(tokens);
 *
 *   // In StatusBar::create():
 *   lv_obj_add_style(m_statusBar, ss.statusBar(), 0);
 *
 *   // On theme change:
 *   ss.rebuild(newTokens);
 *   lv_obj_report_style_change(nullptr);
 * @endcode
 */
class StyleStore : public flx::Singleton<StyleStore> {
	friend class flx::Singleton<StyleStore>;

public:

	/// Initialize all styles from the given design tokens.
	void init(const tokens::DesignTokens& tokens);

	/// Rebuild all styles with new tokens (e.g., after theme switch).
	/// Call `lv_obj_report_style_change(nullptr)` after this.
	void rebuild(const tokens::DesignTokens& tokens);

	/// @return true if init() has been called.
	bool isInitialized() const { return m_initialized; }

	// ── Base Styles ──────────────────────────────────────────────────

	/// Glass panel (launcher, quick access, notification).
	lv_style_t* panel() { return &m_panel; }

	/// Card container (notification items, list entries).
	lv_style_t* card() { return &m_card; }

	/// Elevated card with shadow.
	lv_style_t* cardElevated() { return &m_cardElevated; }

	// ── Component Styles ─────────────────────────────────────────────

	/// Status bar root container.
	lv_style_t* statusBar() { return &m_statusBar; }

	/// Dock root container.
	lv_style_t* dock() { return &m_dock; }

	/// Window header (top app bar).
	lv_style_t* windowHeader() { return &m_windowHeader; }

	/// Window content area.
	lv_style_t* windowContent() { return &m_windowContent; }

	/// Window chrome (outer frame).
	lv_style_t* windowChrome() { return &m_windowChrome; }

	// ── Interactive State Styles ─────────────────────────────────────
	// Apply with LV_STATE_* selectors, e.g.:
	//   lv_obj_add_style(btn, ss.buttonDefault(), 0);
	//   lv_obj_add_style(btn, ss.buttonPressed(), LV_STATE_PRESSED);

	/// Button default state.
	lv_style_t* buttonDefault() { return &m_buttonDefault; }

	/// Button pressed state.
	lv_style_t* buttonPressed() { return &m_buttonPressed; }

	/// Button focused state (keyboard/encoder navigation).
	lv_style_t* buttonFocused() { return &m_buttonFocused; }

	/// Button disabled state.
	lv_style_t* buttonDisabled() { return &m_buttonDisabled; }

	// ── Toggle Styles ────────────────────────────────────────────────

	/// Circular toggle ON (checked) state.
	lv_style_t* toggleOn() { return &m_toggleOn; }

	/// Circular toggle OFF (unchecked) state.
	lv_style_t* toggleOff() { return &m_toggleOff; }

	// ── Typography Styles ────────────────────────────────────────────

	lv_style_t* textDisplayLarge() { return &m_textDisplayLarge; }
	lv_style_t* textDisplaySmall() { return &m_textDisplaySmall; }
	lv_style_t* textHeadlineMedium() { return &m_textHeadlineMedium; }
	lv_style_t* textHeadlineSmall() { return &m_textHeadlineSmall; }
	lv_style_t* textTitleLarge() { return &m_textTitleLarge; }
	lv_style_t* textTitleMedium() { return &m_textTitleMedium; }
	lv_style_t* textBodyLarge() { return &m_textBodyLarge; }
	lv_style_t* textBodyMedium() { return &m_textBodyMedium; }
	lv_style_t* textBodySmall() { return &m_textBodySmall; }
	lv_style_t* textLabelLarge() { return &m_textLabelLarge; }
	lv_style_t* textLabelMedium() { return &m_textLabelMedium; }
	lv_style_t* textLabelSmall() { return &m_textLabelSmall; }

	// ── Utility Styles ───────────────────────────────────────────────

	/// Transparent background (removes default container styling).
	lv_style_t* transparent() { return &m_transparent; }

	/// Invisible container with no borders, padding, or background.
	lv_style_t* invisibleContainer() { return &m_invisibleContainer; }

	// ── Tokens Access ────────────────────────────────────────────────

	/// Read-only access to the current design tokens.
	const tokens::DesignTokens& getTokens() const { return m_tokens; }

private:

	StyleStore() = default;
	~StyleStore() = default;

	void buildBaseStyles(const tokens::DesignTokens& t);
	void buildComponentStyles(const tokens::DesignTokens& t);
	void buildInteractiveStyles(const tokens::DesignTokens& t);
	void buildToggleStyles(const tokens::DesignTokens& t);
	void buildTypographyStyles(const tokens::DesignTokens& t);
	void buildUtilityStyles(const tokens::DesignTokens& t);

	/// Reset all lv_style_t objects (must be called before rebuilding).
	void resetAll();

	bool m_initialized = false;
	tokens::DesignTokens m_tokens {};

	// ── Style storage ────────────────────────────────────────────────
	// Base
	lv_style_t m_panel {};
	lv_style_t m_card {};
	lv_style_t m_cardElevated {};

	// Component
	lv_style_t m_statusBar {};
	lv_style_t m_dock {};
	lv_style_t m_windowHeader {};
	lv_style_t m_windowContent {};
	lv_style_t m_windowChrome {};

	// Interactive
	lv_style_t m_buttonDefault {};
	lv_style_t m_buttonPressed {};
	lv_style_t m_buttonFocused {};
	lv_style_t m_buttonDisabled {};

	// Toggle
	lv_style_t m_toggleOn {};
	lv_style_t m_toggleOff {};

	// Typography
	lv_style_t m_textDisplayLarge {};
	lv_style_t m_textDisplaySmall {};
	lv_style_t m_textHeadlineMedium {};
	lv_style_t m_textHeadlineSmall {};
	lv_style_t m_textTitleLarge {};
	lv_style_t m_textTitleMedium {};
	lv_style_t m_textBodyLarge {};
	lv_style_t m_textBodyMedium {};
	lv_style_t m_textBodySmall {};
	lv_style_t m_textLabelLarge {};
	lv_style_t m_textLabelMedium {};
	lv_style_t m_textLabelSmall {};

	// Utility
	lv_style_t m_transparent {};
	lv_style_t m_invisibleContainer {};
};

} // namespace flx::ui::theming
