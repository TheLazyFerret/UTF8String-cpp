# Usage

First of all, check the [install.md](./install.md) for getting the library.

## Prepare the cmake

For example, if your project is just an executable, you can simply do in the `CMakeLists.txt`:

```cmake
add_executable(my_executable main.cpp)

target_link_libraries(my_executable PRIVATE UTF8String::UTF8String)
```

## Library usage.

All the library is inside the namespace `utf8_string`. There should not be collisions with std, so you can:

```cpp
#include "utf8_string/utf8_string.hpp"

using namespace utf8_string;
```

This library is made of two classes:

### UT8Symbol

This class represents a single UTF8 encoded unicode symbol. It has a fixed size of 5 bytes. It is guaranteed to always contain a valid codified symbol. If using std::istream, check the flags before using the instance.

If constructed by default, the symbol will be set to the ASCII `\0`.

In general this class is pretty simple to use, you will just keep in mind the next cases:

- If trying to construct from a wrongly encoded symbol, it will throw an exception. You can check if a symbol is valid before constructing using the auxiliar function `is_symbol_utf8_valid()`.
- For multi-byte UTF8 symbols, you will need to use the constructor `UTF8Symbol::UTF8Symbol(std::string_view)`. In this case, ensure there is only one unicode symbol and it is valid. Multiple symbol in the str will make it throw an exception.

The internal bytes are not directly writtable, so you will need to construct a new instance to modify. If you need to access them, you have different methods:

- `UTF8Symbol::to_char()` Return a equivalent c-char symbol if possible. Will throw in the case the symbol is multi-byte.
- `UTF8Symbol::to_integer() Return a std::uint32_t with the symbol converted to an integer (using a BE logic).`
- `UTF8Symbol::to_string()` Return a potentially multi-byte std::string with the single symbol.
- `UTF8Symbol::data()` Return an `std::pair` with a `std::array<char8_t, 4>` and a `std::size_t` with the size of the data. The bytes outside of the size are garbage.
- `UTF8Symbol::data_vec()` Return an `std::vector<char8_t>` with the exact number of bytes.
