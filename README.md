# Pet XP Addition (Kyubu port)

> This is a **port to Kyubu** of the original mod [`Intybyte/CubeWorld-ExpBuff-PetAddition`](https://github.com/Intybyte/CubeWorld-ExpBuff-PetAddition).
> Original author: **Intybyte**. This port is not an official release by the author.

**What it does:** Pets that fall behind their owner earn more XP.

**Original:** <https://github.com/Intybyte/CubeWorld-ExpBuff-PetAddition>

**Side:** client (`sides` in `mod.toml`).

## Installation

Requirements: **Cube World Alpha** (the 2013-07-20 `Cube.exe`, SHA-256 `84a7a132...30bf`, unpatched) and **Kyubu** (the mod loader, <https://github.com/kyubumods/kyubu>; this mod needs `kyubu = "^1.1"`, see `mod.toml`).

1. Install Kyubu with its own installer (`install\install.cmd`). The loader is not part of this mod.
2. Put **`pet_xp_addition.zip`** (do not unpack it) into the **`Mods\`** folder next to `Cube.exe`. If `pet_xp_addition.zip` is already there, replace it.
3. Start the game. `kyubu\logs\latest.log` should have a line saying `pet_xp_addition` loaded.
4. To turn the mod off or remove it, take `pet_xp_addition.zip` out of `Mods\` (or switch it off in the mod manager: **Mods** on the title screen).

The old Cube World Mod Launcher is **not needed** for this port and is not compatible with it: do not put the original mod's DLL next to it.


Mod id: `pet_xp_addition`. Source: `Mods\pet_xp_addition\src\main.cpp`.
