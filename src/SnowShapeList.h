// SPDX-License-Identifier: GPL-3.0-only
#pragma once
#include <algorithm>
#include <string>
#include <string_view>

// The text of the snow shape list: one "model.nif | shape name" per line, # or ; starts a comment line.
// Kept free of game types so tools/offline/SnowShapeListTest.cpp can run it.
namespace SnowShapeList
{
	inline std::string Lower(std::string_view a_text)
	{
		std::string out(a_text);
		std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) {
			return c >= 'A' && c <= 'Z' ? static_cast<char>(c + ('a' - 'A')) : static_cast<char>(c);
		});
		return out;
	}

	inline std::string_view Trimmed(std::string_view a_text)
	{
		while (!a_text.empty() && (a_text.front() == ' ' || a_text.front() == '\t')) { a_text.remove_prefix(1); }
		while (!a_text.empty() && (a_text.back() == ' ' || a_text.back() == '\t' || a_text.back() == '\r')) { a_text.remove_suffix(1); }
		return a_text;
	}

	// A model path as the game holds it, or as a person types it, to one form: lower case, backslashes,
	// no leading Data\ or Meshes\.
	inline std::string Model(std::string_view a_path)
	{
		std::string out = Lower(Trimmed(a_path));
		std::replace(out.begin(), out.end(), '/', '\\');
		for (const std::string_view prefix : { std::string_view("data\\"), std::string_view("meshes\\") }) {
			if (out.starts_with(prefix)) { out.erase(0, prefix.size()); }
		}
		return out;
	}

	inline std::string Key(std::string_view a_model, std::string_view a_shape)
	{
		return Model(a_model) + "|" + Lower(Trimmed(a_shape));
	}

	// The model and the key of one line. False for a blank line, a comment or a line with no shape name.
	inline bool Parse(std::string_view a_line, std::string& a_model, std::string& a_key)
	{
		const auto line = Trimmed(a_line);
		if (line.empty() || line.front() == '#' || line.front() == ';') { return false; }
		const auto bar = line.find('|');
		if (bar == std::string_view::npos) { return false; }
		const auto model = Trimmed(line.substr(0, bar));
		const auto shape = Trimmed(line.substr(bar + 1));
		if (model.empty() || shape.empty()) { return false; }
		a_model = Model(model);
		a_key = Key(model, shape);
		return true;
	}
}
