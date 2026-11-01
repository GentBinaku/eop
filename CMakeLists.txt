cmake_minimum_required(VERSION 4.0)
project(eop VERSION 0.2.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CTest)

add_library(eop INTERFACE)
add_library(eop::eop ALIAS eop)

target_compile_features(eop INTERFACE cxx_std_26)
target_include_directories(eop INTERFACE
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

install(TARGETS eop EXPORT eopTargets)
install(DIRECTORY include/eop DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
