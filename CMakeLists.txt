cmake_minimum_required(VERSION 3.19)
set(VIPRA_PROJECT_NAME VIPRA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(VIPRA_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(
  VIPRA_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/modules
  ${CMAKE_CURRENT_SOURCE_DIR}/deps
)


project(
  ${VIPRA_PROJECT_NAME}
  VERSION 0.0.1
  DESCRIPTION "VIPRA pedestrian dynamics simulation library"
  LANGUAGES CXX
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake.utils)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake.options)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ExternalAntlr4Cpp)
include_directories(${ANTLR4_INCLUDE_DIRS})

vipra_add_library(
  LIBNAME nlohmann_json
  GIT_URL https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

add_subdirectory(src)
add_subdirectory(include/vipra)
file(GLOB VIPRA_BASE_SOURCES src/*.cpp src/*.hpp src/*/*.cpp src/*/*.hpp)

vipra_add_base_modules()

vipra_find_all_modules()
foreach(SUBDIR ${VIPRA_MODULE_DIRECTORIES})
    add_subdirectory(${SUBDIR})
endforeach()

target_link_libraries(
  ${VIPRA_PROJECT_NAME}
  PUBLIC
  antlr4_static
)

target_include_directories(
  ${VIPRA_PROJECT_NAME}
  PUBLIC
  ${VIPRA_INCLUDE_DIRS}
  ${ANTLR4CPP_INCLUDE_DIRS}
)