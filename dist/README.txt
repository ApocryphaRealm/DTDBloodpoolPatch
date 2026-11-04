Dynamic Terrain Deformation Bloodpool Patch
Version 1.1.0

Dynamic Bloodpool Framework's pools lie on the snow you see: on top of raised snow, down in trenches and footprints,
and over the rims between them. Without it a pool sits at the bare land height under the snow.

Requires: Dynamic Terrain Deformation skse 1.2, Dynamic Bloodpool Framework.

Install: below Dynamic Terrain Deformation, so this NMN_DeformableTerrain.dll wins. Keep that mod enabled: its INI and
Surfaces folders are still used, and this patch ships no INI.

Setting: EnableBloodPools = 1 is the default. Add EnableBloodPools = 0 to NMN_DeformableTerrain.ini to compare, then
save; it applies to the next frame.

A pool on a rock, bridge or floor is left as it was. A pool that starts on land and spreads onto a rock lifts with the
snow beside it.

Snow meshes in a city (off unless you turn it on): a city world normally keeps the terrain idle. To make a town's snow
meshes take footprints, name the world in NMN_DeformableTerrain.ini, for example CityWorldsAllowed = 0001691D, and list
the walked-on shapes in SKSE\Plugins\NMN_DeformableTerrain_SnowShapes.txt, one "model path | shape name" a line. This
package ships that list empty. Other worlds behave as before. List no roofs: the prints have no height, so a listed roof
shows the prints of the street under it.

Licence: GPL-3.0-only. Based on Dynamic Terrain Deformation by NearMidnightNow, source at
https://github.com/maglarnet/Dynamic_Terrain_Deformation_Skse (branch 1.7, commit b83ebf3).
