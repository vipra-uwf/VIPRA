cmake_minimum_required(VERSION 3.19)
set(VIPRA_LIBRARY_NAME vipra)
set(VIPRA_EXECUTABLE_NAME VIPRA)

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
  ${VIPRA_LIBRARY_NAME}
  VERSION 0.0.1
  DESCRIPTION "VIPRA pedestrian dynamics simulation library"
  LANGUAGES CXX
)

# BUILD VIPRA Library

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake.utils)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake.options)

add_library(
  ${VIPRA_LIBRARY_NAME}
  SHARED
)

add_subdirectory(src)
add_subdirectory(include/vipra/vipra_behaviors/_grammar)
vipra_add_base_modules()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ExternalAntlr4Cpp)
include_directories(${ANTLR4_INCLUDE_DIRS})

vipra_add_library(
  PROJECT ${VIPRA_LIBRARY_NAME}
  LIBNAME nlohmann_json
  GIT_URL https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

target_link_libraries(
  ${VIPRA_LIBRARY_NAME}
  PUBLIC
  antlr4_shared
)

target_include_directories(
  ${VIPRA_LIBRARY_NAME}
  PUBLIC
  ${VIPRA_INCLUDE_DIRS}
  ${ANTLR4CPP_INCLUDE_DIRS}
)

if (VIPRA_BUILD_EXECUTABLE)
  project(
    ${VIPRA_EXECUTABLE_NAME}
    VERSION 0.0.1
    DESCRIPTION "VIPRA pedestrian dynamics simulation executable"
    LANGUAGES CXX
  )

  add_executable(
    ${VIPRA_EXECUTABLE_NAME}
    ${VIPRA_MAIN_FILE}
  )

    target_link_libraries(
      ${VIPRA_EXECUTABLE_NAME}
      PUBLIC
      ${VIPRA_LIBRARY_NAME}
    )

  target_include_directories(
    ${VIPRA_EXECUTABLE_NAME}
    PUBLIC
    ${VIPRA_INCLUDE_DIRS}
  )
endif()

vipra_find_all_modules()
foreach(SUBDIR ${VIPRA_MODULE_DIRECTORIES})
    add_subdirectory(${SUBDIR})
endforeach()