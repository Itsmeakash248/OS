#include <flx/ui/theming/ThemeRegistry.hpp>
#include <flx/ui/theming/themes/Hyprland.hpp>
#include <flx/ui/theming/themes/MaterialDark.hpp>
#include <flx/ui/theming/themes/MaterialLight.hpp>
#include <flx/ui/theming/themes/Nord.hpp>

namespace flx::ui::theming {

void ThemeRegistry::registerTheme(ThemeDefinition def) {
	m_themes.push_back(std::move(def));
}

const ThemeDefinition* ThemeRegistry::getById(const std::string& id) const {
	for (const auto& t: m_themes) {
		if (t.id == id) return &t;
	}
	return nullptr;
}

const ThemeDefinition* ThemeRegistry::getByIndex(size_t index) const {
	if (index < m_themes.size()) return &m_themes[index];
	return nullptr;
}

void ThemeRegistry::registerBuiltins() {
	registerTheme(themes::hyprland());
	registerTheme(themes::materialLight());
	registerTheme(themes::materialDark());
	registerTheme(themes::nord());
}

} // namespace flx::ui::theming
