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
T collision_point_nonterminating_orbit(const T &x, F f) {
  T slow = x;
  T fast = f(x);

  while (fast != slow) {
    slow = f(slow);
    fast = f(fast);
    fast = f(fast);
  }
  return fast;
}

template <typename F, typename T>
  requires eop::transformation<F, T>
bool circular_nonterminating_orbit(T const &x, F f) {
  return x == f(collision_point_nonterminating_orbit(x, f));
}

template <typename F, typename T, typename P>
  requires eop::transformation<F, T> && std::predicate<P &, const T &>
bool circular(T const &x, F f, P p) {
  T y = collision_point(x, f, p);
  return p(y) && x == f(y);
}

template <typename F, typename T>
T convergent_point(T x0, T x1, F f)
  requires eop::transformation<F, T>
{
  // DO i need the precondition
  while (x0 != x1) {
    x0 = f(x0);
    x1 = f(x1);
  }
  return x0;
}

template <typename F, typename T>
  requires eop::transformation<F, T>
T connection_point_non_terminating_orbit(const T &x, F f) {
  return convergent_point(x, f(collision_point_nonterminating_orbit(x, f)), f);
}

template <typename F, typename P, typename T>
  requires eop::transformation<F, T> && std::predicate<P &, const T &>
// Domain(F) == Domain(P) would be done by T
T connection_point(T const &x, F f, P p) {
  T y = collision_point(x, f, p);
  if (!p(y))
    return y;
  return convergent_point(x, f(y), f);
}

template <typename F, typename T, typename P>
  requires eop::transformation<F, T> && std::predicate<P &, const T &>
bool orbit_intersection(T const &x0, T const &x1, F f, P p) {
  // Exercise 2.2. Once two orbits share an element they share everything
  // after it, so they must end the same way.
  T y1 = collision_point(x0, f, p);
  bool t1 = !p(y1);
  T y2 = collision_point(x1, f, p);
  bool t2 = !p(y2);

  // Different shapes: one stops, the other loops forever.
  if (t1 != t2)
    return false;

  // Both terminate: y1 and y2 are the ends; they must be the same end.
  if (t1)
    return y1 == y2;

  // Neither terminates: y1 and y2 are both in cycles. Walk one lap around
  // y1's cycle looking for y2. Returning to y1 means the lap is complete.
  T y = y1;
  do {
    if (y == y2)
      return true;
    y = f(y);
  } while (y != y1);
  return false;
}

template <typename F, typename T>
  requires eop::transformation<F, T>
T convergent_point_guarded(T &x0, T &x1, F f) {
  // To do find a intersection point and start from there
}
} // namespace eop
#endif // !EOP_CH2_COLLISION_POINT_HPP
