### 3.30 for correctly supporting boost libraries.
cmake_minimum_required(VERSION 3.30)

### Project declaration.
project(template_name
  VERSION 0.0    # Version of the program.
  LANGUAGES CXX  # Programming language of the project.
)

### Create the compile_commands.json (mostly for clangd).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

### Add the "Modules" subdirectories.
add_subdirectory(src/module)

### Create the executable of the program.
add_executable(${PROJECT_NAME}
  src/main.cpp
)

### Link with all the modules.
target_link_libraries(${PROJECT_NAME} module)

### Compilation options.
target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall   # More errors.
  -Wextra # Even more errors.
)

### Use the c++23 standard.
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

### Macro definitions for the program.
target_compile_definitions(${PROJECT_NAME} PRIVATE
  PROJECT_VERSION="${PROJECT_VERSION}" # Version of the program.
)

### Testing.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  ### Generate the necessary metadata for using ctest.
  enable_testing()
  ### Ensure the Boost library for testing exist.
  find_package(Boost COMPONENTS unit_test_framework REQUIRED)
  ### Add the testing subdirectories.
  add_subdirectory(test/module_test)
endif()
