//! Test of the UTF8String library.

#include "utf8_symbol.hpp"
#define BOOST_TEST_MODULE utf8StringTest
#include <boost/test/unit_test.hpp>

#include "utf8_string.hpp"

#include <stdexcept>
#include <string>
#include <string_view>
#include <sstream>

constexpr std::string_view EXAMPLE_UNICODE = "ඞ";
constexpr char EXAMPLE_ASCII = 'a';
constexpr char EXAMPLE_BAD_ASCII = 0xFFFF;

/// Generates a bad code unicode.
std::string bad_unicode() noexcept {
  std::string result(EXAMPLE_UNICODE);
  result.pop_back();
  result.push_back('a');
  return result;
}

using namespace utf8_string;

// Check the constructor, and implictly the parser.
BOOST_AUTO_TEST_CASE(UTF8Symbol_UNICODE_constructor_test1) {
  const UTF8Symbol symbol(EXAMPLE_UNICODE);

  BOOST_TEST(symbol.to_string() == EXAMPLE_UNICODE);
}

BOOST_AUTO_TEST_CASE(UTF8Symbol_constructor_test3) {
  const std::string bad_symbol = bad_unicode();

  BOOST_CHECK_THROW(UTF8Symbol{bad_symbol}, std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(UTF8Symbol_ASCII_constructor) {
  BOOST_CHECK_THROW(UTF8Symbol{EXAMPLE_BAD_ASCII}, std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(UTF8Symbol_stream_extract_test) {
  const std::string str = "   ඞඞඞ";
  std::stringstream sstream(str);
  UTF8Symbol symbol1, symbol2, symbol3;
  sstream >> symbol1 >> symbol2 >> symbol3;
  BOOST_TEST((!sstream.fail() || !sstream.eof()));
  BOOST_TEST(symbol1.to_string() == EXAMPLE_UNICODE);
  BOOST_TEST(symbol2.to_string() == EXAMPLE_UNICODE);
  BOOST_TEST(symbol3.to_string() == EXAMPLE_UNICODE);
}
