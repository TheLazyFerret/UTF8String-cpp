//! Test of the UTF8String library.

#define BOOST_TEST_MODULE utf8StringTest
#include <boost/test/unit_test.hpp>

#include "utf8_string.hpp"

#include <stdexcept>
#include <string>
#include <string_view>

constexpr std::string_view EXAMPLE_UNICODE = "ඞ";
constexpr std::string_view EXAMPLE_ASCII_SYMBOL = "a";

/// Generates a bad code unicode.
std::string bad_unicode() noexcept {
  std::string result(EXAMPLE_UNICODE);
  result.pop_back();
  result.push_back('a');
  return result;
}

using namespace utf8_string;

// Check the constructor, and implictly the parser.
BOOST_AUTO_TEST_CASE(UTF8Symbol_constructor_test1) {
  const UTF8Symbol symbol(EXAMPLE_UNICODE);

  BOOST_ASSERT(symbol.to_string() == EXAMPLE_UNICODE);
}

BOOST_AUTO_TEST_CASE(UTF8Symbol_constructor_test2) {
  const UTF8Symbol symbol(EXAMPLE_ASCII_SYMBOL);

  BOOST_ASSERT(symbol.to_string() == EXAMPLE_ASCII_SYMBOL);
}

BOOST_AUTO_TEST_CASE(UTF8Symbol_constructor_test3) {
  const std::string bad_symbol = bad_unicode();

  BOOST_CHECK_THROW(UTF8Symbol{bad_symbol}, std::invalid_argument);
}
