# Prior art for the Dynamic Terrain Deformation blood pool patch (searched 2026-11-03)

What the patch needs to exist: Dynamic Bloodpool Framework's pools drawn at the height of the snow that Dynamic
Terrain Deformation raises and presses, including trenches.

## Nexus (Skyrim SE, v2 GraphQL search by name and by description)

Names searched: Dynamic Terrain Deformation, Terrain Deformation, Bloodpool, Blood pool, Blood Pools, blood snow,
Deformable Terrain, DTD patch, NMN_DeformableTerrain, Snow in Skyrim.
Descriptions searched: Dynamic Terrain Deformation, Dynamic Bloodpool Framework, blood pool deformed snow, DTD blood.

- Dynamic Terrain Deformation skse (192402): lists "Blood on snow - supported" for Dynamic Bloodpool Framework,
  Sanguine Symphony and Core Impact Framework, and credits a blood decal fix. Its source routes only the decal
  manager's terrain decals (src/BloodDecals.cpp), and turns skinned geometry away. DBF pools are skinned meshes, so
  the claim holds for the frameworks' native decals and not for the pools.
- Dynamic Bloodpool Framework (172080), Dynamic Bloodpool for Dismembering Framework (192574): pool makers, no snow
  height handling.
- Responsive Grounds (197435), DynamicShader-DynamicSnow (189713), Open Shaders (180419), Bottled Shaders (192816),
  PIXL Renderer (189836), Deep Snow Makes You Slow (196715, ours): mention DTD or snow; none places pools on it.
- No patch joining DBF and DTD was found.

## GitHub (repos and code search)

Searched: DTD_1_7_public, NMN_DeformableTerrain, DeformableTerrain skyrim, Dynamic Bloodpool Framework,
DynamicBloodpoolFramework, Dynamic Terrain Deformation, "DynamicDecal DeformableTerrain",
"BloodDecals skinInstance DynamicDecal", "Dynamic Bloodpool Framework snow deformation", "DBF_API NMN", bloodpool.

- maglarnet/Dynamic_Terrain_Deformation_Skse: DTD's source, GPL-3.0-only. Branch 1.7, commit b83ebf3 (2026-10-11
  13:41 +0200), matches the shipped 1.2 DLL: PE timestamp 2026-10-11 11:41:08 UTC, and 194 of 197 log strings found in
  the DLL, the other three being debug strings that hold a newline escape. Open issue 9 is Open Shaders support; no
  issue or branch covers pools.
- Seb263/SkyrimSE_DynamicBloodpoolFramework: DBF's source, GPL-3.0 with a modding exception. Its API (DBF_API) only
  spawns pools; it has no height hook.
- Iwillbewarlock/DTD-Personal-Fork and the three forks of the DTD repo: performance and blood decal scan work, nothing
  for pools. 1001Bits/Dynamic-Terrain-Deformation: a Fallout 4 port.

## Ruling

Nothing does this. Worth making. The one better outcome would be the same few lines landing upstream in DTD, since the
patch has to replace DTD's DLL: offering it upstream is the owner's call once it is proven in game.
