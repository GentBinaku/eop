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

    // Terminating orbit: f defined only for x < 10, so the walk stops at 10.
    auto succ_term = [](std::int32_t x) { return x + 1; };
    auto below10 = [](std::int32_t x) { return x < 10; };
    assert(eop::terminating(IV, succ_term, below10) == true);

    // f is total on the orbits below, so the definition-space predicate is
    // always true; only f decides whether the orbit cycles.
    auto always = [](std::int32_t) { return true; };

    // Circular orbit: 0 -> 1 -> ... -> 9 -> 0  (handle h = 0, cycle c = 10).
    // With h = 0 the collision point is f^(c-1)(x), the element before x.
    auto succ_cycle = [](std::int32_t x) { return (x + 1) % 10; };
    assert(eop::collision_point(IV, succ_cycle, always) == 9);

    // Rho-shaped orbit: 0 -> 1 -> 2 -> [3 -> 4 -> 5 -> 6 -> 3]
    // (handle h = 3, cycle c = 4). slow = f^n(x), fast = f^(2n+1)(x);
    // they meet at n = 3, the cycle's entry point.
    auto rho = [](std::int32_t x) { return x < 6 ? x + 1 : 3; };
    assert(eop::collision_point(IV, rho, always) == 3);
  }
}
