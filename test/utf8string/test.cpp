//! Test of the UTF8String library.

#include "utf8_symbol.hpp"
#include <cstdint>
#define BOOST_TEST_MODULE utf8StringTest
#include <boost/test/unit_test.hpp>

#include "utf8_string.hpp"

#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

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

BOOST_AUTO_TEST_CASE(UTF8Symbol_to_integer_test) {
  static constexpr std::string_view unicode_1 = "𰜩";
  static constexpr std::uint32_t unicode_1_value = 0xF0B09CA9;
  static constexpr std::string_view unicode_2 = "a";
  static constexpr std::uint32_t unicode_2_value = static_cast<std::uint32_t>('a');
  const UTF8Symbol symbol_1(unicode_1);
  const UTF8Symbol symbol_2(unicode_2);
  BOOST_TEST(unicode_1_value == symbol_1.to_integer());
  BOOST_TEST(unicode_2_value == symbol_2.to_integer());
}

BOOST_AUTO_TEST_CASE(UTF8Symbol_comparison_test) {
  const UTF8Symbol unicode_1("a");
  const UTF8Symbol unicode_2("ʘ");
  const UTF8Symbol unicode_3("𰜩");
  BOOST_TEST(unicode_1 < unicode_2);
  BOOST_TEST(unicode_2 < unicode_3);
  BOOST_TEST(!(unicode_3 < unicode_3));
}

BOOST_AUTO_TEST_CASE(UTF8String_constructor) {
  const std::string str_og = "   ඞඞඞ";
  UTF8String str(str_og);
  BOOST_TEST((str_og == str.to_string()));
}
