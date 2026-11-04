// SPDX-License-Identifier: GPL-3.0-only
#pragma once
#include "SurfaceProfiles.h"
#include <cstdint>
#include <filesystem>

namespace RE { class BSGeometry; class NiPoint3; }

// Listed snow meshes that take the deformation field, in a world named by CityWorldsAllowed. The list is
// SKSE\Plugins\NMN_DeformableTerrain_SnowShapes.txt. With no list or no such world, Active() stays false and
// nothing else here runs, so every other world behaves as it did.
namespace SnowShapes
{
	void Load();
	std::filesystem::file_time_type ListWriteTime();

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
