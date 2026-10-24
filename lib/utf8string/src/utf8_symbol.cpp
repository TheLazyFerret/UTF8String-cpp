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
    const auto aux_size = bytes_of_utf8(aux);
    if (aux_size <= 0) {
      is.setstate(std::ios::failbit);
      return is;
    }
    byte_size = static_cast<size_t>(aux_size);
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
