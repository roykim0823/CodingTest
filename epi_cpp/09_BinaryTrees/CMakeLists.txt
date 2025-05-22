cmake_minimum_required(VERSION 3.5)
project(10)

set(CMAKE_CXX_FLAGS "-std=c++1z -g -Wall")

include_directories(include)
file(GLOB SRCS "src/*.cc")

foreach(src IN LISTS SRCS) 
#	message(STATUS "${src}")
	get_filename_component(filename ${src} NAME)
	string(REPLACE "." ";" temp ${filename})
	list(GET temp 0 target)
	add_executable(${target} ${src})
	list(APPEND TARGET_LIST ${target})
endforeach()
