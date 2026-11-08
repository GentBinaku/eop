find_package(GTest 1.14 CONFIG QUIET)
if(NOT GTest_FOUND)
  include(FetchContent)
  FetchContent_Declare(googletest
                       URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.tar.gz)
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()

include(GoogleTest)

file(GLOB_RECURSE EOP_TEST_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/*_test.cpp")
add_executable(eop_tests ${EOP_TEST_SOURCES})
target_link_libraries(eop_tests PRIVATE eop::eop GTest::gtest_main)
gtest_discover_tests(eop_tests)
