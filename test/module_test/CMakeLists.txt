### CMakeLists.txt template for a module.

### Add the unit test source file.
add_executable(module_test module_test.cpp)

### Statically link the Boost library.
target_link_libraries(module_test PRIVATE
  Boost::unit_test_framework
  module
)

### Include the module to test directory.
target_include_directories(module_test PRIVATE ${PROJECT_SOURCE_DIR}/src/)

### Add the test to cmake.
add_test(NAME module_test COMMAND module_test)
