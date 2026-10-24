//! Source code of the utf8 string.

#include "utf8_symbol.hpp"

#include <array>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <iostream>

using namespace utf8_string;

/// Send to the stream the useful bytes inside the object.
std::ostream& utf8_string::operator<<(std::ostream& os, const UTF8Symbol& symbol) {
  for (size_t i = 0; i < static_cast<size_t>(symbol.size); ++i) {
    os << static_cast<char>(symbol.inner_data[i]);
  }
  return os;
}

/// Attempt to build a UTF8Symbol from the
std::istream& utf8_string::operator>>(std::istream& is, UTF8Symbol& symbol) noexcept {
  std::array<char8_t, UTF8_SYMBOL_SIZE> buffer;
  char aux;
  size_t byte_size = 0;
  // Extract character until find the first byte of the UTF-8 secuence.
  // Discard any whitespace character.
  // Return (with the fail and/or eof bit enabled) if find an error.
  while (true) {
    aux = is.get();
    if (is.fail() || is.eof()) {
      return is;
    } else if (std::isspace(aux)) {
      continue;
    }
    // Parse the first byte. To know the size of the UTF8 symbol.
    byte_size = bytes_of_utf8(aux);
    if (byte_size <= 0) {
      is.setstate(std::ios::failbit);
      return is;
    }
    break;
  }
  assert(byte_size <= buffer.size());
  buffer[0] = static_cast<char8_t>(aux);
  // Once the first byte is parsed, we attempt to extract the rest of the bytes.
  for (size_t i = 1; i < byte_size; ++i) {
    aux = is.get();
    if (is.fail() || is.eof()) {
      return is;
    }
    // Check if this byte is a correctly encoded.
    if (test_8byte(static_cast<char8_t>(aux), NF_BYTE_MASK, NF_BYTE_RESULT)) {
      buffer[i] = static_cast<char8_t>(aux);
    } else {
      is.setstate(std::ios::failbit);
      return is;
    }
  }
  // If everything went ok, update the UTF8Symbol and return.
  symbol.inner_data = buffer;
  symbol.size = byte_size;
  return is;
}

/// Return as a pair the inner data of the Symbol. Uses a pair so it doesn't allocate anything.
std::pair<std::array<char8_t, UTF8_SYMBOL_SIZE>, size_t> UTF8Symbol::data() const noexcept {
  return std::make_pair(inner_data, static_cast<size_t>(size));
}

/// Returns the inner data (the bytes), allocating a vector with the exact
/// same size of the inner data.
std::vector<char8_t> UTF8Symbol::data_vec() const noexcept {
  std::vector<char8_t> result{inner_data.begin(), inner_data.begin() + size};
  return result;
}

/// Convert the UTF8 Symbol into a string.
/// Important to note that the resulted string will only contain one symbol,
/// but the size will be >= 1
std::string UTF8Symbol::to_string() const noexcept {
  std::string result;
  result.reserve(static_cast<size_t>(size));
  for (unsigned char i = 0; i < this->size; ++i) {
    result.push_back(static_cast<char>(inner_data[i]));
  }
  return result;
}

#define CHECK_SIZE(size, expected_size, msg)                                                                           \
  if (size != expected_size) {                                                                                         \
    throw std::invalid_argument(msg);                                                                                  \
  }

/// Byte parser for an UTF-8 codified unicode symbol.
/// It is allocation free.
/// It is important that the str only has one UTF8 symbol. Will throw otherwise.
/// It is also important to note that is not possible build '\0' from this constructor.
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
