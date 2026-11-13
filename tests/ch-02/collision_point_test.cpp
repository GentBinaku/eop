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

// Exercise 2.2

TEST(OrbitIntersection, SameStartingPoint) {
  EXPECT_TRUE(eop::orbit_intersection(6, 6, long_rho, always));
}

TEST(OrbitIntersection, SameRhoOrbitDifferentCollisionPoints) {
  // collision points are 7 and 5, but the cycle {5,6,7,8} is shared.
  EXPECT_TRUE(eop::orbit_intersection(0, 6, long_rho, always));
  EXPECT_TRUE(eop::orbit_intersection(0, 3, long_rho, always));
}

TEST(OrbitIntersection, SeparateCycles) {
  auto plus2 = [](std::int32_t x) { return (x + 2) % 10; };
  EXPECT_FALSE(eop::orbit_intersection(0, 1, plus2, always)); // evens vs odds
  EXPECT_TRUE(eop::orbit_intersection(0, 4, plus2, always));
}

TEST(OrbitIntersection, TerminatingSameEnd) {
  EXPECT_TRUE(eop::orbit_intersection(0, 5, succ, below10)); // both end at 10
}

TEST(OrbitIntersection, TerminatingDifferentEnds) {
  // f(x) = x + 2 defined for x < 10: evens end at 10, odds end at 11.
  auto plus2 = [](std::int32_t x) { return x + 2; };
  EXPECT_FALSE(eop::orbit_intersection(0, 1, plus2, below10));
}

TEST(OrbitIntersection, DifferentShapes) {
  // f(x) = x + 1 below 10, stays cyclic among 20..29 otherwise.
  auto mixed = [](std::int32_t x) {
    return x >= 20 ? 20 + (x - 19) % 10 : x + 1;
  };
  auto defined = [](std::int32_t x) { return x < 10 || x >= 20; };
  EXPECT_FALSE(eop::orbit_intersection(0, 20, mixed, defined));
  EXPECT_FALSE(eop::orbit_intersection(20, 0, mixed, defined));
}

TEST(ConnectionPoint, RhoAndCircular) {
  EXPECT_EQ(eop::connection_point(0, long_rho, always), 5);
  EXPECT_EQ(eop::connection_point(0, rho, always), 3);
  EXPECT_EQ(eop::connection_point(6, long_rho, always), 6); // h = 0
  EXPECT_EQ(eop::connection_point_non_terminating_orbit(0, long_rho), 5);
}

// Exercise 2.3

namespace {
// Two handles merging into one chain that ends at 10:
//   0 -> 1 -> 2 -> 3 -> ... -> 10 (end)
//  20 -> 21 -> 22 -^   (22 joins at 3)
constexpr auto merge = [](std::int32_t x) { return x == 22 ? 3 : x + 1; };
} // namespace

TEST(ConvergentPointGuarded, SameDistanceMatchesConvergentPoint) {
  // 0 and 20 are both 10 steps from 10: plain convergent_point works too.
  EXPECT_EQ(eop::convergent_point_guarded(0, 20, 10, merge), 3);
  EXPECT_EQ(eop::convergent_point(0, 20, merge), 3);
}

TEST(ConvergentPointGuarded, DifferentDistancesFindMergePoint) {
  // 0 is 10 steps from 10, 21 is 9: plain convergent_point would never meet.
  EXPECT_EQ(eop::convergent_point_guarded(0, 21, 10, merge), 3);
  EXPECT_EQ(eop::convergent_point_guarded(21, 0, 10, merge), 3);
}

TEST(ConvergentPointGuarded, OneOrbitContainsTheOther) {
  // 5 is on 0's orbit, so they come together at 5.
  EXPECT_EQ(eop::convergent_point_guarded(0, 5, 10, succ), 5);
}

TEST(ConvergentPointGuarded, PointsOnSameCycle) {
  // convergent_point(4, 6, long_rho) loops forever; guarded terminates.
  // In a cycle every element is common, so the answer depends on y.
  EXPECT_EQ(eop::convergent_point_guarded(4, 6, 5, long_rho), 5);
  EXPECT_EQ(eop::convergent_point_guarded(4, 6, 7, long_rho), 6);
}

// Section 2.5: orbit_structure returns (m0, m1, m3):
//   terminating: (h - 1, 0,     terminal element)
//   circular:    (0,     c - 1, x)
//   rho-shaped:  (h,     c - 1, connection point)

TEST(OrbitStructure, Terminating) {
  // 0 -> ... -> 10 (end): h = 11, so m0 = 10.
  auto s = eop::orbit_structure(x0, succ, below10);
  EXPECT_EQ(s.m0, 10u);
  EXPECT_EQ(s.m1, 0u);
  EXPECT_EQ(s.m3, 10);
}

TEST(OrbitStructure, Circular) {
  // [0 -> ... -> 9 -> 0]: h = 0, c = 10.
  auto s = eop::orbit_structure(x0, mod10, always);
  EXPECT_EQ(s.m0, 0u);
  EXPECT_EQ(s.m1, 9u);
  EXPECT_EQ(s.m3, 0);
}

TEST(OrbitStructure, RhoShaped) {
  // 0..4 -> [5 -> 6 -> 7 -> 8]: h = 5, c = 4.
  auto s = eop::orbit_structure(x0, long_rho, always);
  EXPECT_EQ(s.m0, 5u);
  EXPECT_EQ(s.m1, 3u);
  EXPECT_EQ(s.m3, 5);
}

TEST(OrbitStructureNonterminatingOrbit, MatchesOrbitStructure) {
  for (auto f : {+[](std::int32_t x) { return (x + 1) % 10; },
                 +[](std::int32_t x) { return x < 8 ? x + 1 : 5; }}) {
    auto a = eop::orbit_structure_nonterminating_orbit(x0, f);
    auto b = eop::orbit_structure(x0, f, always);
    EXPECT_EQ(a.m0, b.m0);
    EXPECT_EQ(a.m1, b.m1);
    EXPECT_EQ(a.m3, b.m3);
  }
}
