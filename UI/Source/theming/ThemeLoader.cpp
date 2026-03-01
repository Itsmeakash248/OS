#include <flx/core/Logger.hpp>
#include <flx/ui/theming/ThemeLoader.hpp>
#include <flx/ui/theming/tokens/DesignTokens.hpp>

#include "cJSON.h"

#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <string>
#include <string_view>
#include <sys/stat.h>

static constexpr std::string_view TAG = "ThemeLoader";

using namespace flx::ui::tokens;

namespace flx::ui::theming {

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

/// Parse a hex color string (#RRGGBB or RRGGBB) to lv_color_t.
static bool parseHexColor(const char* str, lv_color_t& out) {
	if (!str) return false;
	if (str[0] == '#') str++; // skip leading '#'

	unsigned int hex = 0;
	if (std::sscanf(str, "%06x", &hex) != 1) return false;

	out = lv_color_hex(hex);
	return true;
}

/// Try to read a color from a cJSON object by key.
static bool readColor(const cJSON* obj, const char* key, lv_color_t& out) {
	const cJSON* item = cJSON_GetObjectItem(obj, key);
	if (!item || !cJSON_IsString(item)) return false;
	return parseHexColor(item->valuestring, out);
}

/// Try to read an int32_t from a cJSON object by key.
static bool readInt(const cJSON* obj, const char* key, int32_t& out) {
	const cJSON* item = cJSON_GetObjectItem(obj, key);
	if (!item || !cJSON_IsNumber(item)) return false;
	out = static_cast<int32_t>(item->valuedouble);
	return true;
}

/// Try to read a uint32_t from a cJSON object by key.
static bool readUint(const cJSON* obj, const char* key, uint32_t& out) {
	const cJSON* item = cJSON_GetObjectItem(obj, key);
	if (!item || !cJSON_IsNumber(item)) return false;
	out = static_cast<uint32_t>(item->valuedouble);
	return true;
}

/// Try to read a bool from a cJSON object by key.
static bool readBool(const cJSON* obj, const char* key, bool& out) {
	const cJSON* item = cJSON_GetObjectItem(obj, key);
	if (!item) return false;
	if (cJSON_IsBool(item)) {
		out = cJSON_IsTrue(item);
		return true;
	}
	return false;
}

/// Parse the "colors" object into ColorTokens.
/// Missing fields retain defaults (black).
static void parseColors(const cJSON* colorsObj, ColorTokens& c) {
	if (!colorsObj) return;

	// Primary
	readColor(colorsObj, "primary", c.primary);
	readColor(colorsObj, "onPrimary", c.onPrimary);
	readColor(colorsObj, "primaryContainer", c.primaryContainer);
	readColor(colorsObj, "onPrimaryContainer", c.onPrimaryContainer);

	// Secondary
	readColor(colorsObj, "secondary", c.secondary);
	readColor(colorsObj, "onSecondary", c.onSecondary);
	readColor(colorsObj, "secondaryContainer", c.secondaryContainer);
	readColor(colorsObj, "onSecondaryContainer", c.onSecondaryContainer);

	// Tertiary
	readColor(colorsObj, "tertiary", c.tertiary);
	readColor(colorsObj, "onTertiary", c.onTertiary);
	readColor(colorsObj, "tertiaryContainer", c.tertiaryContainer);
	readColor(colorsObj, "onTertiaryContainer", c.onTertiaryContainer);

	// Surface
	readColor(colorsObj, "surface", c.surface);
	readColor(colorsObj, "surfaceDim", c.surfaceDim);
	readColor(colorsObj, "surfaceBright", c.surfaceBright);
	readColor(colorsObj, "surfaceContainerLowest", c.surfaceContainerLowest);
	readColor(colorsObj, "surfaceContainerLow", c.surfaceContainerLow);
	readColor(colorsObj, "surfaceContainer", c.surfaceContainer);
	readColor(colorsObj, "surfaceContainerHigh", c.surfaceContainerHigh);
	readColor(colorsObj, "surfaceContainerHighest", c.surfaceContainerHighest);
	readColor(colorsObj, "onSurface", c.onSurface);
	readColor(colorsObj, "onSurfaceVariant", c.onSurfaceVariant);

	// Outline
	readColor(colorsObj, "outline", c.outline);
	readColor(colorsObj, "outlineVariant", c.outlineVariant);

	// Error
	readColor(colorsObj, "error", c.error);
	readColor(colorsObj, "onError", c.onError);
	readColor(colorsObj, "errorContainer", c.errorContainer);
	readColor(colorsObj, "onErrorContainer", c.onErrorContainer);

	// Inverse
	readColor(colorsObj, "inverseSurface", c.inverseSurface);
	readColor(colorsObj, "inverseOnSurface", c.inverseOnSurface);
	readColor(colorsObj, "inversePrimary", c.inversePrimary);

	// Fixed
	readColor(colorsObj, "scrim", c.scrim);
	readColor(colorsObj, "shadow", c.shadow);

	// Background
	readColor(colorsObj, "background", c.background);
	readColor(colorsObj, "onBackground", c.onBackground);
}

/// Parse the "shapes" object into ShapeTokens (overrides defaults).
static void parseShapes(const cJSON* shapesObj, ShapeTokens& s) {
	if (!shapesObj) return;
	readInt(shapesObj, "none", s.none);
	readInt(shapesObj, "extraSmall", s.extraSmall);
	readInt(shapesObj, "small", s.small);
	readInt(shapesObj, "medium", s.medium);
	readInt(shapesObj, "large", s.large);
	readInt(shapesObj, "extraLarge", s.extraLarge);
	// "full" is always LV_RADIUS_CIRCLE — don't override
}

/// Parse the "motion" object into MotionTokens (overrides defaults).
static void parseMotion(const cJSON* motionObj, MotionTokens& m) {
	if (!motionObj) return;
	readUint(motionObj, "durationShort1", m.durationShort1);
	readUint(motionObj, "durationShort2", m.durationShort2);
	readUint(motionObj, "durationShort3", m.durationShort3);
	readUint(motionObj, "durationShort4", m.durationShort4);
	readUint(motionObj, "durationMedium1", m.durationMedium1);
	readUint(motionObj, "durationMedium2", m.durationMedium2);
	readUint(motionObj, "durationMedium3", m.durationMedium3);
	readUint(motionObj, "durationMedium4", m.durationMedium4);
	readUint(motionObj, "durationLong1", m.durationLong1);
	readUint(motionObj, "durationLong2", m.durationLong2);
	// Easing curves are not overridable from JSON (bezier params are complex)
}

/// Parse the "elevation" object.
static void parseElevationLevel(const cJSON* obj, ElevationLevel& lvl) {
	if (!obj) return;
	readInt(obj, "shadowWidth", lvl.shadowWidth);
	readInt(obj, "shadowOffsetX", lvl.shadowOffsetX);
	readInt(obj, "shadowOffsetY", lvl.shadowOffsetY);
	readInt(obj, "shadowSpread", lvl.shadowSpread);
	int32_t opa = 0;
	if (readInt(obj, "shadowOpa", opa)) {
		lvl.shadowOpa = static_cast<lv_opa_t>(opa);
	}
}

static void parseElevation(const cJSON* elevObj, ElevationTokens& e) {
	if (!elevObj) return;
	parseElevationLevel(cJSON_GetObjectItem(elevObj, "level0"), e.level0);
	parseElevationLevel(cJSON_GetObjectItem(elevObj, "level1"), e.level1);
	parseElevationLevel(cJSON_GetObjectItem(elevObj, "level2"), e.level2);
	parseElevationLevel(cJSON_GetObjectItem(elevObj, "level3"), e.level3);
	parseElevationLevel(cJSON_GetObjectItem(elevObj, "level4"), e.level4);
	parseElevationLevel(cJSON_GetObjectItem(elevObj, "level5"), e.level5);
}

// ─────────────────────────────────────────────────────────────────────────────
// Core parse from cJSON root
// ─────────────────────────────────────────────────────────────────────────────

static bool parseRoot(cJSON* root, ThemeDefinition& def) {
	if (!root || !cJSON_IsObject(root)) return false;

	// Required: "name" and "id"
	const cJSON* nameItem = cJSON_GetObjectItem(root, "name");
	const cJSON* idItem = cJSON_GetObjectItem(root, "id");
	if (!nameItem || !cJSON_IsString(nameItem) ||
		!idItem || !cJSON_IsString(idItem)) {
		Log::error(TAG, "Theme JSON missing required 'name' or 'id' fields");
		return false;
	}

	def.name = nameItem->valuestring;
	def.id = idItem->valuestring;
	readBool(root, "dark", def.dark);

	// Start with sensible defaults
	def.tokens.shapes = defaultShapeTokens();
	def.tokens.motion = defaultMotionTokens();
	def.tokens.elevation = defaultElevationTokens();
	def.tokens.stateLayer = defaultStateLayerTokens();

	// Parse optional sections
	parseColors(cJSON_GetObjectItem(root, "colors"), def.tokens.colors);
	parseShapes(cJSON_GetObjectItem(root, "shapes"), def.tokens.shapes);
	parseMotion(cJSON_GetObjectItem(root, "motion"), def.tokens.motion);
	parseElevation(cJSON_GetObjectItem(root, "elevation"), def.tokens.elevation);

	return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

bool ThemeLoader::loadFromString(const std::string& json, ThemeDefinition& def) {
	cJSON* root = cJSON_Parse(json.c_str());
	if (!root) {
		const char* errPtr = cJSON_GetErrorPtr();
		Log::error(TAG, "JSON parse error near: %s", errPtr ? errPtr : "(null)");
		return false;
	}

	bool ok = parseRoot(root, def);
	cJSON_Delete(root);

	if (ok) {
		Log::info(TAG, "Loaded theme '%s' (id: %s) from JSON string", def.name.c_str(), def.id.c_str());
	}
	return ok;
}

bool ThemeLoader::loadFromFile(const std::string& path, ThemeDefinition& def) {
	FILE* f = fopen(path.c_str(), "r");
	if (!f) {
		Log::error(TAG, "Cannot open theme file: %s", path.c_str());
		return false;
	}

	// Get file size
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (size <= 0 || size > 64 * 1024) { // max 64KB per theme file
		Log::error(TAG, "Theme file too large or empty: %s (%ld bytes)", path.c_str(), size);
		fclose(f);
		return false;
	}

	std::string content(static_cast<size_t>(size), '\0');
	size_t read = fread(content.data(), 1, static_cast<size_t>(size), f);
	fclose(f);

	if (read != static_cast<size_t>(size)) {
		Log::error(TAG, "Failed to read complete file: %s", path.c_str());
		return false;
	}

	Log::info(TAG, "Loading theme from file: %s", path.c_str());
	return loadFromString(content, def);
}

std::vector<ThemeDefinition> ThemeLoader::loadFromDirectory(const std::string& dirPath) {
	std::vector<ThemeDefinition> themes;

	DIR* dir = opendir(dirPath.c_str());
	if (!dir) {
		Log::warn(TAG, "Cannot open theme directory: %s", dirPath.c_str());
		return themes;
	}

	struct dirent* entry;
	while ((entry = readdir(dir)) != nullptr) {
		const char* name = entry->d_name;
		size_t len = strlen(name);

		// Only process *.json files
		if (len < 6) continue;
		if (strcmp(name + len - 5, ".json") != 0) continue;

		std::string fullPath = dirPath + "/" + name;

		ThemeDefinition def;
		if (loadFromFile(fullPath, def)) {
			Log::info(TAG, "Registered user theme: %s (%s)", def.name.c_str(), def.id.c_str());
			themes.push_back(std::move(def));
		}
	}

	closedir(dir);
	Log::info(TAG, "Loaded %zu user theme(s) from %s", themes.size(), dirPath.c_str());
	return themes;
}

} // namespace flx::ui::theming
