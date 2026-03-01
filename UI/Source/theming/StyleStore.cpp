#include <flx/core/Logger.hpp>
#include <flx/ui/theming/StyleStore.hpp>
#include <string_view>

static constexpr std::string_view TAG = "StyleStore";

namespace flx::ui::theming {

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

void StyleStore::init(const tokens::DesignTokens& tokens) {
	if (m_initialized) {
		Log::warn(TAG, "StyleStore already initialized — call rebuild() instead");
		return;
	}

	Log::info(TAG, "Initializing StyleStore...");
	m_tokens = tokens;

	buildBaseStyles(tokens);
	buildComponentStyles(tokens);
	buildSubComponentStyles(tokens);
	buildInteractiveStyles(tokens);
	buildToggleStyles(tokens);
	buildTypographyStyles(tokens);
	buildUtilityStyles(tokens);

	m_initialized = true;
	Log::info(TAG, "StyleStore initialized successfully");
}

void StyleStore::rebuild(const tokens::DesignTokens& tokens) {
	Log::info(TAG, "Rebuilding StyleStore with new tokens...");
	resetAll();
	m_tokens = tokens;

	buildBaseStyles(tokens);
	buildComponentStyles(tokens);
	buildSubComponentStyles(tokens);
	buildInteractiveStyles(tokens);
	buildToggleStyles(tokens);
	buildTypographyStyles(tokens);
	buildUtilityStyles(tokens);

	Log::info(TAG, "StyleStore rebuilt successfully");
}

// ─────────────────────────────────────────────────────────────────────────────
// Reset
// ─────────────────────────────────────────────────────────────────────────────

void StyleStore::resetAll() {
	// lv_style_reset() clears all properties from a style, making it safe
	// to re-initialize with new values.
	lv_style_reset(&m_panel);
	lv_style_reset(&m_card);
	lv_style_reset(&m_cardElevated);

	lv_style_reset(&m_statusBar);
	lv_style_reset(&m_dock);
	lv_style_reset(&m_windowHeader);
	lv_style_reset(&m_windowContent);
	lv_style_reset(&m_windowChrome);

	lv_style_reset(&m_windowContainer);
	lv_style_reset(&m_dockButton);
	lv_style_reset(&m_dockAppContainer);
	lv_style_reset(&m_panelHeader);
	lv_style_reset(&m_panelIconButton);
	lv_style_reset(&m_windowHeaderButton);
	lv_style_reset(&m_screen);
	lv_style_reset(&m_wallpaper);
	lv_style_reset(&m_wallpaperImage);
	lv_style_reset(&m_overlayLabel);

	lv_style_reset(&m_buttonDefault);
	lv_style_reset(&m_buttonPressed);
	lv_style_reset(&m_buttonFocused);
	lv_style_reset(&m_buttonDisabled);

	lv_style_reset(&m_toggleOn);
	lv_style_reset(&m_toggleOff);

	lv_style_reset(&m_textDisplayLarge);
	lv_style_reset(&m_textDisplaySmall);
	lv_style_reset(&m_textHeadlineMedium);
	lv_style_reset(&m_textHeadlineSmall);
	lv_style_reset(&m_textTitleLarge);
	lv_style_reset(&m_textTitleMedium);
	lv_style_reset(&m_textBodyLarge);
	lv_style_reset(&m_textBodyMedium);
	lv_style_reset(&m_textBodySmall);
	lv_style_reset(&m_textLabelLarge);
	lv_style_reset(&m_textLabelMedium);
	lv_style_reset(&m_textLabelSmall);

	lv_style_reset(&m_transparent);
	lv_style_reset(&m_invisibleContainer);
}

// ─────────────────────────────────────────────────────────────────────────────
// Sub-Component Styles
// Captures fine-grained layout/decoration previously set via lv_obj_set_style_*
// ─────────────────────────────────────────────────────────────────────────────

void StyleStore::buildSubComponentStyles(const tokens::DesignTokens& t) {
	// ── Screen (root desktop object: bare, no bg, no border) ──
	lv_style_init(&m_screen);
	lv_style_set_bg_opa(&m_screen, LV_OPA_COVER);
	lv_style_set_bg_color(&m_screen, t.colors.surface);

	// ── Wallpaper container ──
	lv_style_init(&m_wallpaper);
	lv_style_set_bg_color(&m_wallpaper, t.colors.primary);
	lv_style_set_bg_opa(&m_wallpaper, LV_OPA_COVER);

	// ── Wallpaper image (zero padding + no border) ──
	lv_style_init(&m_wallpaperImage);
	lv_style_set_pad_all(&m_wallpaperImage, 0);
	lv_style_set_border_width(&m_wallpaperImage, 0);

	// ── Window container (tiling area) padding ──
	lv_style_init(&m_windowContainer);
	lv_style_set_pad_all(&m_windowContainer, 4); // PAD_SMALL equivalent

	// ── Dock button (radius on top of transparent) ──
	lv_style_init(&m_dockButton);
	lv_style_set_radius(&m_dockButton, t.shapes.extraSmall);

	// ── Dock app container (running-app row inside dock) ──
	lv_style_init(&m_dockAppContainer);
	lv_style_set_pad_hor(&m_dockAppContainer, 4); // PAD_SMALL equivalent
	lv_style_set_pad_gap(&m_dockAppContainer, 4);

	// ── Panel header row ──
	lv_style_init(&m_panelHeader);
	lv_style_set_pad_left(&m_panelHeader, 4);
	lv_style_set_pad_right(&m_panelHeader, 4);
	lv_style_set_pad_top(&m_panelHeader, 4);

	// ── Panel icon button (settings/close button inside a panel) ──
	lv_style_init(&m_panelIconButton);
	lv_style_set_bg_opa(&m_panelIconButton, LV_OPA_TRANSP);
	lv_style_set_border_width(&m_panelIconButton, 0);
	lv_style_set_shadow_width(&m_panelIconButton, 0);
	lv_style_set_pad_all(&m_panelIconButton, 4); // PAD_SMALL equivalent
	lv_style_set_text_color(&m_panelIconButton, t.colors.onSurface);

	// ── Window header control button min-width ──
	lv_style_init(&m_windowHeaderButton);
	lv_style_set_min_width(&m_windowHeaderButton, 20); // SIZE_HEADER dpx-equivalent

	// ── Overlay label (dev/debug overlay text: gold) ──
	lv_style_init(&m_overlayLabel);
	lv_style_set_text_color(&m_overlayLabel, lv_color_hex(0xFFD700));
}

// ─────────────────────────────────────────────────────────────────────────────
// Base Styles
// ─────────────────────────────────────────────────────────────────────────────

void StyleStore::buildBaseStyles(const tokens::DesignTokens& t) {
	// ── Panel: glass-like containers (launcher, quick access, notifications) ──
	lv_style_init(&m_panel);
	lv_style_set_bg_color(&m_panel, t.colors.surfaceContainer);
	lv_style_set_bg_opa(&m_panel, LV_OPA_60);
	lv_style_set_text_color(&m_panel, t.colors.onSurface);
	lv_style_set_radius(&m_panel, t.shapes.large);
	lv_style_set_border_width(&m_panel, 0);
	lv_style_set_pad_all(&m_panel, 0);

	// ── Card: notification items, settings rows ──
	lv_style_init(&m_card);
	lv_style_set_bg_color(&m_card, t.colors.surfaceContainerHigh);
	lv_style_set_bg_opa(&m_card, LV_OPA_COVER);
	lv_style_set_text_color(&m_card, t.colors.onSurface);
	lv_style_set_radius(&m_card, t.shapes.medium);
	lv_style_set_pad_all(&m_card, static_cast<int32_t>(t.shapes.medium));
	lv_style_set_border_width(&m_card, 0);

	// ── Card Elevated: with drop shadow ──
	lv_style_init(&m_cardElevated);
	lv_style_set_bg_color(&m_cardElevated, t.colors.surfaceContainerHigh);
	lv_style_set_bg_opa(&m_cardElevated, LV_OPA_COVER);
	lv_style_set_text_color(&m_cardElevated, t.colors.onSurface);
	lv_style_set_radius(&m_cardElevated, t.shapes.medium);
	lv_style_set_pad_all(&m_cardElevated, static_cast<int32_t>(t.shapes.medium));
	lv_style_set_border_width(&m_cardElevated, 0);
	lv_style_set_shadow_width(&m_cardElevated, t.elevation.level1.shadowWidth);
	lv_style_set_shadow_offset_y(&m_cardElevated, t.elevation.level1.shadowOffsetY);
	lv_style_set_shadow_spread(&m_cardElevated, t.elevation.level1.shadowSpread);
	lv_style_set_shadow_opa(&m_cardElevated, t.elevation.level1.shadowOpa);
	lv_style_set_shadow_color(&m_cardElevated, t.colors.shadow);
}

// ─────────────────────────────────────────────────────────────────────────────
// Component Styles
// ─────────────────────────────────────────────────────────────────────────────

void StyleStore::buildComponentStyles(const tokens::DesignTokens& t) {
	// ── Status Bar ──
	lv_style_init(&m_statusBar);
	lv_style_set_bg_color(&m_statusBar, t.colors.surfaceContainer);
	lv_style_set_bg_opa(&m_statusBar, LV_OPA_60);
	lv_style_set_text_color(&m_statusBar, t.colors.onSurface);
	lv_style_set_pad_hor(&m_statusBar, 4); // ~PAD_SMALL in dpx-equivalent

	// ── Dock ──
	lv_style_init(&m_dock);
	lv_style_set_bg_color(&m_dock, t.colors.surfaceContainer);
	lv_style_set_bg_opa(&m_dock, LV_OPA_60);
	lv_style_set_text_color(&m_dock, t.colors.onSurface);
	lv_style_set_radius(&m_dock, t.shapes.small);
	lv_style_set_pad_hor(&m_dock, 4);
	lv_style_set_pad_gap(&m_dock, 4);
	lv_style_set_shadow_width(&m_dock, t.elevation.level2.shadowWidth);
	lv_style_set_shadow_offset_y(&m_dock, t.elevation.level2.shadowOffsetY);
	lv_style_set_shadow_spread(&m_dock, t.elevation.level2.shadowSpread);
	lv_style_set_shadow_opa(&m_dock, t.elevation.level2.shadowOpa);
	lv_style_set_shadow_color(&m_dock, t.colors.shadow);

	// ── Window Header ──
	lv_style_init(&m_windowHeader);
	lv_style_set_bg_color(&m_windowHeader, t.colors.surfaceContainerHigh);
	lv_style_set_bg_opa(&m_windowHeader, LV_OPA_COVER);
	lv_style_set_text_color(&m_windowHeader, t.colors.onSurface);
	lv_style_set_pad_all(&m_windowHeader, 0);

	// ── Window Content ──
	lv_style_init(&m_windowContent);
	lv_style_set_bg_color(&m_windowContent, t.colors.surface);
	lv_style_set_bg_opa(&m_windowContent, LV_OPA_COVER);
	lv_style_set_text_color(&m_windowContent, t.colors.onSurface);
	lv_style_set_pad_all(&m_windowContent, 0);

	// ── Window Chrome (outer frame) ──
	lv_style_init(&m_windowChrome);
	lv_style_set_radius(&m_windowChrome, t.shapes.small);
	lv_style_set_clip_corner(&m_windowChrome, true);
	lv_style_set_border_post(&m_windowChrome, true);
	lv_style_set_shadow_width(&m_windowChrome, t.elevation.level3.shadowWidth);
	lv_style_set_shadow_offset_y(&m_windowChrome, t.elevation.level3.shadowOffsetY);
	lv_style_set_shadow_spread(&m_windowChrome, t.elevation.level3.shadowSpread);
	lv_style_set_shadow_opa(&m_windowChrome, t.elevation.level3.shadowOpa);
	lv_style_set_shadow_color(&m_windowChrome, t.colors.shadow);
}

// ─────────────────────────────────────────────────────────────────────────────
// Interactive Styles
// ─────────────────────────────────────────────────────────────────────────────

void StyleStore::buildInteractiveStyles(const tokens::DesignTokens& t) {
	// ── Button Default ──
	lv_style_init(&m_buttonDefault);
	lv_style_set_bg_color(&m_buttonDefault, t.colors.primary);
	lv_style_set_bg_opa(&m_buttonDefault, LV_OPA_COVER);
	lv_style_set_text_color(&m_buttonDefault, t.colors.onPrimary);
	lv_style_set_radius(&m_buttonDefault, t.shapes.full);
	lv_style_set_shadow_width(&m_buttonDefault, t.elevation.level1.shadowWidth);
	lv_style_set_shadow_offset_y(&m_buttonDefault, t.elevation.level1.shadowOffsetY);
	lv_style_set_shadow_spread(&m_buttonDefault, t.elevation.level1.shadowSpread);
	lv_style_set_shadow_opa(&m_buttonDefault, t.elevation.level1.shadowOpa);
	lv_style_set_shadow_color(&m_buttonDefault, t.colors.shadow);

	// ── Button Pressed ──
	lv_style_init(&m_buttonPressed);
	lv_style_set_bg_color(&m_buttonPressed, t.colors.onPrimary);
	lv_style_set_bg_opa(&m_buttonPressed, t.stateLayer.pressed);
	// Reduce shadow when pressed (appears to sink)
	lv_style_set_shadow_width(&m_buttonPressed, t.elevation.level0.shadowWidth);
	lv_style_set_shadow_opa(&m_buttonPressed, t.elevation.level0.shadowOpa);

	// ── Button Focused ──
	lv_style_init(&m_buttonFocused);
	lv_style_set_bg_color(&m_buttonFocused, t.colors.onPrimary);
	lv_style_set_bg_opa(&m_buttonFocused, t.stateLayer.focus);
	lv_style_set_outline_width(&m_buttonFocused, 2);
	lv_style_set_outline_color(&m_buttonFocused, t.colors.primary);
	lv_style_set_outline_pad(&m_buttonFocused, 2);

	// ── Button Disabled ──
	lv_style_init(&m_buttonDisabled);
	lv_style_set_bg_opa(&m_buttonDisabled, t.stateLayer.disabledContainer);
	lv_style_set_text_opa(&m_buttonDisabled, t.stateLayer.disabledContent);
	lv_style_set_shadow_width(&m_buttonDisabled, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// Toggle Styles
// ─────────────────────────────────────────────────────────────────────────────

void StyleStore::buildToggleStyles(const tokens::DesignTokens& t) {
	// ── Toggle ON (checked) ──
	lv_style_init(&m_toggleOn);
	lv_style_set_bg_color(&m_toggleOn, t.colors.primaryContainer);
	lv_style_set_bg_opa(&m_toggleOn, LV_OPA_COVER);
	lv_style_set_text_color(&m_toggleOn, t.colors.onPrimaryContainer);
	lv_style_set_radius(&m_toggleOn, t.shapes.full);

	// ── Toggle OFF (unchecked) ──
	lv_style_init(&m_toggleOff);
	lv_style_set_bg_color(&m_toggleOff, t.colors.surfaceContainerHighest);
	lv_style_set_bg_opa(&m_toggleOff, LV_OPA_COVER);
	lv_style_set_text_color(&m_toggleOff, t.colors.onSurfaceVariant);
	lv_style_set_radius(&m_toggleOff, t.shapes.full);
}

// ─────────────────────────────────────────────────────────────────────────────
// Typography Styles
// ─────────────────────────────────────────────────────────────────────────────

static void applyTypeStyle(lv_style_t* style, const tokens::TypeStyle& ts) {
	lv_style_init(style);
	if (ts.font) {
		lv_style_set_text_font(style, ts.font);
	}
	if (ts.letterSpacing != 0) {
		lv_style_set_text_letter_space(style, ts.letterSpacing);
	}
	if (ts.lineHeight != 0) {
		lv_style_set_text_line_space(style, ts.lineHeight);
	}
}

void StyleStore::buildTypographyStyles(const tokens::DesignTokens& t) {
	applyTypeStyle(&m_textDisplayLarge, t.typography.displayLarge);
	applyTypeStyle(&m_textDisplaySmall, t.typography.displaySmall);
	applyTypeStyle(&m_textHeadlineMedium, t.typography.headlineMedium);
	applyTypeStyle(&m_textHeadlineSmall, t.typography.headlineSmall);
	applyTypeStyle(&m_textTitleLarge, t.typography.titleLarge);
	applyTypeStyle(&m_textTitleMedium, t.typography.titleMedium);
	applyTypeStyle(&m_textBodyLarge, t.typography.bodyLarge);
	applyTypeStyle(&m_textBodyMedium, t.typography.bodyMedium);
	applyTypeStyle(&m_textBodySmall, t.typography.bodySmall);
	applyTypeStyle(&m_textLabelLarge, t.typography.labelLarge);
	applyTypeStyle(&m_textLabelMedium, t.typography.labelMedium);
	applyTypeStyle(&m_textLabelSmall, t.typography.labelSmall);
}

// ─────────────────────────────────────────────────────────────────────────────
// Utility Styles
// ─────────────────────────────────────────────────────────────────────────────

void StyleStore::buildUtilityStyles(const tokens::DesignTokens& /*t*/) {
	// ── Transparent ──
	lv_style_init(&m_transparent);
	lv_style_set_bg_opa(&m_transparent, LV_OPA_TRANSP);
	lv_style_set_border_width(&m_transparent, 0);
	lv_style_set_shadow_width(&m_transparent, 0);

	// ── Invisible Container ──
	lv_style_init(&m_invisibleContainer);
	lv_style_set_bg_opa(&m_invisibleContainer, LV_OPA_TRANSP);
	lv_style_set_border_width(&m_invisibleContainer, 0);
	lv_style_set_shadow_width(&m_invisibleContainer, 0);
	lv_style_set_pad_all(&m_invisibleContainer, 0);
}

} // namespace flx::ui::theming
