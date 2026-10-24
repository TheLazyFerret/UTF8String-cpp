# Add the test, that at the end is just an executable.
add_executable(UTF8String_test ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)

# Link the library to test. In this case, the utf8_string library.
# Also link the Boost module for testing.
target_link_libraries(UTF8String_test PRIVATE
  Boost::unit_test_framework
  UTF8String
)

target_compile_features(UTF8String_test PRIVATE cxx_std_20)

# Add the test for cmake and ctest.
add_test(NAME UTF8StringTest
  COMMAND UTF8String_test
)
