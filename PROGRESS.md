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

## Open

- [x] Built, packaged, recorded working 2026-11-03: the owner's hands-on test, combined run B-20261103-173705-9d34, SE 1.5.97.
- [x] Public repo; compatibility verdict and Nexus text written 2026-11-03.
- [ ] Frame time never measured. These are skinned meshes on the tessellation path, so that is the first thing to check.
- [ ] Offered upstream as issue 11 on maglarnet/Dynamic_Terrain_Deformation_Skse. Retire this patch if it is merged.
