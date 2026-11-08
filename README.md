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
tests/                                        GoogleTest tests, one file per header
src/main.cpp                                  scratch program
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
cmake -B build
cmake --build build
ctest --test-dir build
```

Tests use GoogleTest and live in `tests/`, mirroring `include/eop/`
(`tests/ch-02/collision_point_test.cpp` tests `ch-02/collision_point.hpp`).
Any file named `*_test.cpp` under `tests/` is picked up automatically. CMake
uses an installed GoogleTest if it finds one and downloads it otherwise.

To check that each header compiles on its own (that it includes everything it
needs), build the verification target:

```sh
cmake --build build --target all_verify_interface_header_sets
```
