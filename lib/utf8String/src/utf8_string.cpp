//! Source code of the utf8 string.

#include "utf8_string.hpp"
#include "impl.hpp"

#include <array>
#include <stdexcept>

using namespace utf8_string;
using namespace utf8_string::impl;

#define CHECK_SIZE(size, expected_size, msg)                                                                           \
  if (size != expected_size) {                                                                                         \
    throw std::invalid_argument(msg);                                                                                  \
  }

/// Byte parser for an UTF-8 codified unicode symbol.
/// It should be allocation free. Sadly it can't be evaluated at compile time,
/// because std::string_view::size() is not constexpr.
UTF8Symbol UTF8Symbol::decode_str(std::string_view str) {
  const char* const ERROR_MESSAGE = "The raw str is not a UTF-8 symbol.";
  if (!(str.size() >= 1 && str.size() <= 4)) {
    throw std::invalid_argument(ERROR_MESSAGE);
  }
  UTF8Symbol result;
  const char8_t first_char = static_cast<char8_t>(str.front());
  // The unicode symbol is two byte long.
  if (test_8byte(first_char, F_BYTE_MASK[0], F_BYTE_RESULT[0])) {
    CHECK_SIZE(str.size(), 1, ERROR_MESSAGE)
    result.inner_data = {first_char, '\0', '\0', '\0'};
    result.size = 1;
    return result;
  }
  // The unicode symbol is two byte long.
  else if (test_8byte(first_char, F_BYTE_MASK[1], F_BYTE_RESULT[1])) {
    CHECK_SIZE(str.size(), 2, ERROR_MESSAGE)
    const char8_t second_char = static_cast<char8_t>(str.at(1));
    if (test_8byte(second_char, NF_BYTE_MASK, NF_BYTE_RESULT)) {
      throw std::invalid_argument(ERROR_MESSAGE);
    }
    result.inner_data = {first_char, second_char, '\0', '\0'};
    result.size = 2;
    return result;
  }
  // The unicode symbol is thre byte long.
  else if (test_8byte(first_char, F_BYTE_MASK[2], F_BYTE_RESULT[2])) {
    CHECK_SIZE(str.size(), 3, ERROR_MESSAGE)
    const char8_t second_char = static_cast<char8_t>(str.at(1));
    const char8_t third_char = static_cast<char8_t>(str.at(2));
    if (!test_8byte(second_char, NF_BYTE_MASK, NF_BYTE_RESULT) ||
        !test_8byte(third_char, NF_BYTE_MASK, NF_BYTE_RESULT)) {
      throw std::invalid_argument(ERROR_MESSAGE);
    }
    result.inner_data = {first_char, second_char, third_char, '\0'};
    result.size = 3;
    return result;
  }
  // The unicode symbol is four byte long.
  else if (test_8byte(first_char, F_BYTE_MASK[3], F_BYTE_RESULT[3])) {
    CHECK_SIZE(str.size(), 4, ERROR_MESSAGE)
    const char8_t second_char = static_cast<char8_t>(str.at(1));
    const char8_t third_char = static_cast<char8_t>(str.at(2));
    const char8_t fourth_char = static_cast<char8_t>(str.at(3));
    if (!test_8byte(second_char, NF_BYTE_MASK, NF_BYTE_RESULT) ||
        !test_8byte(third_char, NF_BYTE_MASK, NF_BYTE_RESULT) ||
        !test_8byte(fourth_char, NF_BYTE_MASK, NF_BYTE_RESULT)) {
      throw std::invalid_argument(ERROR_MESSAGE);
    }
    result.inner_data = {first_char, second_char, third_char, fourth_char};
    result.size = 4;
    return result;
  }
  // The first byte is bad coded.
  throw std::invalid_argument(ERROR_MESSAGE);
}

#undef CHECK_SIZE
