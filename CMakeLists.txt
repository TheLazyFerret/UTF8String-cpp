### Main CMakeLists of the project.
cmake_minimum_required(VERSION 3.30)

# Project declaration.
project(ferret-lib
  VERSION 0.0    # Version of the program.
  LANGUAGES CXX  # Programming language of the project.
)

# Create the compile_commands.json (mostly for clangd).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib/utf8String)

# Special rules for DEBUG builds.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  # Generate the necessary metadata for using ctest.
  enable_testing()
  # Ensure the Boost library for testing exist.
  find_package(Boost COMPONENTS unit_test_framework REQUIRED)
  # Add the testing subdirectories.
  add_subdirectory(test/utf8String)
endif()
