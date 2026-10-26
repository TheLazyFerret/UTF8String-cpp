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

add_subdirectory(lib/utf8string)

# Special rules for DEBUG builds.
if (TESTING)
  # Generate the necessary metadata for using ctest.
  enable_testing()
  # Ensure the Boost library for testing exist.
  find_package(Boost COMPONENTS unit_test_framework REQUIRED)
  # Add the testing subdirectories.
  add_subdirectory(test/utf8string)
endif()
