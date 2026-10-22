//! Private implementations not shared to the public interface.

#pragma once

#include <array>
#include <string_view>

namespace utf8_string::impl {

/// Example unicode symbol.
[[maybe_unused]] constexpr std::string_view EXAMPLE_UNICODE = "ඞ";

constexpr std::array<char8_t, 4> F_BYTE_MASK = {0b10000000, 0b11100000, 0b11110000, 0b11111000};
constexpr std::array<char8_t, 4> F_BYTE_RESULT = {0b00000000, 0b11000000, 0b11100000, 0b11110000};
constexpr char8_t NF_BYTE_MASK = 0b11000000;
constexpr char8_t NF_BYTE_RESULT = 0b10000000;

/// Test a byte, ANDing the mask and comparing with the expected result.
constexpr bool test_8byte(char8_t byte, char8_t mask, char8_t result) noexcept { return (byte & mask) == result; }

} // namespace utf8_string::impl
