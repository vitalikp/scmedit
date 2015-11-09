# main scmedit cmake file
project(scmedit)
cmake_minimum_required(VERSION 3.0)

set(VERSION 0.1-dev)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

# directories
set(bindir ${CMAKE_INSTALL_PREFIX}/bin)

# check program and library
#include(check.cmake)

# config
configure_file(config.h.cmake config.h)
add_definitions(-include ${PROJECT_BINARY_DIR}/config.h)

# source code
include_directories(src)
add_subdirectory(src)

# show current config
message("   ${CMAKE_PROJECT_NAME} ${VERSION}")
message("      prefix:      ${CMAKE_INSTALL_PREFIX}")