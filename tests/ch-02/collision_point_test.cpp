#include <cstdint>
#include <eop/ch-02/collision_point.hpp>
#include <gtest/gtest.h>

namespace {

constexpr std::int32_t x0 = 0;

// f is total on these orbits, so the definition-space predicate is always
// true; only f decides whether the orbit cycles.
constexpr auto always = [](std::int32_t) { return true; };

// Terminating: f defined only for x < 10, so the walk stops at 10.
constexpr auto succ = [](std::int32_t x) { return x + 1; };
constexpr auto below10 = [](std::int32_t x) { return x < 10; };

// Circular: 0 -> 1 -> ... -> 9 -> 0  (h = 0, c = 10).
constexpr auto mod10 = [](std::int32_t x) { return (x + 1) % 10; };

// Rho-shaped: 0 -> 1 -> 2 -> [3 -> 4 -> 5 -> 6 -> 3]  (h = 3, c = 4).
constexpr auto rho = [](std::int32_t x) { return x < 6 ? x + 1 : 3; };

} // namespace

TEST(CollisionPoint, StopsWhereFIsUndefined) {
  // f defined only at x0, so the walk stops at f(x0).
  auto only_x0 = [](std::int32_t x) { return x == x0; };
  EXPECT_EQ(eop::collision_point(x0, succ, only_x0), x0 + 1);
}

TEST(CollisionPoint, TerminatingOrbitReturnsTerminalElement) {
  EXPECT_EQ(eop::collision_point(x0, succ, below10), 10);
}

TEST(CollisionPoint, CircularOrbitMeetsAtElementBeforeStart) {
  // With h = 0 the collision point is f^(c-1)(x).
  EXPECT_EQ(eop::collision_point(x0, mod10, always), 9);
}

TEST(CollisionPoint, RhoShapedOrbit) {
  // slow = f^n(x), fast = f^(2n+1)(x); they meet at n = 3.
  EXPECT_EQ(eop::collision_point(x0, rho, always), 3);
}

TEST(Terminating, TrueWhenOrbitReachesUndefinedPoint) {
  EXPECT_TRUE(eop::terminating(x0, succ, below10));
}

TEST(Terminating, FalseForCircularOrbit) {
  EXPECT_FALSE(eop::terminating(x0, mod10, always));
}

TEST(Terminating, FalseForRhoShapedOrbit) {
  EXPECT_FALSE(eop::terminating(x0, rho, always));
}
