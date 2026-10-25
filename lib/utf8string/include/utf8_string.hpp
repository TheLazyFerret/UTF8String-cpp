//! My personal implementation for working with UTF-8 strings in c++.
//! I decided to use with a vector because this way I don't need to worry about
//! manually manage the memory.
//!
//! Would love to use std::optional for accessor methods, sadly they can't yet use T as a reference.
//! Could use Boost implementation, but that would create a pretty unnecesary dependency.
//!
//! I decided the Undefined methods in the std container instead throw in error.

#pragma once

#include "utf8_symbol.hpp"

#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace utf8_string {

class UTF8String {
public:
  // Special methods.
  constexpr UTF8String() noexcept = default;
  constexpr explicit UTF8String(std::string_view);
  constexpr ~UTF8String() = default;
  constexpr UTF8String(const UTF8String&) = default;
  constexpr UTF8String& operator=(const UTF8String&) = default;
  constexpr UTF8String(UTF8String&&) = default;
  constexpr UTF8String& operator=(UTF8String&&) = default;

  // Accessors methods.
  constexpr UTF8Symbol& at(size_t pos) { return inner_data.at(pos); }
  constexpr const UTF8Symbol& at(size_t pos) const { return inner_data.at(pos); }
  constexpr UTF8Symbol& operator[](size_t pos) { return inner_data.at(pos); }
  constexpr const UTF8Symbol& operator[](size_t pos) const { return inner_data.at(pos); }
  constexpr UTF8Symbol& front() { return inner_data.at(0); }
  constexpr const UTF8Symbol& front() const { return inner_data.at(0); }
  constexpr UTF8Symbol& back() { return inner_data.at(inner_data.size() - 1); }
  constexpr const UTF8Symbol& back() const { return inner_data.at(inner_data.size() - 1); }

  // Capacity methods.
  constexpr size_t size() const noexcept { return inner_data.size(); }
  constexpr size_t length() const noexcept { return inner_data.size(); }
  constexpr bool empty() const noexcept { return inner_data.empty(); }
  constexpr size_t max_size() const noexcept { return inner_data.max_size(); }
  constexpr void reserve(size_t n) { inner_data.reserve(n); }
  constexpr size_t capacity() const noexcept { return inner_data.capacity(); }
  constexpr void shrink_to_fit() { inner_data.shrink_to_fit(); }

  // Modifiers methods.
  constexpr void clear() noexcept { inner_data.clear(); }
  constexpr void push_back(UTF8Symbol symbol) { inner_data.push_back(symbol); }
  constexpr void pop_back();
  constexpr void resize(size_t count) { inner_data.resize(count); }
  constexpr void resize(size_t count, UTF8Symbol symbol) { inner_data.resize(count, symbol); }
  constexpr UTF8String& append(const UTF8String&);

  constexpr friend UTF8String operator+(const UTF8String&, const UTF8String&);
  constexpr friend UTF8String operator+(const UTF8String&, UTF8Symbol);

  std::string to_string() const noexcept;

private:
  std::vector<UTF8Symbol> inner_data;
};

/// Construct a new UTF8String from a str.
/// It only does as much one allocation for holding the resulting value.
constexpr UTF8String::UTF8String(std::string_view str) {
  // Reserve the worst case memory needed to hold the converted string.
  inner_data.reserve(str.size());
  for (size_t i = 0; i < str.size(); ++i) {
    const auto size = bytes_of_utf8(str[i]);
    // If fails, it is because the parameter is bad encoded.
    if (size <= 0) {
      throw std::invalid_argument("Bad encoded symbol.");
    }
    const std::string_view slice = str.substr(i, static_cast<size_t>(size));
    inner_data.emplace_back(slice);
    i += static_cast<size_t>(size) - 1; // Need to substract 1 because skips 1 byte otherwise.
  }
  inner_data.shrink_to_fit();
}

/// Removes the last element in the UTF8String. Throws if it is empty.
constexpr void UTF8String::pop_back() {
  if (inner_data.empty()) {
    throw std::runtime_error("The container is empty.");
  }
  inner_data.pop_back();
}

/// Append the src UTF8String at the end of the object.
constexpr UTF8String& UTF8String::append(const UTF8String& src) {
  inner_data.insert(inner_data.end(), src.inner_data.begin(), src.inner_data.end());
  return *this;
}

/// Return the concatenation of two UTF8String as a new UTF8String.
constexpr UTF8String operator+(const UTF8String& lhs, const UTF8String& rhs) {
  UTF8String result;
  result.inner_data.reserve(lhs.size() + rhs.size());
  result.inner_data.insert(result.inner_data.end(), lhs.inner_data.begin(), lhs.inner_data.end());
  result.inner_data.insert(result.inner_data.end(), rhs.inner_data.begin(), rhs.inner_data.end());
  return result;
}

/// Return the concatenation of a UTF8String and a UTF8Symbol as a new UTF8String.
constexpr UTF8String operator+(const UTF8String& lhs, UTF8Symbol rhs) {
  UTF8String result = lhs;
  result.inner_data.push_back(rhs);
  return result;
}

} // namespace utf8_string
