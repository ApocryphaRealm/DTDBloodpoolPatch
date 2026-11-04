# DTDBloodpoolPatch - progress

Fork of Dynamic Terrain Deformation at branch 1.7 commit b83ebf3, the source of the shipped 1.2 DLL. The remote named
`upstream` has its push URL disabled. Stay merge-friendly: the patch touches BloodDecals.cpp, BloodDecalFilter.h and one
setting; the rest of the diff is build identity (version resource, symbols, path trimming).

## Why pools misbehave

- DBF: `PerformZRaycast` and the placement ray pick Havok collision (`COL_LAYER::kPathPick`). DTD changes no collision,
  so DBF always gets the bare land height. fHeightOffset only adds 1.5.
- DTD: raise and trenches exist only in a generated domain shader: `rise = Displacement(xy) + SnowRaise(xy)`, from
  GPU textures. The native-blood route (Mode::kBloodDecal) applies the same `rise` to a decal draw.
- `BloodDecals::MayContain` rejects skinned geometry, and DBF's four pool meshes are skinned (BSDismemberSkinInstance
  on every BSTriShape). So pools never take that route.

## Snow shapes in a city world (1.1.0, built 2026-11-04, unproven in game)

The owner chose option C of `4. plans\windhelm-deformable-snow-2026-11-04` for Windhelm Is Snowy: its street snow is
static mesh about 900 units above Tamriel's land, in a city world. Five things stood in the way, walked by hand:

1. `Hooks.cpp` idles a city world. `CityWorldsAllowed` (form IDs, empty by default) lifts that per world.
2. Statics only had the probe route (constant lift, no subdivision, no field). `Mode::kSnowShape` is the field route of
   `kBloodDecal` with normals recomputed and no raise. The raise reads land and shelter under the mesh, and would lift the
   mesh by what the land beneath it holds, so it is off; a print is a dent below the mesh surface.
3. Which shapes: `SnowShapes::Contains`, from `SKSE\Plugins\NMN_DeformableTerrain_SnowShapes.txt`, model path plus shape
   name. One hash lookup per draw; strings are read once per geometry, and an answer holds only for the same pooled name
   pointer and form ID, since a freed geometry's address can come back as another shape.
4. Stamp gates measured feet against land. `SnowShapes::OnListedShape` rays down with the pathing pick layer (static,
   ground and terrain; not actors or clutter, per Skyrim.esm's COLL records), and only a hit on a listed model counts.
   Any other hit keeps the land path, which refuses as before.
5. `ClipmapUpdateCS.h` floors a snow-flagged stamp at the blanket it reads from the land (`h = max(h, -(SnowBlanket +
   Raise.z))`). Under a street that blanket is nothing, so such a stamp would dig nothing. Shape stamps therefore carry
   `snow = false` and use the ordinary depth. Cost: they take the general rim settings, not the snow rim ones.

The field itself has no height: a print shows on every listed shape over that spot, which is why this is a list and not
a material test. Coverage and shelter keep reading land in an allowed world; only land uses them.

Not built: magic and arrow impacts on the mesh (their gates still measure land). Heat melts still read the land type under
the mesh, so a fire over snow-typed land dents a listed shape.

Long edges: the hull stage stops at factor 64, so a 1742-unit edge gets a vertex every 27 units at best and prints there
look soft. Going finer means splitting those triangles in the NIF.

## Open

- [x] Built, packaged, recorded working 2026-11-03: the owner's hands-on test, combined run B-20261103-173705-9d34, SE 1.5.97.
- [x] Public repo; compatibility verdict and Nexus text written 2026-11-03.
- [ ] Frame time never measured. These are skinned meshes on the tessellation path, so that is the first thing to check.
- [ ] Snow shapes: first in-game tests (Test 1 decides K1). Episode 113 in the falsification log holds the killing conditions.
- [ ] Offered upstream as issue 11 on maglarnet/Dynamic_Terrain_Deformation_Skse. Retire this patch if it is merged.
