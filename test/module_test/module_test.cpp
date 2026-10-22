//! Example unit test.

#define BOOST_TEST_MODULE module_test
#include <boost/test/unit_test.hpp>

#include "module/module.hpp"

BOOST_AUTO_TEST_CASE(example_test_function) {
  
  BOOST_TEST(module::func() == true);

} /// BOOST_AUTO_TEST_CASE
