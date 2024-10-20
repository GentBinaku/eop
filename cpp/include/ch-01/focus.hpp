#ifndef EOP_CH1_FOUNDATIONS_H
#define EOP_CH1_FOUNDATIONS_H

#include "../intrinsics.hpp"

namespace eop {
/**
 * @brief Copy and move construction semantics
 *
 * @tparam _Tp0 A copy or move constructible type
 * @tparam _Tp1 Another copy or move constructible type
 */
template <copy_constructible _Tp0, copy_constructible _Tp1>
struct copy_constructor {
  void operator()(const _Tp0 &x, const _Tp1 &y) const noexcept {
    static_assert(std::is_copy_constructible_v<_Tp0> &&
                  std::is_copy_constructible_v<_Tp1>);
    construct(ptr_construct(x), _Tp0(y));
  }
};

template <move_constructible _Tp0, move_constructible _Tp1>
struct move_constructor {
  void operator()(_Tp0 &&x, _Tp1 &&y) const noexcept {
    static_assert(std::is_move_constructible_v<_Tp0> &&
                  std::is_move_constructible_v<_Tp1>);
    construct(ptr_construct(x), _Tp0(std::move(y)));
  }
};

template <copy_assignable _Tp0, copy_assignable _Tp1> struct copy_assignment {
  void operator()(const _Tp0 &x, const _Tp1 &y) const noexcept {
    static_assert(std::is_copy_constructible_v<_Tp0> &&
                  std::is_copy_constructible_v<_Tp1>);
    x = y;
  }
};

template <move_assignable _Tp0, move_assignable _Tp1> struct move_assignment {
  void operator()(_Tp0 &&x, _Tp1 &&y) const noexcept {
    static_assert(std::is_move_constructible_v<_Tp0> &&
                  std::is_move_constructible_v<_Tp1>);
    x = std::move(y);
  }
};

template <equality_comparable _Tp0, const unsigned arity,
          equality_comparable _Tp1 = _Tp0, equality_comparable... Args>
struct equal {
  inline bool operator()(const _Tp0 &x, const _Tp0 &y,
                         const Args &&...args) const noexcept {
    static_assert(eop::is_equality_comparable_v<_Tp0>);
    return (x = y) && operator()(args...);
  }
  inline bool operator()(const _Tp0 &x, const _Tp1 &y,
                         const Args &&...args) const noexcept {
    static_assert(eop::is_equality_comparable_v<_Tp0> &&
                  eop::is_equality_comparable_v<_Tp1>);
    return (x == y) && operator()(args...);
  }
  inline bool operator()(const _Tp1 &x, const _Tp0 &y,
                         const Args &&...args) const noexcept {
    static_assert(eop::is_equality_comparable_v<_Tp0> &&
                  eop::is_equality_comparable_v<_Tp1>);
    return (x == y) && operator()(args...);
  }
  inline bool operator()(const _Tp1 &x, const _Tp1 &y,
                         const Args &&...args) const noexcept {
    static_assert(eop::is_equality_comparable_v<_Tp1>);
    return (x == y) && operator()(args...);
  }
};

}; // namespace eop

#endif
