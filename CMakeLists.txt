cmake_minimum_required(VERSION 3.14)
set(VIPRA_PROJECT_NAME VIPRA)

OPTION(VIPRA_BUILD_LIBRARY ON "Build the library")
OPTION(VIPRA_BUILD_EXECUTABLE OFF "Build the executable")
OPTION(VIPRA_DEBUG_MODE OFF "Build in debug mode")
OPTION(VIPRA_PERF_TESTING OFF "Enable performance testing")

if (VIPRA_BUILD_LIBRARY AND VIPRA_BUILD_EXECUTABLE)
  message(FATAL_ERROR "Cannot build both library and executable")
endif()

if (NOT VIPRA_BUILD_LIBRARY AND NOT VIPRA_BUILD_EXECUTABLE)
  set(VIPRA_BUILD_LIBRARY ON)
endif()

if (VIPRA_BUILD_LIBRARY)
  message("Building VIPRA as library")
elseif (VIPRA_BUILD_EXECUTABLE)
  message("Building VIPRA as executable")

  if (VIPRA_DEBUG_MODE)
    message("Building in debug mode")
    set(CMAKE_BUILD_TYPE Debug)
  else()
    message("Building in release mode")
    set(CMAKE_BUILD_TYPE Release)
  endif()
endif()

project(
  ${VIPRA_PROJECT_NAME} 
  VERSION 0.0.1
  DESCRIPTION "VIPRA pedestrian dynamics simulation library"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if (VIPRA_BUILD_LIBRARY)
  add_library(
    ${VIPRA_PROJECT_NAME}
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vipra/vipra.cpp
  )
elseif (VIPRA_BUILD_EXECUTABLE)
  add_executable(
    ${VIPRA_PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  )
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if (VIPRA_PERF_TESTING)
  add_definitions(-DVIPRA_PERF_TESTING)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/vipra)
add_subdirectory(modules)

include(FetchContent)
message(STATUS "Getting nlohmann_json")
FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(nlohmann_json)
message(STATUS "Done Getting nlohmann_json")

message(STATUS "Getting libdxfrw")
FetchContent_Declare(
  libdxfrw
  GIT_REPOSITORY https://github.com/LibreCAD/libdxfrw.git
)
FetchContent_MakeAvailable(libdxfrw)
message(STATUS "Done Getting libdxfrw")


list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ExternalAntlr4Cpp)
include_directories(${ANTLR4_INCLUDE_DIRS})

find_package(MPI REQUIRED)

target_link_libraries(
  ${VIPRA_PROJECT_NAME}
  PUBLIC
  nlohmann_json::nlohmann_json
  antlr4_static
  MPI::MPI_CXX
  dxfrw
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