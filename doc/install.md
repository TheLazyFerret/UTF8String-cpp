# Instalation

## Requirements

Obligatory:

- A c++ compiler (tested with [clang](https://clang.llvm.org/)) with atleast `cxx_std >= 20`.
- Cmake with atleast the version `3.30`.

Optional:

- [`Boost.Test`](https://www.boost.org/library/latest/test/) for run the tests.

## Using FetchContent (preferred)

In the cmake of your project, place the next code (After the `project` command):

```cmake
include(FetchContent)

FetchContent_Declare(
    UTF8String
    GIT_REPOSITORY https://github.com/TheLazyFerret/UTF8String-cpp.git
    GIT_TAG 0.2.0
)

FetchContent_MakeAvailable(UTF8String)
```

After this, the library should be avaible for your project. Remember compile with optimizations for release builds.

## Alternative traditional installation

First you will need to retrieve the source code.
For doing so, just git clone the latest commit in main branch.

```bash
git clone --branch main https://github.com/TheLazyFerret/UTF8String-cpp.git
```

After that, you will need to build the project:

```bash
cd UTF8String-cpp
cmake --preset release
cmake --build --preset release
```

Now, you can install the library locally:

```bash
cmake --install build/release
```

If you want to override the installation location, instead do:

```bash
cmake --install build/release --prefix ./install
```

## Uninstallation

For uninstalling the library, just remove the copied files. You can see the list in the generated `install_manifest.txt` file inside the `build/release` directory.
