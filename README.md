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

1. Download the [foobar2000 SDK](https://www.foobar2000.org/SDK), extract its
   contents to `path\to\sdk` and open the single solution file,
   `path\to\sdk\foobar2000\foo_sample\foo_sample.sln`.

   [WTL 10](https://sourceforge.net/projects/wtl/) is needed, extracted at a
   location such as `other\path\to\wtl\`. For every one of the:

      * `foo_sample`
      * `foobar2000_sdk_helpers`
      * `libPPUI`

   subprojects, edit in Visual Studio the properties for
   **All Configurations**/**All Platforms** under
   **Configuration Properties** -> **C/C++** -> **General**
   and add the WTL headers path, `other\path\to\wtl\Include`, to
   **Additional Include Directories**.

2. Build the foobar2000 sample project for `Debug` and `Release`. The included
   settings target **x86**. It's fine to generate configurations for **x64** and
   **ARM64** based on **x86**.

   Project **pfc** is special: for **Debug** and, respectively, **Release**, for
   any **Active solution platform**, select **Debug FB2k** and, respectively,
   **Release FB2K** in the Visual Studio **Configuration Manager**.

3. Switch to the **foo_cuefixer** directory and generate the Visual Studio
   solution, explicitly referencing the **Win32**, **x64** or **ARM64**
   architecture and the **foobar2000sdk** path:

   ```pwsh
   mkdir build
   cd build
   cmake .. -AWin32 -Dfoobar2000sdk="path\to\sdk"
   ```

4. Open the generated **foo_cuefixer.sln** solution, build it and use the
   resulting `build\src\<Debug|Release>\foo_cuefixer.dll` in foobar2000, under
   the `user-components` directory.
