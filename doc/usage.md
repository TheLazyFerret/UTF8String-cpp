# Usage

First of all, check the [install.md](./install.md) for getting the library.

## Prepare the cmake

For example, if your project is just an executable, you can simply do in the `CMakeLists.txt` of your project:

```cmake
add_executable(my_executable main.cpp)

target_link_libraries(my_executable PRIVATE UTF8String::UTF8String)
```

## Library usage

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

- If trying to construct from a wrongly encoded symbol, it will throw an exception. You can check if a symbol is valid before constructing using the auxiliar function `is_symbol_utf8_valid(char)` or `is_symbol_utf8_valid(std::string_view)`.
- For multi-byte UTF8 symbols, you will need to use the constructor `UTF8Symbol::UTF8Symbol(std::string_view)`. In this case, ensure there is only one unicode symbol and it is valid. Multiple symbol in the str will make it throw an exception.

The internal bytes are not directly writtable, so you will need to construct a new instance to modify. If you need to access them, you have different methods:

- `UTF8Symbol::to_char()` Return a equivalent c-char symbol if possible. Will throw in the case the symbol is multi-byte.
- `UTF8Symbol::to_integer()` Return a std::uint32_t with the symbol converted to an integer (using a BE logic).`
- `UTF8Symbol::to_string()` Return a potentially multi-byte std::string with the single symbol.
- `UTF8Symbol::data()` Return an `std::pair` with a `std::array<char8_t, 4>` and a `std::size_t` with the size of the data. The bytes outside of the size are garbage.
- `UTF8Symbol::data_vec()` Return an `std::vector<char8_t>` with the exact number of bytes.

Also it has implemented `std::hash` and `std::formatter`.

Besides that, check [utf8_symbol.hpp](../include/utf8_symbol.hpp) to see the overriden methods. This class is also completely constexpr friendly.

### UTF8String

The most important part of the library, this class represent a correctly encoded UTF-8 unicode array of symbols. Internally utilizes a `std::vector<UTF8Symbol>`, so I avoid reinvent the wheel.

Similar to UTF8Symbol, this class can be constructed from a correctly encoded `std::string_view` (throwing an exception if not). You can use the auxiliar method `is_string_utf8_valid(std::string_view)` to ensure a string is valid before constructing it.

Besides that, it has more or less the same interface as `std::string`. Some differences are:

- `UTF8String::utf8_byte_size()` return the length in bytes of the string, instead of the number of symbols of `UTF8String::length()` and `UTF8String::size()`.
- Some typical undefined behaviour methods in std::string (like `operator[]`) will throw instead in case of error.
- `UTF8String::to_string()` will return a `std::string` representing the same array of symbols as the caller. _It is not marked constexpr for optimization purposes (I hope I can change this in the future)._

The most expensive operation is the decodification of the original string. So if you need to create, modify and delete many of them, probably the best option would be create it only when it is needed, and do the rest of operations with a normal `std::string`.

Some implemented algorithms (like `UTF8String::find(const UTF8String&)`) are not the most optimal options. Although I am trying to find better solutions :>

This class also has overriden `std::hash` _(using the FNV-1a algorithm)_ and for `std::format` (relying in the formatter of `std::string`).

## Example

See [example.md](./example.md) for a examples.
