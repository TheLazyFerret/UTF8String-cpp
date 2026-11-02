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

#include <compare>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace utf8_string {

constexpr bool is_string_utf8_valid(std::string_view) noexcept;

class UTF8String {
public:
  struct Iterator {
    // Tags.
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = UTF8Symbol;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t;

    // Special methods.
    constexpr Iterator() noexcept = default;
    constexpr Iterator(UTF8String* data, size_t index = 0) noexcept : data(data), index(index) {}

    // Methods.
    constexpr Iterator& operator++() noexcept;
    constexpr Iterator operator++(int) noexcept;
    constexpr Iterator& operator--() noexcept;
    constexpr Iterator operator--(int) noexcept;

    constexpr Iterator operator+(difference_type n) const noexcept;
    constexpr friend Iterator operator+(difference_type n, const Iterator&) noexcept;
    constexpr Iterator& operator+=(difference_type n) noexcept;
    constexpr Iterator operator-(difference_type n) const noexcept;
    constexpr Iterator& operator-=(difference_type n) noexcept;

    constexpr difference_type operator-(const Iterator& other) const;

    constexpr reference operator[](difference_type n) const noexcept;

    constexpr bool operator==(const Iterator&) const noexcept;
    constexpr std::strong_ordering operator<=>(const Iterator& other) const;

    constexpr reference operator*() const;
    constexpr UTF8Symbol* operator->() const noexcept;

  private:
    UTF8String* data;
    size_t index;
  };
  static_assert(std::contiguous_iterator<Iterator>);

  // Special methods.
  constexpr UTF8String() noexcept = default;
  constexpr explicit UTF8String(std::string_view);
  constexpr explicit UTF8String(UTF8Symbol);
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
  constexpr size_t utf8_byte_size() const noexcept;
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
  constexpr UTF8String& append(UTF8Symbol);

  // Search methods.
  constexpr size_t find(const UTF8String& src, size_t pos = 0) const noexcept;
  constexpr size_t find(UTF8Symbol symbol, size_t pos = 0) const noexcept;

  // Iterator methods.

  // Other operations methods.
  constexpr UTF8String substr(size_t pos = 0, size_t count = npos) const;
  std::string to_string() const;

  constexpr friend UTF8String operator+(const UTF8String&, const UTF8String&);
  constexpr friend UTF8String operator+(const UTF8String&, UTF8Symbol);
  constexpr friend UTF8String& operator+=(UTF8String&, const UTF8String&);
  constexpr friend UTF8String& operator+=(UTF8String&, UTF8Symbol);

  friend std::ostream& operator<<(std::ostream& os, const UTF8String&);
  friend std::istream& operator>>(std::istream& is, UTF8String&);

  constexpr friend bool operator==(const UTF8String&, const UTF8String&) noexcept;
  constexpr friend bool operator!=(const UTF8String&, const UTF8String&) noexcept;

  static constexpr size_t npos = -1; // No position special value.

private:
  constexpr static std::vector<UTF8Symbol> decode_str(std::string_view);

  std::vector<UTF8Symbol> inner_data;
};

/// Decode a str into a vector of UTF8Symbol.
constexpr std::vector<UTF8Symbol> UTF8String::decode_str(std::string_view str) {
  std::vector<UTF8Symbol> result;
  // Reserve a conservative ammount of space.
  result.reserve(str.size() / 2);
  for (size_t i = 0; i < str.size(); ++i) {
    const auto size = bytes_of_utf8(str[i]);
    // If fails, it is because the parameter is bad encoded.
    if (size <= 0) {
      throw std::invalid_argument("Bad encoded symbol.");
    }
    const std::string_view slice = str.substr(i, static_cast<size_t>(size));
    result.emplace_back(slice);
    i += static_cast<size_t>(size) - 1; // Need to substract 1 because skips 1 byte otherwise.
  }
  return result;
}

/// Construct a new UTF8String from a str.
/// In the best case, it uses only one allocation.
constexpr UTF8String::UTF8String(std::string_view str) { inner_data = decode_str(str); }

/// Creates a UTF8String with only one symbol.
constexpr UTF8String::UTF8String(UTF8Symbol symbol) { inner_data.push_back(symbol); }

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

/// Append the symbol at the end of the object.
constexpr UTF8String& UTF8String::append(UTF8Symbol symbol) {
  inner_data.push_back(symbol);
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

/// Return the exact number of bytes required for saving this utf8
/// in a contiguous byte container (like a string, or a vector of char).
/// Loops for each symbol to gets its size.
constexpr size_t UTF8String::utf8_byte_size() const noexcept {
  size_t result = 0;
  for (const auto& symbol : inner_data) {
    result += symbol.byte_size();
  }
  return result;
}

/// Returns true if the str is a valid UTF8 encoded string.
constexpr bool is_string_utf8_valid(std::string_view str) noexcept {
  for (size_t i = 0; i < str.size(); ++i) {
    const auto size = bytes_of_utf8(str[i]);
    if (size < 1) {
      return false;
    }
    const std::string_view slice = str.substr(i, static_cast<size_t>(size));
    if (!is_symbol_utf8_valid(slice)) {
      return false;
    }
    i += static_cast<size_t>(size) - 1;
  }
  return true;
}

/// Return true if both UTF8String are equal.
constexpr bool operator==(const UTF8String& lhs, const UTF8String& rhs) noexcept {
  if (lhs.size() != rhs.size()) {
    return false;
  }
  for (size_t i = 0; i < lhs.size(); ++i) {
    if (lhs.inner_data[i] != rhs.inner_data[i]) {
      return false;
    }
  }
  return true;
}

/// Return true if both UTF8String are different.
constexpr bool operator!=(const UTF8String& lhs, const UTF8String& rhs) noexcept {
  if (lhs.size() != rhs.size()) {
    return true;
  }
  for (size_t i = 0; i < lhs.size(); ++i) {
    if (lhs.inner_data[i] == rhs.inner_data[i]) {
      return false;
    }
  }
  return true;
}

/// Do the += assignment between two UTF8String.
constexpr UTF8String& operator+=(UTF8String& lhs, const UTF8String& rhs) {
  lhs.append(rhs);
  return lhs;
}

/// Do the += assignment between a UTF8String and a UTF8Symbol.
constexpr UTF8String& operator+=(UTF8String& lhs, UTF8Symbol rhs) {
  lhs.inner_data.push_back(rhs);
  return lhs;
}

/// Find the substring str in the caller instance.
constexpr size_t UTF8String::find(const UTF8String& str, size_t pos) const noexcept {
  if (str.size() + pos > this->size()) {
    return npos;
  } else if (str.empty() && empty()) {
    return 0;
  }
  for (size_t i = pos; i < this->size(); ++i) {
    if (str.size() + i > this->size()) {
      return npos;
    }
    for (size_t j = 0; j < str.size(); ++j) {
      if (at(i + j) != str[j]) {
        break;
      }
      if (j == str.size() - 1) {
        return i;
      }
    }
  }
  return npos;
}

/// Find the symbol in the UTF8String.
constexpr size_t UTF8String::find(UTF8Symbol symbol, size_t pos) const noexcept {
  for (size_t i = pos; i < size(); ++i) {
    if (at(i) == symbol) {
      return i;
    }
  }
  return npos;
}

/// Return a slice of the source string.
/// Follows the same rules as std::string::substr().
constexpr UTF8String UTF8String::substr(size_t pos, size_t count) const {
  if (pos > size()) {
    throw(std::out_of_range("pos > size()"));
  }
  UTF8String result;
  result.inner_data.reserve(this->inner_data.size());
  size_t inserted = 0;
  for (size_t i = pos; i < size(); ++i) {
    if (inserted >= count) {
      return result;
    }
    result.push_back(this->at(i));
    inserted += 1;
  }
  return result;
}

#define Iterator UTF8String::Iterator

/// Pre-increment operator.
constexpr Iterator& Iterator::operator++() noexcept {
  this->index += 1;
  return *this;
}

/// Post-increment operator.
constexpr Iterator Iterator::operator++(int) noexcept {
  const auto tmp = *this;
  this->index += 1;
  return tmp;
}

/// Pre-decrement operator.
constexpr Iterator& Iterator::operator--() noexcept {
  this->index -= 1;
  return *this;
}

/// Post-decrement operator.
constexpr Iterator Iterator::operator--(int) noexcept {
  const auto tmp = *this;
  this->index -= 1;
  return tmp;
}

/// Returns true if two iterators are equal.
constexpr bool Iterator::operator==(const Iterator& other) const noexcept {
  return (this->data == other.data) && (this->index == other.index);
}

/// Returns a reference to the value in the position (throws if invalid).
constexpr UTF8Symbol& Iterator::operator*() const { return this->data->at(this->index); }

/// Advance n position in the iterator.
constexpr Iterator Iterator::operator+(difference_type n) const noexcept {
  Iterator copy = *this;
  copy.index += static_cast<size_t>(n);
  return copy;
}

/// Same adding, but in the inverted order.
constexpr Iterator operator+(std::ptrdiff_t n, const Iterator& iter) noexcept {
  auto copy = iter;
  copy.index += static_cast<size_t>(n);
  return copy;
}

/// Advance and modify n positions in the iterator.
constexpr Iterator& Iterator::operator+=(difference_type n) noexcept {
  this->index += static_cast<size_t>(n);
  return *this;
}

/// Goes back n position in the iterator.
constexpr Iterator Iterator::operator-(difference_type n) const noexcept {
  Iterator copy = *this;
  copy.index -= static_cast<size_t>(n);
  return copy;
}

/// Goes back n position and modify the iterator.
constexpr Iterator& Iterator::operator-=(difference_type n) noexcept {
  this->index -= static_cast<size_t>(n);
  return *this;
}

/// Return the value in the n position.
constexpr UTF8Symbol& Iterator::operator[](difference_type n) const noexcept { return *(*this + n); }

/// Spaceship comparison of the iterator.
constexpr std::strong_ordering Iterator::operator<=>(const Iterator& other) const {
  if (this->data != other.data) {
    throw(std::runtime_error("Invalid comparison"));
  }
  if (this->index < other.index) {
    return std::strong_ordering::less;
  } else if (this->index > other.index) {
    return std::strong_ordering::greater;
  }
  return std::strong_ordering::equal;
}

/// Return the difference between two iterators.
constexpr std::ptrdiff_t Iterator::operator-(const Iterator& other) const {
  if (this->data != other.data) {
    throw(std::runtime_error("Different iterators pointers"));
  }
  return static_cast<std::ptrdiff_t>(other.index - this->index);
}

/// Easy access to a pointer to the iterated value.
constexpr UTF8Symbol* Iterator::operator->() const noexcept { return &data->at(index); }

#undef Iterator

} // namespace utf8_string
