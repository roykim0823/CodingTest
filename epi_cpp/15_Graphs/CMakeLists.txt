cmake_minimum_required(VERSION 3.20)
project(15_Graphs)

# LeetCode snippet files without a main(); not buildable as executables.
set(EPI_EXCLUDE_SRCS "cpp/(10_number_of_islands|11_rotting_oranges|12_walls_and_gates|13_clone_graph)\\.cc$")

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/chapter.cmake)
