#pragma once

#include <cstddef>
#include <flx/ui/theming/ThemeDefinition.hpp>
#include <string>
#include <vector>

namespace flx::ui::theming {

/**
 * @brief Registry of all available themes (built-in + user-loaded).
 *
 * Built-in themes are registered at init time.
 * User themes can be added via `registerTheme()` (e.g. from SD card JSON).
 * ThemeEngine uses `getById()` or `getByIndex()` to retrieve definitions.
 */
class ThemeRegistry {
public:

	/// Register a built-in or user theme definition.
	void registerTheme(ThemeDefinition def);

	/// Get a theme definition by its id. Returns nullptr if not found.
	const ThemeDefinition* getById(const std::string& id) const;

	/// Get a theme definition by index. Returns nullptr if out of range.
	const ThemeDefinition* getByIndex(size_t index) const;

	/// Get the total number of registered themes.
	size_t count() const { return m_themes.size(); }

	/// Get all registered themes.
	const std::vector<ThemeDefinition>& getAll() const { return m_themes; }

	/// Register all built-in themes (Material Light/Dark, Hyprland, Nord).
	void registerBuiltins();

private:

	std::vector<ThemeDefinition> m_themes;
};

} // namespace flx::ui::theming
