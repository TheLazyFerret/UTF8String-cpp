//! My personal implementation for working with UTF-8 strings in c++.
//! I decided to use with a vector because this way I don't need to worry about
//! manually manage the memory.

#pragma once

#include "utf8_symbol.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace utf8_string {

class UTF8String {
public:
  constexpr UTF8String() noexcept = default;                    // Default constructor.
  explicit UTF8String(std::string_view);                        // Constructor from string_view.
  ~UTF8String() = default;                                      // Destructor.
  constexpr UTF8String(const UTF8String&) = default;            // Copy constructor.
  constexpr UTF8String& operator=(const UTF8String&) = default; // Copy assignment.
  constexpr UTF8String(UTF8String&&) = default;                 // Move constructor.
  constexpr UTF8String& operator=(UTF8String&&) = default;      // Move assignment.

  std::string to_string() const noexcept;

private:
  std::vector<UTF8Symbol> inner_data;
};

} // namespace utf8_string
