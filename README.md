# UTF8String-cpp

A simple, small (under 1000 lines) and easy to use UTF8 encoded string library written with modern c++ and cmake practices.

## Quick start

```cpp
//! Small program that calculates the number of UTF8 symbols in a string at compile time.

#include <iostream>
#include <string_view>
#include <cstdlib>

#include "utf8_string.hpp"

using namespace utf8_string;

constexpr std::string_view STR = "⎼❏⮢◅⬰⑄⃬ⱋ⩸⁐⋅≡➚⋣⠕⯂┽⡶⸞⠣ⲡ‾⯨⠡⻚♋⟼⓪❣ⲱ♦⟭Ɀ⽊";

consteval size_t calculate(std::string_view str) {
  const UTF8String aux(str);
  return aux.size();
}

int main() {
  std::cout << calculate(STR) << std::endl;
  return EXIT_SUCCESS;
}

```

## Installation

Check [install.md](./doc/install.md) for further explanation.

## Usage

Check [usage.md](./doc/usage.md) for a basic usage tour and implementation details.

## License

This project is under the [MIT](./LICENSE) License.
