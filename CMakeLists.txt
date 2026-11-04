### Main CMakeLists of the project.
cmake_minimum_required(VERSION 3.30)

# Project declaration.
project(UTF8String-cpp
  VERSION 0.0    # Version of the program.
  LANGUAGES CXX  # Programming language of the project.
)

# Add option for testing.
option(TESTING "Enable test compilations" OFF)

# Create the compile_commands.json (mostly for clangd).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add the static library.
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

# Special rules for DEBUG builds.
if (TESTING)
  # Generate the necessary metadata for using ctest.
  enable_testing()
  # Ensure the Boost library for testing exist.
  find_package(Boost COMPONENTS unit_test_framework REQUIRED)
  # Add the testing subdirectories.
  add_subdirectory(test/)
endif()
