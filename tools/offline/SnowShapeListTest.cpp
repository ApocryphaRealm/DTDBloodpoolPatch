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

	// The world line, and everything that must not pass for one.
	uint32_t world = 0;
	Check(ParseWorld("world = 0001691D\r", world) && world == 0x0001691D, "world line");
	Check(ParseWorld("  WORLD=0001691d", world) && world == 0x0001691D, "world line, case and spacing");
	Check(!ParseWorld("world = 1691D", world), "short form ID");
	Check(!ParseWorld("world = 00000000", world), "no world");
	Check(!ParseWorld("world = 0001691G", world), "not hex");
	Check(!ParseWorld("# world = 0001691D", world), "commented out");
	Check(!ParseWorld("a.nif | world = 0001691D", world), "a shape line");
	Check(!Parse("world = 0001691D", model, key), "a world line is not a shape");

	std::string plugin;
	Check(ParseRequires("requires = Windhelm Is Snowy.esp\r", plugin) && plugin == "Windhelm Is Snowy.esp", "requires line, spaces in the name");
	Check(!ParseRequires("requires =  ", plugin), "requires with no plugin");
	Check(!ParseRequires("# requires = a.esp", plugin), "requires commented out");
	Check(!ParseRequires("world = 0001691D", plugin), "a world line is not requires");
	Check(!Parse("requires = a.esp", model, key), "a requires line is not a shape");

	std::printf(g_failures ? "SnowShapeListTest: %d failure(s)\n" : "SnowShapeListTest: ALL PASS\n", g_failures);
	return g_failures ? 1 : 0;
}
