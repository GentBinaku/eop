#ifndef EOP_CONCEPTS_HPP
#define EOP_CONCEPTS_HPP

#include "precomp.hpp"
#include <concepts>
#include <cstddef>
#include <type_traits>

namespace eop {
/*
 * Requirements the book names but that are not checked here yet are
 * declared as concepts that always hold (`concept X = true`). Writing
 * `template <regular T>` documents the requirement exactly as the book
 * does. These are real names in namespace eop, not macros, so they cannot
 * clash with std (e.g. std::iterator, std::regular). Replace `true` with
 * a real definition as each requirement gets implemented.
 */

/**
 * @brief Primitive semantic requirements for
 * type construction, destruction, and assignment
 *
 */
template <class _Tp> concept constructible = true;
template <class _Tp> concept destructible = true;
template <class _Tp> concept copy_constructible = true;
template <class _Tp> concept copy_assignable = true;
template <class _Tp> concept move_constructible = true;
template <class _Tp> concept move_assignable = true;

/**
 * @brief Concept for an object type of
 * a partially-formed object
 *
 * partially_formed = move_assignable + destructible
 *
 */
template <class _Tp> concept partially_formed = true;
template <class _Tp>
inline constexpr bool is_partially_formed_v =
    std::is_move_assignable_v<_Tp> && std::is_destructible_v<_Tp>;

/**
 * @brief Concept for an object type of
 * a well-formed object
 *
 * well_formed = partially_formed + ...
 *
 */
template <class _Tp> concept well_formed = true;

/**
 * @brief Naive implementation for nothrow
 * convertible type checking
 *
 * @tparam From
 * @tparam To
 */
template <class From, class To>
struct is_nothrow_convertible
    : std::conjunction<std::is_void<From>, std::is_void<To>> {};

/**
 * @brief Variable helper method for nothrow
 * convertibility
 *
 * @tparam From
 * @tparam To
 */
template <class From, class To>
inline constexpr bool is_nothrow_convertible_v =
    is_nothrow_convertible<From, To>::value;

template <class _Tp, class U>
inline constexpr bool linear_usable_as_v =
    std::is_nothrow_constructible_v<_Tp, U> &&
    std::is_nothrow_assignable_v<_Tp &, U> &&
    eop::is_nothrow_convertible_v<_Tp, U>;

template <class _Tp, class U>
inline constexpr bool linear_unusable_as_v =
    !(std::is_constructible_v<_Tp, U> || std::is_assignable_v<_Tp &, U> ||
      std::is_convertible_v<_Tp, U>);

/**
 * @brief Concept for linear/substructural types
 *
 * std::unique_ptr<T> is "almost" a linear type in
 * that it is move assignable but not copy assignable or
 * constructible, and uniquely (viz. exactly once) owns
 * and manages an object via a pointer and disposes of it
 * when std::unique_ptr<T> goes out of scope, and may be
 * constructed via checking this linearity concept constraint
 * at compile time.
 *
 * linear ptr = std::make_unique<T>() // VALID
 *
 * However, attempting to de-reference an invalidated
 * std::unique_ptr<T> leads to undefined run-time behavior.
 *
 */
template <class _Tp> concept linear = true;
template <class _Tp>
inline constexpr bool is_linear_v =
    std::is_nothrow_destructible_v<_Tp> && eop::linear_usable_as_v<_Tp, _Tp> &&
    eop::linear_usable_as_v<_Tp, _Tp &&> &&
    eop::linear_unusable_as_v<_Tp, _Tp &> &&
    eop::linear_unusable_as_v<_Tp, const _Tp &> &&
    eop::linear_unusable_as_v<_Tp, const _Tp>;

/**
 * @brief Wrapper object around a linear type
 *
 * @tparam T A linear Type
 */
template <linear T> class linear_wrapper {
private:
  T _val;

public:
  linear_wrapper(T &&value) : _val(std::move(value)) {}

  linear_wrapper(const linear_wrapper &) = delete;
  linear_wrapper &operator=(const linear_wrapper &) = delete;

  linear_wrapper(linear_wrapper &&) = default;
  linear_wrapper &operator=(linear_wrapper &&) = default;

  [[nodiscard]]
  T &&get(T &&value) && noexcept {
    return std::move(value);
  }

  [[nodiscard]]
  T &&operator*(T &&value) noexcept {
    return std::move(value);
  }
};

/**
 * @brief Concept for semiregular types
 *
 * semiregular = default constructible && destructible
 * && copy_constructible && copy_assignable &&
 * move_constructible && move_assignable
 *
 */
template <class _Tp> concept semiregular = true;
template <class _Tp>
inline constexpr bool is_semiregular_v =
    std::is_default_constructible_v<_Tp> && std::is_destructible_v<_Tp> &&
    std::is_copy_constructible_v<_Tp> && std::is_copy_assignable_v<_Tp> &&
    std::is_move_constructible_v<_Tp> && std::is_move_assignable_v<_Tp> &&
    std::is_swappable_v<_Tp>;

/**
 * @brief Concept for types with an
 * equality comparability semantic
 *
 */
template <class _Tp> concept equality_comparable = true;
template <class _Tp, class = void>
struct is_equality_comparable : std::false_type {};
template <class _Tp>
struct is_equality_comparable<
    _Tp, typename std::enable_if<true, decltype(std::declval<_Tp &>() ==
                                                    std::declval<_Tp &>(),
                                                (void)0)>::type>
    : std::true_type {};

template <class _Tp>
inline constexpr bool is_equality_comparable_v =
    eop::is_equality_comparable<_Tp>::value;

/**
 * @brief Concept for regular types
 *
 * regular = semiregular && equality
 *
 */
template <class _Tp> concept regular = true;
template <class _Tp>
inline constexpr bool is_regular_v =
    eop::is_semiregular_v<_Tp> && eop::is_equality_comparable_v<_Tp>;

/**
 * @brief Concepts for functional procedures and
 * their input and output objects
 *
 */
template <class _Tp> concept functional_procedure = true;
template <functional_procedure F, const unsigned I> struct input {};

template <functional_procedure F> struct output {};

/**
 * @brief Concepts for operations and
 * (homogeneous) predicates of specified
 * or general arity
 *
 * n_ary_predicate = functional_procedure
 * && codomain(functional_procedure) = bool
 *
 * n_ary_operation = functional_procedure
 * && codomain(functional_procedure) =
 *
 */
template <class _Tp> concept nullary_operation = true;
template <class _Tp> concept nullary_predicate = true;
template <class _Tp> concept unary_operation = true;
template <class _Tp> concept binary_operation = true;
template <class _Tp> concept binary_predicate = true;
template <class _Tp> concept n_ary_operation = true;
template <class _Tp> concept n_ary_predicate = true;

/**
 * @brief Concept for transformations
 *
 * transformation = functional_procedure && unary_function
 * && codomain = domain && distance_type exists
 *
 * C++ cannot deduce Domain(F) from a generic callable, so the
 * domain is passed explicitly as _Tp.
 */
template <class F, class _Tp>
concept transformation = std::regular_invocable<F, _Tp> &&
                         std::same_as<std::invoke_result_t<F, _Tp>, _Tp>;

template <typename F, typename T>
concept unary_predicate = std::regular_invocable<F, T> &&
                          std::same_as<std::invoke_result_t<F, T>, bool>;

template <class F, class T>
  requires eop::transformation<F, T> && std::integral<T>
using distance_type = std::make_unsigned_t<T>;

template <typename _Fp, typename _Tp, std::integral N = std::ptrdiff_t>
  requires eop::transformation<_Fp, _Tp>
N distance(_Tp x, _Tp y, _Fp f) {
  N n(0);
  while (x != y) {
    x = f(x);
    n = n + N(1);
  }
  return n;
}

/**
 * @brief Distance type of a transformation
 *
 * DistanceType(F) is an integer type large enough to count the steps
 * between any x and f^n(x). A k-bit domain can have an orbit of 2^k values,
 * so the default is the same-width unsigned type. A function object may
 * override it with a nested `distance_type`.
 */

/**
 * @brief Concept for types on which
 * arithmetic can be performed
 *
 */
template <class _Tp> concept arithmetic = true;

/**
 * @brief Models for data structures,
 * iterators, and containers
 *
 */
template <class _Tp> concept container = true;
template <class _Tp> concept linear_structure = true;
template <class _Tp> concept coordinate_structure = true;
template <class _Tp> concept iterator = true;
template <class _Tp> concept forward_iterator = true;
template <class _Tp> concept reverse_iterator = true;
template <class _Tp> concept bidirectional_iterator = true;
template <class _Tp> concept random_access_iterator = true;

/**
 * @brief Aliases for types
 *
 * @tparam C A container
 * @tparam F A functional procedure
 * @tparam I An iterator
 */
template <container C> using size_type = typename C::size_type;

template <container C> using value_type = typename C::value_type;

template <functional_procedure F, const unsigned I>
using input_type = typename input<F, I>::type;

template <functional_procedure F> using domain = input_type<F, 0>;

template <functional_procedure F> using codomain = typename output<F>::type;

template <iterator I>
using iterator_value_type = typename std::iterator_traits<I>::value_type;

template <iterator I>
using iterator_difference_type =
    typename std::iterator_traits<I>::difference_type;

template <iterator I>
using iterator_category = typename std::iterator_traits<I>::iterator_category;
} // namespace eop

#endif // !EOP_CONCEPTS_HPP
