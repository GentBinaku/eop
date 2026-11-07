#include <cassert>
#include <cstdint>
#include <eop/ch-02/collision_point.hpp>
#include <eop/ch-02/signed_predicate.hpp>
#include <eop/ch-02/transformation.hpp>

int main() {

  // transformations
  {
    constexpr auto succ = [](std::int32_t x) -> std::int32_t { return x + 1; };
    constexpr auto add = [](std::int32_t x, std::int32_t y) { return x + y; };
    constexpr auto pos = [](std::int32_t x) { return x > 0; };
    static_assert(eop::transformation<decltype(succ), std::int32_t>);
    static_assert(!eop::transformation<decltype(add), std::int32_t>); // binary
    static_assert(
        !eop::transformation<decltype(pos), std::int32_t>); // returns bool
  }

  // distance_type
  {
    auto succ = [](std::int32_t x) { return x + 1; };
    auto mod10 = [](std::int32_t x) { return (x + 1) % 10; };
    auto collatz = [](std::int32_t x) {
      return x % 2 == 0 ? x / 2 : 3 * x + 1;
    };
    static_assert(std::same_as<eop::distance_type<decltype(succ), std::int32_t>,
                               std::uint32_t>);
  }
  // cycle
  {

    auto IV = std::int32_t{0};
    auto succ = [&](std::int32_t x) { return x + 1; };
    auto predicate = [&](std::int32_t x) { return x == IV; };
    assert(eop::collision_point(IV, succ, predicate) == IV + 1);
  }
}
