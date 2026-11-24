# AlchemyWeightReducer-NG

A lightweight, native SKSE plugin (`CommonLibSSE-NG`) that automatically adjusts the weight of **all** ingredients, potions, poisons, and (optionally) food—including items added by mods and player-crafted potions.

### Default Weights
| Category | Default Mode | Default Weight |
| :--- | :--- | :--- |
| **Ingredients** | `Flat` | `0.05` |
| **Potions** | `Flat` | `0.1` |
| **Poisons** | `Flat` | `0.05` |
| **Food / Drinks** | `Flat` | `0.1` *(disabled by default)* | 

## Requirements
* [SKSE64](https://skse.silverlock.org/)
* [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444) (supports Skyrim SE 1.5.97, AE 1.6.x, and 1.7.x+)

## Installation
1. Install with Mod Organizer 2 or Vortex (or extract `SKSE/` into your Skyrim `Data/` folder).
2. *(Optional)* Edit `SKSE/Plugins/AlchemyWeightReducer.ini` to customize weights, switch to `Multiply` mode, or disable categories.

## Building from Source
Requires Visual Studio (C++23), CMake, and `vcpkg` (with `VCPKG_ROOT` set):
```powershell
git clone --recursive https://github.com/NoSlimes/AlchemyWeightReducer-NG.git
cd AlchemyWeightReducer-NG
build.cmd x64-release