#ifndef EOP_CH2_TRANSFORMATION_HPP
#define EOP_CH2_TRANSFORMATION_HPP

#include <cassert>
#include <concepts>

#include "../concepts.hpp"

namespace eop {

/**
 * @brief Apply f to x, n times: f^n(x)  (EOP 2.2)
 *
 * T is Domain(F); N is any integer type used only as the count.
 *
 * Precondition: n >= 0 and f^i(x) is defined for 0 < i <= n
 */
template <typename F, typename T, std::integral N>
  requires transformation<F, T>
constexpr T power_unary(T x, N n, F f) {
  if constexpr (std::signed_integral<N>)
    assert(n >= N(0));
  while (n != N(0)) {
    n = n - N(1);
    x = f(x);
  }
  return x;
}

} // namespace eop

#endif // !EOP_CH2_TRANSFORMATION_HPP
