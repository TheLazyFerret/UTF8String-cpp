//! Test of the UTF8String library.

#define BOOST_TEST_MODULE utf8StringTest
#include <boost/test/unit_test.hpp>

#include "utf8_string.hpp"
#include "utf8_symbol.hpp"

#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

constexpr std::string_view EXAMPLE_UNICODE = "ඞ";
constexpr char EXAMPLE_ASCII = 'a';
constexpr char EXAMPLE_BAD_ASCII = 0xFF;

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
  BOOST_TEST((unicode_1 < unicode_2) == true);
  BOOST_TEST((unicode_2 < unicode_3) == true);
  BOOST_TEST((unicode_1 <= unicode_1) == true);
  BOOST_TEST((unicode_1 <= unicode_1) == true);
  BOOST_TEST((unicode_3 >= unicode_2) == true);
  BOOST_TEST((unicode_1 > unicode_3) == false);
  BOOST_TEST((unicode_1 > unicode_1) == false);
}

BOOST_AUTO_TEST_CASE(UTF8String_constructor_test) {
  const std::string str_og = "   ඞඞඞ";
  UTF8String str(str_og);
  BOOST_TEST((str_og == str.to_string()));
}

BOOST_AUTO_TEST_CASE(UTF8String_appending_test) {
  const std::string str_a = "ඞඞඞa";
  const std::string str_b = "aඞඞඞ";
  const std::string str_c = str_a + str_b;
  const UTF8String aux_a(str_a);
  const UTF8String aux_b(str_b);
  const UTF8String aux_c = aux_a + aux_b;
  BOOST_TEST(aux_c.to_string() == str_c);
}

BOOST_AUTO_TEST_CASE(UTF8Symbol_valid_test) {
  std::string str_a(EXAMPLE_UNICODE);
  BOOST_TEST(is_symbol_utf8_valid(str_a) == true);
  BOOST_TEST(is_symbol_utf8_valid(EXAMPLE_ASCII) == true);
  str_a.pop_back();
  BOOST_TEST(is_symbol_utf8_valid(str_a) == false);
  BOOST_TEST(is_symbol_utf8_valid(EXAMPLE_BAD_ASCII) == false);
}

BOOST_AUTO_TEST_CASE(UTF8String_valid_test) {
  std::string str = "aඞඞඞ";
  BOOST_TEST(is_string_utf8_valid(str) == true);
  str.pop_back();
  BOOST_TEST(is_string_utf8_valid(str) == false);
}

BOOST_AUTO_TEST_CASE(UTF8String_equality_test) {
  UTF8String a("ඞඞඞ");
  UTF8String b("amogus");
  BOOST_TEST((a == a) == true);
  BOOST_TEST((a == b) == false);
}

BOOST_AUTO_TEST_CASE(UTF8String_find_string) {
  const UTF8String a("aaaඞඞඞaaa");
  const UTF8String b("ඞඞඞ");
  const UTF8String c("");
  BOOST_TEST(a.find(b, 1) == 3);
  BOOST_TEST(b.find(a) == UTF8String::npos);
  BOOST_TEST(a.find(b, 3) == 3);
  BOOST_TEST(a.find(b, 4) == UTF8String::npos);
  BOOST_TEST(a.find(a) == 0);
  BOOST_TEST(c.find(c) == 0);
  BOOST_TEST(a.find(a, 1) == UTF8String::npos);
}

BOOST_AUTO_TEST_CASE(UTF8String_find_symbol) {
  const UTF8String a("aaaඞඞඞaaa");
  const UTF8Symbol amogus("ඞ");
  BOOST_TEST(a.find(amogus) == 3);
  BOOST_TEST(a.find(amogus, 3) == 3);
  BOOST_TEST(a.find(amogus, 4) == 4);
  BOOST_TEST(a.find(amogus, 5) == 5);
  BOOST_TEST(a.find(amogus, 6) == UTF8String::npos);
}

BOOST_AUTO_TEST_CASE(UTF8String_substr) {
  const UTF8String str("aඞaඞaඞ");
  const UTF8String substr1("ඞaඞaඞ");
  const UTF8String substr2("ඞaඞ");
  const UTF8String substr3("ඞ");
  BOOST_TEST(str.substr() == str);
  BOOST_TEST(str.substr(1) == substr1);
  BOOST_TEST(str.substr(1, 3) == substr2);
  BOOST_TEST(str.substr(1, 1) == substr3);
}