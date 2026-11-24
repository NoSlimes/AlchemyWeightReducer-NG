# AlchemyWeightReducer-NG

A lightweight, native SKSE plugin (`CommonLibSSE-NG`) that automatically adjusts the weight of **all** ingredients, potions, poisons, and (optionally) food—including items added by mods and player-crafted potions.

<img width="1106" height="1307" alt="image" src="https://github.com/user-attachments/assets/e1397922-28d9-4820-9a01-56ae387778c6" />


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
Requires Visual Studio (C++23), CMake, and `vcpkg`:
```powershell
git clone --recursive https://github.com/NoSlimes/AlchemyWeightReducer-NG.git
cd AlchemyWeightReducer-NG
build.cmd x64-release
```
---

## Third-Party Notices & License
* Licensed under **GNU GPL v3.0-or-later** (see [LICENSE](LICENSE)).
* Built with [CommonLibSSE-NG](https://github.com/alandtse/CommonLibSSE-NG) (GPL-3.0-or-later).
* Uses [SimpleIni](https://github.com/brofield/simpleini) (MIT License).
