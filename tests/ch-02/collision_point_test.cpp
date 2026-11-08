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

// Rho-shaped, handle longer than the cycle:
// 0 -> 1 -> 2 -> 3 -> 4 -> [5 -> 6 -> 7 -> 8 -> 5]  (h = 5, c = 4).
// h = 1*4 + 1, so r = 1 and d = c - r - 1 = 2: collision point is 5 + 2 = 7.
constexpr auto long_rho = [](std::int32_t x) { return x < 8 ? x + 1 : 5; };

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

TEST(CollisionPoint, HandleLongerThanCycle) {
  EXPECT_EQ(eop::collision_point(x0, long_rho, always), 7);
}

TEST(CollisionPointNonterminatingOrbit, MatchesCollisionPoint) {
  // With an always-true predicate the general version never stops early,
  // so both must find the same point.
  EXPECT_EQ(eop::collision_point_nonterminating_orbit(x0, mod10), 9);
  EXPECT_EQ(eop::collision_point_nonterminating_orbit(x0, rho), 3);
  EXPECT_EQ(eop::collision_point_nonterminating_orbit(x0, long_rho), 7);
}

TEST(CircularNonterminatingOrbit, TrueWhenNoHandle) {
  // h = 0: f(collision point) = f(9) = 0 = x.
  EXPECT_TRUE(eop::circular_nonterminating_orbit(x0, mod10));
}

TEST(CircularNonterminatingOrbit, FalseWhenHandle) {
  // h > 0: f(collision point) is in the cycle, x is not.
  EXPECT_FALSE(eop::circular_nonterminating_orbit(x0, rho));
  EXPECT_FALSE(eop::circular_nonterminating_orbit(x0, long_rho));
}

TEST(CircularNonterminatingOrbit, TrueFromAnyPointOnTheCycle) {
  // Starting inside the cycle of a rho orbit makes it circular.
  EXPECT_TRUE(eop::circular_nonterminating_orbit(std::int32_t{5}, rho));
}

TEST(Circular, TrueForCircularOrbit) {
  EXPECT_TRUE(eop::circular(x0, mod10, always));
}

TEST(Circular, FalseForRhoShapedOrbit) {
  EXPECT_FALSE(eop::circular(x0, rho, always));
  EXPECT_FALSE(eop::circular(x0, long_rho, always));
}

TEST(Circular, FalseForTerminatingOrbit) {
  // p(y) is false at the terminal element, so f(y) is never called.
  EXPECT_FALSE(eop::circular(x0, succ, below10));
}

TEST(Circular, AgreesWithNonterminatingVersion) {
  for (std::int32_t x = 0; x <= 8; ++x) {
    EXPECT_EQ(eop::circular(x, long_rho, always),
              eop::circular_nonterminating_orbit(x, long_rho))
        << "x = " << x;
  }
}
