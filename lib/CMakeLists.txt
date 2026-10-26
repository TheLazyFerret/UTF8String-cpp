### Simple static library definition for utf8String.

add_library(UTF8String STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utf8_string.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utf8_symbol.cpp
)

# Mostly compile warnings.
target_compile_options(UTF8String PRIVATE
  -Wall
  -Wextra
  -pedantic
)

# Set the C++ version used for compiling.
target_compile_features(UTF8String PRIVATE cxx_std_20)

# Set the include directories.
# One public, the other is the private implementation.
target_include_directories(UTF8String
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
