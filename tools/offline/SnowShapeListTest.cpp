// SPDX-License-Identifier: GPL-3.0-only
// The list file's text, and the model forms the game and a person write for the same mesh.
#include "SnowShapeList.h"

#include <cstdio>

namespace
{
	int g_failures = 0;

	void Check(bool a_ok, const char* a_what)
	{
		if (!a_ok) {
			++g_failures;
			std::printf("FAIL: %s\n", a_what);
		}
	}
}

int main()
{
	using namespace SnowShapeList;

	// The game's form, a typed form with forward slashes, and a form with the folder prefixes.
	Check(Model("Architecture\\Windhelm\\WHmarket01.nif") == "architecture\\windhelm\\whmarket01.nif", "game path");
	Check(Model("architecture/windhelm/whmarket01.nif") == "architecture\\windhelm\\whmarket01.nif", "forward slashes");
	Check(Model("Meshes\\Architecture\\Windhelm\\WHmarket01.nif") == "architecture\\windhelm\\whmarket01.nif", "meshes prefix");
	Check(Model("Data/Meshes/Architecture/Windhelm/WHmarket01.nif") == "architecture\\windhelm\\whmarket01.nif", "data prefix");

	std::string model, key;
	Check(Parse("architecture\\windhelm\\whmarket01.nif | WHmarket01:0.008\r", model, key), "a line parses");
	Check(model == "architecture\\windhelm\\whmarket01.nif", "model of the line");
	Check(key == "architecture\\windhelm\\whmarket01.nif|whmarket01:0.008", "key of the line");
	Check(key == Key("Architecture\\Windhelm\\WHmarket01.nif", "WHmarket01:0.008"), "the game's strings make the same key");

	// A shape name keeps its own dots and colons, and spaces inside it.
	Check(Parse("a.nif|Ground 003.x:1", model, key) && key == "a.nif|ground 003.x:1", "shape name kept");

	Check(!Parse("", model, key), "blank line");
	Check(!Parse("   \t", model, key), "white space");
	Check(!Parse("# a comment | with a bar", model, key), "hash comment");
	Check(!Parse("; a comment | with a bar", model, key), "semicolon comment");
	Check(!Parse("a.nif", model, key), "no bar");
	Check(!Parse("a.nif |  ", model, key), "no shape");
	Check(!Parse(" | shape", model, key), "no model");

	std::printf(g_failures ? "SnowShapeListTest: %d failure(s)\n" : "SnowShapeListTest: ALL PASS\n", g_failures);
	return g_failures ? 1 : 0;
}
