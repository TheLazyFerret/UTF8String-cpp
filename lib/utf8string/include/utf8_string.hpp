//! My personal implementation for working with UTF-8 strings in c++.
//! I decided to use with a vector because this way I don't need to worry about
//! manually manage the memory.

#pragma once

#include "utf8_symbol.hpp"

#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace utf8_string {

class UTF8String {
public:
  constexpr UTF8String() noexcept = default;
  explicit UTF8String(std::string_view);
  ~UTF8String() = default;
  constexpr UTF8String(const UTF8String&) = default;
  constexpr UTF8String& operator=(const UTF8String&) = default;
  constexpr UTF8String(UTF8String&&) = default;
  constexpr UTF8String& operator=(UTF8String&&) = default;

  std::string to_string() const noexcept;

  constexpr void push_back(const UTF8Symbol& symbol) noexcept { inner_data.push_back(symbol); }
  constexpr void push_back(UTF8Symbol&& symbol) noexcept { inner_data.push_back(std::move(symbol)); }

private:
  std::vector<UTF8Symbol> inner_data;
};

} // namespace utf8_string
