#include <cstdint>
#include <iostream>
#include <limits>

template <typename T> struct is_signed_32_bit_integer {
  static const bool value = std::is_same<T, int32_t>::value;
};

// Predicate to check for overflow in addition of signed 32-bit integers
template <typename T> struct can_add_without_overflow {
  static_assert(is_signed_32_bit_integer<T>::value, "Type must be int32_t");
  static bool check(int32_t a, int32_t b) {
    // Check for overflow:
    // If both numbers are positive and their sum is negative, overflow
    // occurred. If both numbers are negative and their sum is positive,
    // overflow occurred.
    if ((b > 0) && (a > std::numeric_limits<int32_t>::max() - b)) {
      return false; // Overflow
    }
    if ((b < 0) && (a < std::numeric_limits<int32_t>::min() - b)) {
      return false; // Underflow
    }
    return true; // No overflow
  }
};
