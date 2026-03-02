#pragma once

#include <flx/ui/theming/ThemeDefinition.hpp>
#include <string>
#include <vector>

namespace flx::ui::theming {

/**
 * @brief Loads ThemeDefinition objects from JSON files.
 *
 * Parses JSON theme files (e.g. from SD card `/sdcard/themes/` directory)
 * into `ThemeDefinition` structs that can be registered in `ThemeRegistry`.
 *
 * JSON schema:
 * @code
 * {
 *   "name": "Custom Nord",
 *   "id": "custom_nord",
 *   "dark": true,
 *   "colors": {
 *     "primary": "#88C0D0",
 *     "onPrimary": "#2E3440",
 *     "surface": "#3B4252",
 *     ...
 *   },
 *   "shapes": {
 *     "none": 0,
 *     "extraSmall": 4,
 *     "small": 8,
 *     "medium": 12,
 *     "large": 16,
 *     "extraLarge": 28
 *   }
 * }
 * @endcode
 *
 * Uses ESP-IDF's built-in cJSON library.
 */
class ThemeLoader {
public:

	/// Load a single theme from a JSON file path.
	/// @param path Absolute path to JSON file (e.g. "/sdcard/themes/nord.json")
	/// @param[out] def Populated ThemeDefinition on success
	/// @return true on success, false on parse error or missing required fields
	static bool loadFromFile(const std::string& path, ThemeDefinition& def);

	/// Load a single theme from a JSON string.
	/// @param json JSON string content
	/// @param[out] def Populated ThemeDefinition on success
	/// @return true on success, false on parse error or missing required fields
	static bool loadFromString(const std::string& json, ThemeDefinition& def);

	/// Scan a directory for *.json files and load all valid themes.
	/// @param dirPath Directory path (e.g. "/sdcard/themes")
	/// @return Vector of successfully parsed ThemeDefinitions
	static std::vector<ThemeDefinition> loadFromDirectory(const std::string& dirPath);
};

} // namespace flx::ui::theming
