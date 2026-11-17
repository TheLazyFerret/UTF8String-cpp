# Instalation

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

For uninstalling the library, just remove the copied files. You can see the list in the generated `install_manifest.txt` file inside the build directory.

## Alternative to traditional install (using FetchContent)

In the cmake of your project, place the next code (After the `project` command):

```cmake
include(FetchContent)

FetchContent_Declare(
    UTF8String
    GIT_REPOSITORY https://github.com/TheLazyFerret/UTF8String-cpp.git
    GIT_TAG main
)

FetchContent_MakeAvailable(UTF8String)
```
