cmake_minimum_required(VERSION 3.14)
set(VIPRA_PROJECT_NAME VIPRA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

project(
  ${VIPRA_PROJECT_NAME}
  VERSION 0.0.1
  DESCRIPTION "VIPRA pedestrian dynamics simulation library"
  LANGUAGES CXX
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake.utils)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake.options)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/vipra)
add_subdirectory(modules)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ExternalAntlr4Cpp)
include_directories(${ANTLR4_INCLUDE_DIRS})

target_link_libraries(
  ${VIPRA_PROJECT_NAME}
  PUBLIC
  antlr4_static
)

target_include_directories(
  ${VIPRA_PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/vipra/vipra_behaviors
  ${CMAKE_CURRENT_SOURCE_DIR}/modules
  ${CMAKE_CURRENT_SOURCE_DIR}/deps
  ${ANTLR4CPP_INCLUDE_DIRS}
)

add_compile_options(-g)