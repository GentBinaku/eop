#ifndef EOP_CH2_COLLISION_POINT_HPP
#define EOP_CH2_COLLISION_POINT_HPP

#include <cassert>
#include <concepts>

#include "../concepts.hpp"

namespace eop {

template <typename F, typename P, typename T>
  requires eop::transformation<F, T> && std::predicate<P &, const T &>
T collision_point(const T &x, F f, P p) {
  if (!p(x))
    return x;

  T slow = x;
  T fast = f(x);

  while (fast != slow) {
    slow = f(slow);
    if (!p(fast))
      return fast;
    fast = f(fast);
    if (!p(fast))
      return fast;
    fast = f(fast);
  }
  return fast;
}

template <typename F, typename P, typename T>
  requires eop::transformation<F, T> && std::predicate<P &, const T &>
bool terminating(const T &x, F f, P p) {
  return !p(collision_point(x, f, p));
}

template <typename F, typename T>
  requires eop::transformation<F, T>
T &collision_point_nonterminating_orbit(const T &x, F f) {
  T slow = x;
  T fast = f(x);

  while (fast != slow) {
    slow = f(slow);
    fast = f(fast);
    fast = f(fast);
  }
  return fast;
}
} // namespace eop
#endif // !EOP_CH2_COLLISION_POINT_HPP
