cmake_minimum_required(VERSION 3.12)

# Determine if libtd is built as a subproject (using add_subdirectory)
# or if it is the master project.
if (NOT DEFINED LIBTD_MASTER_PROJECT)
  set(LIBTD_MASTER_PROJECT OFF)
  if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(LIBTD_MASTER_PROJECT ON)
  endif ()
endif ()

# Set the default CMAKE_BUILD_TYPE to Release.
if (LIBTD_MASTER_PROJECT AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif ()
project(libtd C)
set(LIBTD_SOURCES
  src/gc.c
  src/argv.c
  src/string.c
)

option(LIBTD_TEST "Generate the test target." ${LIBTD_MASTER_PROJECT})

if (LIBTD_MASTER_PROJECT)
  message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()

# Add the library and remove the "lib" prefix.
add_library(libtd ${LIBTD_SOURCES})
target_include_directories(libtd PUBLIC include/)
set_target_properties(libtd PROPERTIES PREFIX "")

# Enable warnings.
if (MSVC)
  target_compile_options(libtd PRIVATE /W4 /WX)
else()
  target_compile_options(libtd PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

if (LIBTD_TEST)
  add_subdirectory(test)
endif ()

# vim: et ts=8 sts=2 sw=2
