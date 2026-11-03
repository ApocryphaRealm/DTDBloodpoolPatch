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

- [ ] Build. Refused 2026-11-03 by the auto-mode classifier ("Code from External") on `configure.bat`: the owner has to
      allow building this cloned repo. Then: health gate, `configure.bat`, `build.bat`, `package-mod.ps1 -Mod DTDBloodpoolPatch`.
- [ ] Confirm the build matches shipped behaviour with EnableBloodPools=0.
- [ ] In-game test (steps in the hand-off). Pool on land only; a pool spreading onto a rock will lift with the snow.
- [ ] Before any release: public repo (GPL needs the source available), compatibility verdict, Nexus text.
