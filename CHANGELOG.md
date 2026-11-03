# DTDBloodpoolPatch - changelog

Written as changes happen, not reconstructed afterwards (rule 61). Each version carries its
**version-ledger status**, so this file cannot quietly claim more than the ledger does:

* **working** - observed running in game
* **untested** - built and packaged, not yet confirmed
* **failed** - built but crashed or malfunctioned; the number was reclaimed
* **scratch** - a hypothesis-test build that never held a real number

## 1.0.0 - 2026-11-03 - untested

### Added
- Dynamic Bloodpool Framework pools on land are drawn through Dynamic Terrain Deformation's blood decal path, so they take the same raise and the same trenches as the ground under them.
- EnableBloodPools setting, on by default; 0 puts pools back at bare land height. It reloads live.
- Log lines for the first 32 pool shapes seen after a load: position, land height, and whether the shape follows the ground.
