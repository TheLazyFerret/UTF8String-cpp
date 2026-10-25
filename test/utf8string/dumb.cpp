/// Main for doing debugging dumb stuff.

#include <cassert>
#include <cstdlib>
#include <string_view>

#include <utf8_string.hpp>

using namespace utf8_string;

constexpr std::string_view random_text = "Ё Ђ Ѓ Є Ѕ І Ї Ј Љ Њ Ћ Ќ Ў Џ А Б В Г Д Е Ж З И Й К Л М Н О П Р С Т";

consteval size_t utf8_symbols_in_text() {
  const UTF8String a(random_text) ;
  return a.size();
}

int main() {
  std::cout << utf8_symbols_in_text() << std::endl;
  return EXIT_SUCCESS;
}
