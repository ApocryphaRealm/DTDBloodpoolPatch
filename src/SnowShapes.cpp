// SPDX-License-Identifier: GPL-3.0-only

#include "PCH.h"
#include "SnowShapes.h"
#include "SnowShapeList.h"

#include <algorithm>
#include <atomic>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

namespace SnowShapes
{
	namespace
	{
		constexpr auto kPath = "Data/SKSE/Plugins/NMN_DeformableTerrain_SnowShapes.txt";

		// A ray from this far above the point to this far below it finds the surface under standing feet.
		constexpr float kRayAbove = 32.0f;
		constexpr float kRayBelow = 96.0f;

		constexpr size_t kMaxDraws = 1 << 16;
		constexpr uint32_t kMaxNotListedLogged = 24;
		constexpr uint32_t kMaxGroundLogged = 24;

		std::unordered_set<std::string> g_shapes;
		std::unordered_set<std::string> g_models;

		std::atomic<bool> g_active{ false };
		uint32_t g_world{ 0 };

		// A freed geometry's address can come back as another shape, so an answer is only good for the same
		// name and the same reference. Comparing the pooled name pointer and the form ID reads no string.
		struct Draw
		{
			const char* name{ nullptr };
			RE::FormID ref{ 0 };
			bool listed{ false };
		};
		std::unordered_map<RE::BSGeometry*, Draw> g_draws;
		std::unordered_set<std::string> g_routedKeys;
		uint32_t g_notListedLogged{ 0 };

		std::unordered_map<RE::FormID, bool> g_refs;
		uint32_t g_groundLogged{ 0 };
		bool g_noRefLogged{ false };

		const char* ModelOf(RE::TESObjectREFR* a_ref)
		{
			auto* base = a_ref ? a_ref->GetBaseObject() : nullptr;
			const auto* model = base ? base->As<RE::TESModel>() : nullptr;
			return model ? model->GetModel() : nullptr;
		}

		bool Resolve(RE::BSGeometry* a_geometry, RE::TESObjectREFR* a_ref, const char* a_name)
		{
			if (!a_ref || a_ref->As<RE::Actor>() || a_geometry->GetGeometryRuntimeData().skinInstance) { return false; }
			const char* model = ModelOf(a_ref);
			if (!model || !*model) { return false; }
			const auto modelKey = SnowShapeList::Model(model);
			if (!g_models.contains(modelKey)) { return false; }
			const auto key = SnowShapeList::Key(modelKey, a_name ? a_name : "");
			const bool listed = g_shapes.contains(key);
			if (listed) {
				if (g_routedKeys.insert(key).second) {
					logger::info("Snow shapes: routed {}/{}: {}", g_routedKeys.size(), g_shapes.size(), key);
				}
			} else if (g_notListedLogged < kMaxNotListedLogged) {
				++g_notListedLogged;
				logger::info("Snow shapes: left native, in a listed model but not listed: {}", key);
			}
			return listed;
		}

		bool RefIsListed(RE::TESObjectREFR* a_ref)
		{
			const auto id = a_ref->GetFormID();
			if (const auto it = g_refs.find(id); it != g_refs.end()) { return it->second; }
			const char* model = ModelOf(a_ref);
			const bool listed = model && g_models.contains(SnowShapeList::Model(model));
			g_refs.emplace(id, listed);
			if (g_groundLogged < kMaxGroundLogged) {
				++g_groundLogged;
				logger::info("Snow shapes: ground ray hit ref {:08X} model {} -> {}", id, model ? model : "<none>",
					listed ? "listed" : "not listed");
			}
			return listed;
		}
	}

	std::filesystem::file_time_type ListWriteTime()
	{
		std::error_code ec;
		const auto when = std::filesystem::last_write_time(kPath, ec);
		return ec ? std::filesystem::file_time_type{} : when;
	}

	void Load()
	{
		g_shapes.clear();
		g_models.clear();
		std::ifstream file(kPath);
		if (!file) {
			logger::info("Snow shapes: no {}, none listed", kPath);
			return;
		}
		std::string line, model, key;
		while (std::getline(file, line)) {
			if (SnowShapeList::Parse(line, model, key)) {
				g_shapes.insert(key);
				g_models.insert(model);
			}
		}
		logger::info("Snow shapes: {} shape(s) in {} model(s) listed", g_shapes.size(), g_models.size());
	}

	void SetWorld(uint32_t a_world)
	{
		const bool active = a_world != 0 && !g_shapes.empty();
		const bool wasActive = g_active.load(std::memory_order_relaxed);
		if (active) {
			logger::info("Snow shapes: world {:08X} active, {} shape(s) listed", a_world, g_shapes.size());
		} else if (a_world != 0) {
			logger::info("Snow shapes: world {:08X} allowed, no shapes listed, nothing routed", a_world);
		} else if (wasActive) {
			logger::info("Snow shapes: off, {} of {} listed shape(s) were routed", g_routedKeys.size(), g_shapes.size());
		}
		g_world = a_world;
		g_draws.clear();
		g_routedKeys.clear();
		g_notListedLogged = 0;
		g_refs.clear();
		g_groundLogged = 0;
		g_noRefLogged = false;
		g_active.store(active, std::memory_order_relaxed);
	}

	bool Active()
	{
		return g_active.load(std::memory_order_relaxed);
	}

	bool Contains(RE::BSGeometry* a_geometry)
	{
		if (!a_geometry) { return false; }
		auto* ref = a_geometry->GetUserData();
		const RE::FormID refID = ref ? ref->GetFormID() : 0;
		const char* name = a_geometry->name.c_str();

		if (const auto it = g_draws.find(a_geometry); it != g_draws.end() &&
			it->second.name == name && it->second.ref == refID) {
			return it->second.listed;
		}

		const bool listed = Resolve(a_geometry, ref, name);
		if (g_draws.size() >= kMaxDraws) { g_draws.clear(); }
		g_draws[a_geometry] = { name, refID, listed };
		return listed;
	}

	bool OnListedShape(const RE::NiPoint3& a_at, float& a_surfaceZ)
	{
		auto* tes = RE::TES::GetSingleton();
		if (!tes) { return false; }

		const float scale = RE::bhkWorld::GetWorldScale();
		const float fromZ = a_at.z + kRayAbove;
		const float toZ = a_at.z - kRayBelow;

		RE::bhkPickData pick;
		pick.rayInput.from = RE::hkVector4(a_at.x * scale, a_at.y * scale, fromZ * scale, 0.0f);
		pick.rayInput.to = RE::hkVector4(a_at.x * scale, a_at.y * scale, toZ * scale, 0.0f);
		// The pathing pick meets statics, ground and terrain, and passes through actors and clutter
		// (the COLL records in Skyrim.esm), so the ray does not stop on the feet it starts from.
		pick.rayInput.filterInfo.SetCollisionLayer(RE::COL_LAYER::kPathPick);
		tes->Pick(pick);

		if (!pick.rayOutput.HasHit() || !pick.rayOutput.rootCollidable) { return false; }
		auto* ref = RE::TESHavokUtilities::FindCollidableRef(*pick.rayOutput.rootCollidable);
		if (!ref) {
			if (!g_noRefLogged) {
				g_noRefLogged = true;
				logger::info("Snow shapes: ground ray hit something with no reference (land, or an unowned body) at ({:.0f}, {:.0f}, {:.0f})",
					a_at.x, a_at.y, a_at.z);
			}
			return false;
		}
		if (!RefIsListed(ref)) { return false; }

		a_surfaceZ = fromZ + (toZ - fromZ) * std::clamp(pick.rayOutput.hitFraction, 0.0f, 1.0f);
		return true;
	}

	Surfaces::Ground ShapeGround()
	{
		Surfaces::Ground ground{};
		ground.type = Surfaces::Type::kSnow;
		ground.response = Surfaces::ResponseFor(Surfaces::Type::kSnow);
		return ground;
	}
}
