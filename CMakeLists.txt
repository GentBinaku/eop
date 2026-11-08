cmake_minimum_required(VERSION 4.0)
project(eop VERSION 0.2.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(eop INTERFACE)
add_library(eop::eop ALIAS eop)

target_compile_features(eop INTERFACE cxx_std_26)
target_include_directories(eop INTERFACE
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# Register the headers as a file set so CMake can compile each one standalone
# (eop_verify_interface_header_sets). This checks headers are self-contained
# and gives compile_commands.json an entry per header for clangd.
file(GLOB_RECURSE EOP_HEADERS CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/include/eop/*.hpp")
target_sources(eop INTERFACE
               FILE_SET HEADERS
               BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
               FILES ${EOP_HEADERS})
set_target_properties(eop PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)

install(TARGETS eop EXPORT eopTargets FILE_SET HEADERS)

# Scratch executable for exercising the library (not installed)
add_executable(eop_main src/main.cpp)
target_link_libraries(eop_main PRIVATE eop::eop)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
