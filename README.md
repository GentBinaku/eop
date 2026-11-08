# eop

Working through *Elements of Programming* by Alexander Stepanov and Paul McJones
(Addison-Wesley, 2009) in modern C++.

The book builds programming up from mathematics: values, objects, and
procedures, then the algebraic structures (transformations, orbits,
semigroups, iterators, ...) that generic algorithms are written against. This
repository reimplements its definitions and algorithms one chapter at a time,
using C++26 concepts where the book uses its own requirement notation.

The book is freely available from the authors at
<http://elementsofprogramming.com>.

## Layout

```
include/eop/
  concepts.hpp, intrinsics.hpp, precomp.hpp   shared building blocks
  ch-01/foundations.hpp                       Ch. 1  Foundations
  ch-02/transformation.hpp                    Ch. 2  Transformations and their orbits
  ch-02/signed_predicate.hpp
  ch-02/collision_point.hpp
src/main.cpp                                  scratch tests exercising the library
```

The library is header-only and exported as the CMake target `eop::eop`.

## Progress

- **Chapter 1: Foundations.** Construction and destruction, partially formed
  values, assignment.
- **Chapter 2: Transformations and Their Orbits.** The `transformation`
  concept, `distance_type`, `power_unary`, and `collision_point` and
  `terminating` (Floyd's tortoise and hare).

## Building

Requires CMake 4.0+ and a compiler with C++26 support.

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/eop_main
```

The tests in `src/main.cpp` are plain `assert`s. Release builds define
`NDEBUG`, which compiles them out, so build in Debug to run them. A silent
exit means every test passed.

To check that each header compiles on its own (that it includes everything it
needs), build the verification target:

```sh
cmake --build build --target all_verify_interface_header_sets
```
