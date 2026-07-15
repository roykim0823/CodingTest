cmake_minimum_required(VERSION 3.20)
project(epi)

# Builds every chapter that has a CMakeLists.txt. Each chapter is also a
# standalone project: cmake -S 01_PrimitiveType -B 01_PrimitiveType/build
file(GLOB chapter_dirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     ${CMAKE_CURRENT_SOURCE_DIR}/[0-2][0-9]_*)
foreach(dir IN LISTS chapter_dirs)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt)
        add_subdirectory(${dir})
    endif()
endforeach()
