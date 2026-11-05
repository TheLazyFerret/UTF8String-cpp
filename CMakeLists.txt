### Main CMakeLists of the project.
cmake_minimum_required(VERSION 3.30)

# Project declaration.
project(UTF8String
  VERSION 0.0    # Version of the program.
  LANGUAGES CXX  # Programming language of the project.
)

# Definitions for install (the paths where stuff will be placed).
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Add option for testing.
option(TESTING "Enable test compilations" OFF)

# Create the compile_commands.json (mostly for clangd).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the namespace for the project.
set(namespace UTF8String)

# Add the static library for UTF8String.
add_library(${PROJECT_NAME} STATIC)

# Alias for FetchContent.
add_library("${namespace}::${PROJECT_NAME}" ALIAS ${PROJECT_NAME})

# Set the headers of the library.
set(public_headers
  include/utf8_symbol.hpp
  include/utf8_string.hpp
)

# Source files of the project.
target_sources(${PROJECT_NAME}
  PRIVATE
  src/utf8_string.cpp
)

# Set the public headers of the library as a property of the target.
set_target_properties(${PROJECT_NAME}
  PROPERTIES PUBLIC_HEADER
  "${public_headers}"
)

# Compilation warnings and stuff.
target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall
  -Wextra
  -pedantic
)

# The C++ standard used.
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# Location of the header files of the library.
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
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

# Install command for the sources and headers.
install(TARGETS ${PROJECT_NAME}
  EXPORT "${PROJECT_NAME}Targets"
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export file.
install(EXPORT "${PROJECT_NAME}Targets"
  FILE "${PROJECT_NAME}Targets.cmake"
  NAMESPACE ${namespace}::
  DESTINATION cmake
)

# Generate the version config file.
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY AnyNewerVersion
)

# Create the config file.
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION cmake
)

# Install config files.
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION cmake
)
