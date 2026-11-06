Dynamic Terrain Deformation Bloodpool Patch
Version 1.1.0

Dynamic Bloodpool Framework's pools lie on the snow you see: on top of raised snow, down in trenches and footprints,
and over the rims between them. Without it a pool sits at the bare land height under the snow.

Requires: Dynamic Terrain Deformation skse 1.2, Dynamic Bloodpool Framework.

Install: below Dynamic Terrain Deformation, so this NMN_DeformableTerrain.dll wins. Keep that mod enabled: its INI and
Surfaces folders are still used, and this patch ships no INI.

Setting: EnableBloodPools = 1 is the default. Add EnableBloodPools = 0 to NMN_DeformableTerrain.ini to compare, then
save; with Dynamic Terrain Deformation's live reload on, a running game picks it up within about half a second.

A pool on a rock, bridge or floor is left as it was. A pool that starts on land and spreads onto a rock lifts with the
snow beside it.

Snow meshes in a city (off unless a list names the world): a city world normally keeps the terrain idle. A snow shape
list turns a town on: a .txt in SKSE\Plugins\NMN_DeformableTerrain_SnowShapes\ with a "world = 0001691D" line and the
walked-on shapes, one "model path | shape name" a line; a "requires = Some.esp" line skips the list unless that plugin is
loaded. This package ships no such list, so every city stays idle as
before. CityWorldsAllowed in NMN_DeformableTerrain.ini names a world too, and SKSE\Plugins\
NMN_DeformableTerrain_SnowShapes.txt still works as a list. List no roofs: the prints have no height, so a listed roof
shows the prints of the street under it.

Licence: GPL-3.0-only. Based on Dynamic Terrain Deformation by NearMidnightNow, source at
https://github.com/maglarnet/Dynamic_Terrain_Deformation_Skse (branch 1.7, commit b83ebf3).
This patch's own source: https://github.com/ApocryphaRealm/DTDBloodpoolPatch (release tag v1.1.0).

Known: Dynamic Terrain Deformation 1.3 has the blood pool change built in; once it is out, this patch is not needed
for blood pools. As a pool appears, thin lines can show across it (zero-area triangles in the pool mesh).
