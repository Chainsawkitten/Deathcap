# Deathcap
[![Travis CI Build Status](https://travis-ci.org/Chainsawkitten/Deathcap.svg?branch=master)](https://travis-ci.org/Chainsawkitten/Deathcap)
[![AppVeyor CI Build Status](https://ci.appveyor.com/api/projects/status/92lhke7fq1u5h58k?svg=true)](https://ci.appveyor.com/project/Chainsawkitten/deathcap)

Deathcap is a VR horror experience made for the Large Game Project course at BTH 2017. Ride through the abandoned mine and solve the puzzles before it gets to you.

Developed in C++/OpenGL. See the [wiki](https://github.com/Chainsawkitten/Deathcap/wiki) for information about the engine.

## Building
1. Clone the GitHub repository.
2. Run `git submodule update --init`
3. Use CMake to generate make/project files for Deathcap.
4. Build Deathcap.

## Setting up game project
On Windows:
1. Open command line.
2. Go to `%appdata%/Deathcap Engine/Hymns`
3. Type `mklink /J Game <Path to repository folder>/Game`

## Measuring VRAM
Measuring VRAM requires building with Visual Studio and having the Windows SDK installed. Enable the `MeasureVram` option in CMake.

## License
Released under the [MIT license](LICENSE).

Deathcap uses several third party libraries with their own licenses:

| Library  | License |
| --- | --- |
| [AngelScript](https://github.com/IngwiePhoenix/AngelScript) | zlib |
| [assimp](https://github.com/assimp/assimp) | Modified BSD |
| [Bullet](https://github.com/bulletphysics/bullet3) | zlib |
| [Catch](https://github.com/philsquared/Catch) | BSL |
| [Compressonator](https://github.com/GPUOpen-Tools/Compressonator) | MIT |
| [Headerize](https://github.com/Chainsawkitten/Headerize) | Public domain |
| [GLEW](https://github.com/Perlmint/glew-cmake) | Modified BSD / MIT |
| [GLFW](https://github.com/glfw/glfw) | zlib / libpng |
| [GLM](https://github.com/g-truc/glm) | Modified MIT / MIT |
| [dear imgui](https://github.com/ocornut/imgui) | MIT |
| [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) | MIT |
| [JsonCpp](https://github.com/open-source-parsers/jsoncpp) | MIT / Public domain |
| [miniz](https://github.com/richgel999/miniz) | MIT |
| [OpenVR](https://github.com/ValveSoftware/openvr) | BSD |
| [SimpleIni](https://github.com/brofield/simpleini) | MIT |
| [stb](https://github.com/nothings/stb) | Public domain |
| [Steam Audio](https://valvesoftware.github.io/steam-audio/) | STEAM® AUDIO SDK License |
