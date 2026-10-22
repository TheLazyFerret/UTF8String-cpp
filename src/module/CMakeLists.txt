### Example static library "module" CMakeLists.txt

### Add the "module" as a static library.
add_library(module STATIC module.cpp)

### Compilation options.
target_compile_options(module PRIVATE
  -Wall   # More errors.
  -Wextra # Even more errors.
)

### Use the c++23 standard.
target_compile_features(module PRIVATE cxx_std_23)