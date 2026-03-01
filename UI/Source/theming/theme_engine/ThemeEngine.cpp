#include "core/lv_obj_style.h"
#include "display/lv_display.h"
#include "lv_conf_internal.h"
#include "misc/lv_types.h"
#include "themes/default/lv_theme_default.h"
#include "themes/lv_theme.h"
#include <cstddef>
#include <cstdint>
#include <flx/core/Logger.hpp>
#include <flx/system/managers/ThemeManager.hpp>
#include <flx/ui/GuiTask.hpp>
#include <flx/ui/theming/StyleStore.hpp>
#include <flx/ui/theming/ThemeRegistry.hpp>
#include <flx/ui/theming/theme_engine/ThemeEngine.hpp>
#include <flx/ui/theming/themes/Themes.hpp>
#include <map>
#include <string_view>
#include <vector>

using namespace flx::ui;
using namespace flx::ui::tokens;

static constexpr std::string_view TAG = "ThemeEngine";

ThemeType ThemeEngine::current_theme = ThemeType::HYPRLAND;
static std::map<lv_display_t*, std::vector<lv_theme_t*>> engine_themes;

/// Shared ThemeRegistry instance — populated once at init
static flx::ui::theming::ThemeRegistry s_registry;

// ─────────────────────────────────────────────────────────────────────────────
// Color interpolation helpers for animated theme transitions
// ─────────────────────────────────────────────────────────────────────────────

/// Linear interpolation of a single uint8_t channel.
static inline uint8_t lerpU8(uint8_t a, uint8_t b, int32_t t) {
	// t is 0–255, where 0 = a and 255 = b
	return static_cast<uint8_t>(a + ((static_cast<int32_t>(b) - static_cast<int32_t>(a)) * t / 255));
}

/// Linear interpolation of two lv_color_t values.
static inline lv_color_t lerpColor(lv_color_t a, lv_color_t b, int32_t t) {
	lv_color_t out;
	out.red = lerpU8(a.red, b.red, t);
	out.green = lerpU8(a.green, b.green, t);
	out.blue = lerpU8(a.blue, b.blue, t);
	return out;
}

/// Interpolate all ColorTokens fields between 'from' and 'to'.
static ColorTokens lerpColorTokens(const ColorTokens& from, const ColorTokens& to, int32_t t) {
	ColorTokens out;
	// Primary
	out.primary = lerpColor(from.primary, to.primary, t);
	out.onPrimary = lerpColor(from.onPrimary, to.onPrimary, t);
	out.primaryContainer = lerpColor(from.primaryContainer, to.primaryContainer, t);
	out.onPrimaryContainer = lerpColor(from.onPrimaryContainer, to.onPrimaryContainer, t);
	// Secondary
	out.secondary = lerpColor(from.secondary, to.secondary, t);
	out.onSecondary = lerpColor(from.onSecondary, to.onSecondary, t);
	out.secondaryContainer = lerpColor(from.secondaryContainer, to.secondaryContainer, t);
	out.onSecondaryContainer = lerpColor(from.onSecondaryContainer, to.onSecondaryContainer, t);
	// Tertiary
	out.tertiary = lerpColor(from.tertiary, to.tertiary, t);
	out.onTertiary = lerpColor(from.onTertiary, to.onTertiary, t);
	out.tertiaryContainer = lerpColor(from.tertiaryContainer, to.tertiaryContainer, t);
	out.onTertiaryContainer = lerpColor(from.onTertiaryContainer, to.onTertiaryContainer, t);
	// Surface
	out.surface = lerpColor(from.surface, to.surface, t);
	out.surfaceDim = lerpColor(from.surfaceDim, to.surfaceDim, t);
	out.surfaceBright = lerpColor(from.surfaceBright, to.surfaceBright, t);
	out.surfaceContainerLowest = lerpColor(from.surfaceContainerLowest, to.surfaceContainerLowest, t);
	out.surfaceContainerLow = lerpColor(from.surfaceContainerLow, to.surfaceContainerLow, t);
	out.surfaceContainer = lerpColor(from.surfaceContainer, to.surfaceContainer, t);
	out.surfaceContainerHigh = lerpColor(from.surfaceContainerHigh, to.surfaceContainerHigh, t);
	out.surfaceContainerHighest = lerpColor(from.surfaceContainerHighest, to.surfaceContainerHighest, t);
	out.onSurface = lerpColor(from.onSurface, to.onSurface, t);
	out.onSurfaceVariant = lerpColor(from.onSurfaceVariant, to.onSurfaceVariant, t);
	// Outline
	out.outline = lerpColor(from.outline, to.outline, t);
	out.outlineVariant = lerpColor(from.outlineVariant, to.outlineVariant, t);
	// Error
	out.error = lerpColor(from.error, to.error, t);
	out.onError = lerpColor(from.onError, to.onError, t);
	out.errorContainer = lerpColor(from.errorContainer, to.errorContainer, t);
	out.onErrorContainer = lerpColor(from.onErrorContainer, to.onErrorContainer, t);
	// Inverse
	out.inverseSurface = lerpColor(from.inverseSurface, to.inverseSurface, t);
	out.inverseOnSurface = lerpColor(from.inverseOnSurface, to.inverseOnSurface, t);
	out.inversePrimary = lerpColor(from.inversePrimary, to.inversePrimary, t);
	// Fixed
	out.scrim = lerpColor(from.scrim, to.scrim, t);
	out.shadow = lerpColor(from.shadow, to.shadow, t);
	// Background
	out.background = lerpColor(from.background, to.background, t);
	out.onBackground = lerpColor(from.onBackground, to.onBackground, t);
	return out;
}

// ─────────────────────────────────────────────────────────────────────────────
// Transition state — file-static so the lv_anim_t callback can access it
// ─────────────────────────────────────────────────────────────────────────────

struct ThemeTransitionState {
	DesignTokens fromTokens; ///< Snapshot of tokens before transition
	DesignTokens toTokens; ///< Target tokens
	bool active = false; ///< True while animation is running
};

static ThemeTransitionState s_transition;
static lv_anim_t s_transitionAnim;

/// Duration of theme transition animation in ms.
static constexpr uint32_t THEME_TRANSITION_DURATION_MS = 300;

/// Animation exec callback — called each frame with value 0..255.
static void themeTransitionExecCb(void* /*var*/, int32_t value) {
	if (!s_transition.active) return;

	// Interpolate colors only; shapes/motion/elevation use target values immediately
	DesignTokens interpolated = s_transition.toTokens;
	interpolated.colors = lerpColorTokens(s_transition.fromTokens.colors, s_transition.toTokens.colors, value);

	auto& ss = flx::ui::theming::StyleStore::getInstance();
	ss.rebuild(interpolated);
	lv_obj_report_style_change(nullptr);
}

/// Animation ready callback — clean up transition state.
static void themeTransitionReadyCb(lv_anim_t* /*a*/) {
	// Final rebuild with exact target tokens (ensure no rounding artifacts)
	auto& ss = flx::ui::theming::StyleStore::getInstance();
	ss.rebuild(s_transition.toTokens);
	lv_obj_report_style_change(nullptr);
	s_transition.active = false;
	Log::info(TAG, "Theme transition complete");
}

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

void ThemeEngine::init() {
	Log::info(TAG, "Initializing theme engine...");

	// Register built-in themes and wire up the legacy compat shim
	s_registry.registerBuiltins();
	Themes::setRegistry(&s_registry);

	lv_display_t* disp = lv_display_get_default();
	if (disp) {
		apply_theme(current_theme, disp);
	} else {
		Log::warn(TAG, "No default display found during init");
	}
}

void ThemeEngine::set_theme(ThemeType theme, lv_display_t* disp) {
	GuiTask::lock();
	if (!disp) {
		disp = lv_display_get_default();
	}
	if (!disp) {
		GuiTask::unlock();
		return;
	}

	if (current_theme != theme) {
		Log::info(TAG, "Switching theme to %d", (int)theme);
		current_theme = theme;
		apply_theme(theme, disp);
	}
	GuiTask::unlock();
}

ThemeType ThemeEngine::get_current_theme() { return current_theme; }

void ThemeEngine::cycle_theme() {
	// Cycle through all registered themes
	int32_t next_idx = (static_cast<int32_t>(current_theme) + 1) % THEME_COUNT;
	GuiTask::lock();
	flx::system::ThemeManager::getInstance().getThemeObservable().set(next_idx);
	GuiTask::unlock();
}

flx::ui::theming::ThemeRegistry& ThemeEngine::getRegistry() {
	return s_registry;
}

void ThemeEngine::cleanup_previous_theme(lv_display_t* disp) {
	if (engine_themes.count(disp)) {
		auto& themes = engine_themes[disp];
		for (auto it = themes.rbegin(); it != themes.rend(); ++it) {
			if (*it == lv_theme_default_get()) {
				continue;
			}
			lv_theme_delete(*it);
		}
		themes.clear();
		engine_themes.erase(disp);
	}
}

void ThemeEngine::apply_theme(ThemeType theme, lv_display_t* disp) {
	GuiTask::lock();
	if (!disp) {
		disp = lv_display_get_default();
	}
	if (!disp) {
		GuiTask::unlock();
		return;
	}

	auto idx = static_cast<size_t>(static_cast<int32_t>(theme));
	const auto* def = s_registry.getByIndex(idx);
	if (!def) {
		Log::error(TAG, "Theme index %zu not found in registry", idx);
		GuiTask::unlock();
		return;
	}

	Log::debug(TAG, "Applying theme: %s (Dark: %s)", def->name.c_str(), def->dark ? "Yes" : "No");

	const auto& tokens = def->tokens;
	auto& ss = flx::ui::theming::StyleStore::getInstance();

	if (!ss.isInitialized()) {
		// First time — no animation, just init
		ss.init(tokens);
	} else {
		// Animated transition: capture current tokens, lerp to new ones
		if (s_transition.active) {
			// A transition is already running — cancel it and jump to target
			lv_anim_delete(&s_transition, themeTransitionExecCb);
			s_transition.active = false;
		}

		s_transition.fromTokens = ss.getTokens();
		s_transition.toTokens = tokens;
		s_transition.active = true;

		lv_anim_init(&s_transitionAnim);
		lv_anim_set_var(&s_transitionAnim, &s_transition);
		lv_anim_set_values(&s_transitionAnim, 0, 255);
		lv_anim_set_duration(&s_transitionAnim, THEME_TRANSITION_DURATION_MS);
		lv_anim_set_exec_cb(&s_transitionAnim, themeTransitionExecCb);
		lv_anim_set_ready_cb(&s_transitionAnim, themeTransitionReadyCb);
		lv_anim_set_path_cb(&s_transitionAnim, lv_anim_path_ease_in_out);
		lv_anim_start(&s_transitionAnim);

		Log::info(TAG, "Started %lums theme transition animation", (unsigned long)THEME_TRANSITION_DURATION_MS);
	}

	// Build LVGL native theme chain
	std::vector<lv_theme_t*> new_themes_vec;

	// Legacy compat: derive primary/secondary for lv_theme_default_init
	ThemeConfig cfg = Themes::GetConfig(theme);
	lv_theme_t* base_th = lv_theme_default_init(disp, cfg.primary, cfg.secondary, def->dark, LV_FONT_DEFAULT);
	new_themes_vec.push_back(base_th);

	lv_theme_t* global_th = lv_theme_create();
	lv_theme_set_parent(global_th, base_th);
	lv_theme_set_apply_cb(global_th, Themes::ApplyGlobal);
	new_themes_vec.push_back(global_th);

	lv_theme_t* final_theme = global_th;
	lv_display_set_theme(disp, final_theme);

	cleanup_previous_theme(disp);

	engine_themes[disp] = new_themes_vec;

	lv_obj_report_style_change(nullptr);
	lv_obj_invalidate(lv_display_get_screen_active(disp));
	lv_obj_invalidate(lv_display_get_layer_top(disp));
	lv_obj_invalidate(lv_display_get_layer_sys(disp));
	GuiTask::unlock();
}
