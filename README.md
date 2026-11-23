# foo_cuefixer

Simple component for foobar2000 to fix CUE/FLAC problems

[![Made in Ukraine](https://img.shields.io/badge/made_in-ukraine-ffd700.svg?labelColor=0057b7)](https://stand-with-ukraine.pp.ua)

* Removes duplicate song entries that appear when adding adirectory containing
  a `.cue` file that links to timestamps in a base `.flac`/`.wav`/`.ape` file.
* Removes such duplicates also when adding tracks from media library.
* Removes broken `.cue` files that point to missing files (`.flac`/`.wav`/`.ape`
  etc).

## Supported platforms

Windows x86, x64, ARM

## Installation

Download component from foobar or from releases
1. https://www.foobar2000.org/components/view/foo_cuefixer
2. Open `foo_cuefixer.fb2k-component` to install it into foobar2000
3. Remove (if it exists) the `*.cue` exclusion from the list at
   **Preferencess** -> **Shell Integration** -> **Exclude file types**

## Release version download

Get it on GitHub from
[Releases](https://github.com/RevenantX/foo_cuefixer/releases)

## Building

1. Install CMake and Visual C++ Build Tools, including MSBuild, ATL, and ARM64
   compiler support. The Visual Studio IDE is not required.
2. Download the [foobar2000 SDK](https://www.foobar2000.org/SDK). Place WTL 10
   at `..\WTL` next to the SDK directory.
3. If needed, update `sdk_path` in [build_sdk.bat](./build_sdk.bat) and
   [build_foo_cuefixer.bat](./build_foo_cuefixer.bat), then run
   `build_foo_cuefixer.bat`.

The script builds Release DLLs for x86, x64, and ARM64EC in the `build32`,
`build64`, and `buildarm64ec` directories. ARM64EC is the ARM target provided
by this SDK; it does not include native ARM64 libraries.
