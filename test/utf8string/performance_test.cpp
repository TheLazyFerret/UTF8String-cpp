/// Main for doing debugging dumb stuff.

#include "utf8_symbol.hpp"
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <string_view>

#include <utf8_string.hpp>

using namespace utf8_string;

// clang-format off
constexpr std::string_view unicode_10 = "⣱ⱗ⚧▚⳥⊁⼸⼒⭷⧈";
constexpr std::string_view unicode_20 = "⌳ⱹ⚙♤⯶✜Ⳓ⣴⇮⏳↿⃄⺨⍚⎶↣≍❎ⵖₔ";
constexpr std::string_view unicode_50 = "⫙≨⎒ⳕ⫕Ⲑ⫋⻍↨⤥↧⠪⽝Ⳏ⊇⍉’⓽⡣⸑⽵✕⒜❶⹓ⷱ⃵⋈⡎✲⡝⟙⎰⠍◐⿮➛⸻∆⃩⨴⚴⩺␌➡⫉⣲ⵈⲚ⨌";
constexpr std::string_view unicode_100 ="⹳⤽⩳⦐⁮ⱏ⺁ⅰⶅ⊷ⶉ⹾◄⬼┄⠢⠊⦬ⶮ⏑≉⦡⫱▾✚⤧⧪ⶊ⮰⇾∝ⵅ⬙⁜⟲⯜⢺⽇␄ⳓ➬⾥⨨⒀⤑⍍⬹⚱⧅␜⁢Ⓝⴍⳮ⏛⬞⫮⬊⅏⨁ ⴶ⟺❇☞ⰵ⒬ⱽ⼆┠⦮⾜⋷♴⌻Ⳡ⾕⌼⹱⾈◂ⶫⓤⵒ⎿⼻⭗⩻ⅶ➫⧳⽈⻼⳽⇎ⅨℷⲂ┵⩈";
constexpr std::string_view unicode_500 = "⊂␾⪽⽓↲⬷②⓵⻠⌅⻶⽌ℾ₷⋪⎪⌔⹆▴⊏⟼ⴆ➞♆↫⨃⺾ⱇ↠⦘⎲➖Ⲷ⛌⭳ⷯ⨹Ⓝ⯪▁↤➉⾮►⡢Ⱂ⅟⨻▵⠀↢Ⰷ⍕╳⺒ⲇ⫂⭏⻍ⷤ⑍⮰ⱛ⚒┸⥌☸⢣⦙ₚ⛉⍺⴦⌠ⷽ≐❓⋀⺋⫥℅≖₄⎑⤁⟛⼪❏╞⁴⭙☱⻴↽♔⇗⦖⤚☊∳⌻⬧⭨⑘ ⺰⛰⇎⺗⪕⷗➿⚨ⓜ✷⍳⨇⹭ⱋ⑶⃜⊬⦛∏Ⓚ⭿⪴╄Ⅽ⧴⩷⻣ⵗ⑄➰ⓦ⬢➌⵰➝⤜␘❺ⱷ⣥∰⊖⹰⢮Ⅵⲁ₼⼋⎇ⲥⶸ⸝⾖⒳Ⓚ⹧⚓⽥⛄↱⁚⒚⡹Ⱂ℣℺⮙⃾⭏⧚⠲⋮♽⼸▆⭵⟈⧨⳧⚭⾾ⰽ⹚▓⡯☮⺝⧉⤨∡∘ⷉ⫹ⶮ⍍⋳⳱⌇≻⨨Ⱋ◖ⅇ⇚⍨⻧⺮⇹⧽⯬ⅎ⭷⯬◸⬕⏆―⚵Ⳇ⬇⸠◇⤁ⵁ ⫆⦲♹⛉⟭⮔Ⓚ⩧↙⤔♉♽⍜⫶⍥✫₇⫽✩⡤▰⭉⯠⎸➔⺔ⵢ␙⃀⤝⒏⭨ⰿ⣶⮌⣇₴⏅⢧⓪∱⿡⡣⃭⎫⚧Ⲹ⠃ℝ⸗└┾⇧ⵄ⹼⦢⵩ⶫⓠ▀ⱋ⽰⤙⤝ⷽ⅖┎⹥⚎⾓⟎⍯⣴⥞⟳℻⣡₤⡤⽂➲ⷃ⺰⏿⃷⾸⎊✃Ⱌ⋽⁊≧⚽☾⇝∄⎦⃎⓱₃⮢⶘⎘⣠℣⩝␡⦹➈⽾⏑⓪⧎⒠⡞ⴤⰹ╷⵭⯖ ⓓⓋ₡⊿ℴ⾻⟢⌢⨒ⴟ╙ⷰℝⅩ⮐⍰❌⟆ⱂ⃉⛱⽝⽨⽧☞⠦⣃⊊⒲⻱⬵⎟⇜⯄⏾▸⤍⺊⿑≞⎚Ⱃ⠁⇴ⴼ≀⫦╣ⷙⵖ⢸ℨ⇟⇑₍⫏ⷀ❰⭹⠧℥⁜⽈↲➎⤽⏋⨁⣌⚱⹅❮⨐⼦✬⦼⪧⤍⮏⿺⠖⵽∷⨍✸⿗⏌⥟⍊⫗❓⢯✁⛉⏞▭⠙⧶Ⱋ⻰―⣍⁉⥩❻⃬⁻Ⱨ⟼⒛⫙⑝⚬≅⤧⭝⸈≝∌↜Ⳙ⨸ⳟ➱♜≬₏⥞⏟⠺╗⅗❽⹃⢪⤁⺪⭤₏⪲ⶏ▘♍ⰾ┎⽙⦏⦽⇜";
//constexpr std::string_view unicode_500 = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibu";
// clang-format on

#define TEST(test_string, number_of_test)                                                                              \
  build_sum = 0;                                                                                                       \
  conversion_sum = 0;                                                                                                  \
  build_average = 0.0;                                                                                                 \
  conversion_average = 0.0;                                                                                            \
  for (size_t i = 0; i < number_of_test; ++i) {                                                                        \
    const auto start_build = chrono::steady_clock::now();                                                              \
    const UTF8String aux(test_string);                                                                                 \
    const auto end_build = chrono::steady_clock::now();                                                                \
    build_sum += chrono::duration_cast<chrono::nanoseconds>(end_build - start_build).count();                          \
    const auto start_conversion = chrono::steady_clock::now();                                                         \
    const auto string = aux.to_string();                                                                               \
    const auto end_conversion = chrono::steady_clock::now();                                                           \
    conversion_sum += chrono::duration_cast<std::chrono::nanoseconds>(end_conversion - start_conversion).count();      \
    benchmark_sink += string.size();                                                                                   \
  }                                                                                                                    \
  build_average = static_cast<double>(build_sum) / static_cast<double>(number_of_test);                                \
  conversion_average = static_cast<double>(conversion_sum) / static_cast<double>(number_of_test);                      \
  std::cout << " build average: " << build_average << "ns\n"                                                           \
            << " conversion to std::string average: " << conversion_average << "ns" << std::endl;

void performance_test() {
  constexpr size_t NUMBER_OF_TEST = 10000;
  namespace chrono = std::chrono;
  std::uint64_t benchmark_sink = 0;
  // WARMUP
  for (size_t i = 0; i < NUMBER_OF_TEST; ++i) {
    const UTF8String a(unicode_10);
    const auto aux_a = a.to_string();
    const UTF8String b(unicode_20);
    const auto aux_b = b.to_string();
    const UTF8String c(unicode_50);
    const auto aux_c = c.to_string();
    const UTF8String d(unicode_100);
    const auto aux_d = d.to_string();
    const UTF8String e(unicode_500);
    const auto aux_e = e.to_string();
    benchmark_sink += aux_a.size() + aux_b.size() + aux_c.size() + aux_d.size() + aux_e.size();
  }
  std::uint64_t build_sum;
  std::uint64_t conversion_sum;
  double build_average;
  double conversion_average;
  std::cout << "Test 10 symbol unicode" << std::endl;
  TEST(unicode_10, NUMBER_OF_TEST)
  std::cout << "Test 20 symbol unicode" << std::endl;
  TEST(unicode_20, NUMBER_OF_TEST)
  std::cout << "Test 50 symbol unicode" << std::endl;
  TEST(unicode_50, NUMBER_OF_TEST)
  std::cout << "Test 100 symbol unicode" << std::endl;
  TEST(unicode_100, NUMBER_OF_TEST)
  std::cout << "Test 500 symbol unicode" << std::endl;
  TEST(unicode_500, NUMBER_OF_TEST)
  std::cout << "benchmark_sink: " << benchmark_sink << std::endl;
}

#undef TEST

consteval size_t calculate_utf8_size(std::string_view msg) {
  return UTF8String(msg).utf8_byte_size();
}

int main() {
  std::cout << "unicode string size: " << calculate_utf8_size(unicode_500) << std::endl;
  performance_test();
  return EXIT_SUCCESS;
  
}
