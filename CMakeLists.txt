project(langeNacht-2015)
cmake_minimum_required(VERSION 2.8)

include(CMakeLists.mira)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

add_subdir(toolboxes)
add_subdir(domains)
