#include <cstdint>
#include <iostream>
#include <limits>

#include <eop/ch-02/signed_predicate.hpp>
#include <eop/ch-02/transformation.hpp>

using eop::can_add_without_overflow;

// constexpr lets the compiler prove these at build time
using i32 = std::numeric_limits<std::int32_t>;
static_assert(can_add_without_overflow<std::int32_t>(1, 2));
static_assert(!can_add_without_overflow(i32::max(), 1));
static_assert(!can_add_without_overflow(i32::min(), -1));
static_assert(can_add_without_overflow(i32::max(), i32::min()));
static_assert(!can_add_without_overflow<std::int8_t>(100, 100));

// transformation concept
constexpr auto succ = [](std::int32_t x) { return x + 1; };
constexpr auto add = [](std::int32_t x, std::int32_t y) { return x + y; };
constexpr auto pos = [](std::int32_t x) { return x > 0; };
static_assert(eop::transformation<decltype(succ), std::int32_t>);
static_assert(!eop::transformation<decltype(add), std::int32_t>); // binary
static_assert(
    !eop::transformation<decltype(pos), std::int32_t>); // returns bool
static_assert(std::same_as<eop::distance_type<decltype(succ), std::int32_t>,
                           std::uint32_t>);

// power_unary: domain and count types are independent
static_assert(eop::power_unary(std::int32_t{5}, 3, succ) == 8);
static_assert(eop::power_unary(std::int32_t{5}, 3u, succ) == 8);
static_assert(eop::power_unary(std::int64_t{5}, std::int8_t{3},
                               [](std::int64_t x) { return x * 2; }) == 40);

int main() {
  std::cout << std::boolalpha;
  std::cout << "1 + 2:     " << can_add_without_overflow(1, 2) << '\n';
  std::cout << "max + 1:   " << can_add_without_overflow(i32::max(), 1) << '\n';
  std::cout << "min + -1:  " << can_add_without_overflow(i32::min(), -1)
            << '\n';
  std::cout << "max + min: " << can_add_without_overflow(i32::max(), i32::min())
            << '\n';
  std::cout << "succ^3(5): " << eop::power_unary(std::int32_t{5}, 3, succ)
            << '\n';
}
