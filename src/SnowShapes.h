// SPDX-License-Identifier: GPL-3.0-only
#pragma once
#include "SurfaceProfiles.h"
#include <cstdint>
#include <filesystem>

namespace RE { class BSGeometry; class NiPoint3; }

// Listed snow meshes that take the deformation field, in a world named by CityWorldsAllowed or by a list's own
// "world = 0001691D" line. Lists are SKSE\Plugins\NMN_DeformableTerrain_SnowShapes.txt and every .txt in the
// SKSE\Plugins\NMN_DeformableTerrain_SnowShapes folder. With no list or no such world, Active() stays false
// and nothing else here runs, so every other world behaves as it did.
namespace SnowShapes
{
	void Load();
	std::filesystem::file_time_type ListWriteTime();

	// Does a list that holds shapes name this world?
	bool ListNamesWorld(uint32_t a_world);

	// The world to act in, 0 for none. Called when terrain activity changes.
	void SetWorld(uint32_t a_world);
	bool Active();

	// Draw hook: is this geometry a listed shape? One lookup per draw; the strings are read once per geometry.
	bool Contains(RE::BSGeometry* a_geometry);

	// Does a ray down from just above this point land on a reference with a listed model, and at what height?
	// Update thread only.
	bool OnListedShape(const RE::NiPoint3& a_at, float& a_surfaceZ);

	// What a mark on a listed shape uses. The list holds snow meshes, so it is the snow response.
	Surfaces::Ground ShapeGround();
}
