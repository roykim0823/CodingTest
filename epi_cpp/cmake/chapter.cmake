# Shared build logic for every chapter. A chapter CMakeLists.txt is:
#
#   cmake_minimum_required(VERSION 3.20)
#   project(NN_ChapterName)
#   # optional: set(EPI_EXCLUDE_SRCS "<regex>") for files that must not build
#   include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/chapter.cmake)

set(CMAKE_CXX_FLAGS "-std=c++1z -O2 -g -Wall")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")          # test_framework/
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")  # prototype headers

file(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/cpp/*.cc")

if(DEFINED EPI_EXCLUDE_SRCS)
    list(FILTER SRCS EXCLUDE REGEX "${EPI_EXCLUDE_SRCS}")
endif()

# One executable per solution file; target name = filename stem.
foreach(src IN LISTS SRCS)
    get_filename_component(target ${src} NAME_WLE)
    add_executable(${target} ${src})
endforeach()
