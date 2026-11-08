#include <concepts>
#include <cstdint>
#include <eop/ch-02/transformation.hpp>
#include <gtest/gtest.h>

namespace {

constexpr auto succ = [](std::int32_t x) -> std::int32_t { return x + 1; };
constexpr auto add = [](std::int32_t x, std::int32_t y) { return x + y; };
constexpr auto pos = [](std::int32_t x) { return x > 0; };

// Concept checks are compile-time: a failure stops the test build.
static_assert(eop::transformation<decltype(succ), std::int32_t>);
static_assert(!eop::transformation<decltype(add), std::int32_t>); // binary
static_assert(
    !eop::transformation<decltype(pos), std::int32_t>); // returns bool

static_assert(std::same_as<eop::distance_type<decltype(succ), std::int32_t>,
                           std::uint32_t>);

} // namespace

// Placeholder so the static_asserts above show up in the test report.
TEST(Transformation, ConceptChecksCompile) { SUCCEED(); }
