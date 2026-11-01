#ifndef EOP_CH2_SIGNED_PREDICATE_HPP
#define EOP_CH2_SIGNED_PREDICATE_HPP

#include <concepts>
#include <limits>

namespace eop {

// True when a + b is representable in T.
//
// Signed overflow is undefined behaviour, so we cannot compute a + b and then
// inspect the result. Instead compare against the bound on the side b moves
// toward; neither max - b (for b > 0) nor min - b (for b < 0) can overflow.
template <std::signed_integral T>
constexpr bool can_add_without_overflow(T a, T b) noexcept {
  if (b > 0)
    return a <= std::numeric_limits<T>::max() - b;
  if (b < 0)
    return a >= std::numeric_limits<T>::min() - b;
  return true;
}

} // namespace eop

#endif // !EOP_CH2_SIGNED_PREDICATE_HPP
