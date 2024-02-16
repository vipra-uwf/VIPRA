cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME VIPRA)

project(
  ${PROJECT_NAME} 
  VERSION 0.0.1
  DESCRIPTION "VIPRA pedestrian dynamics simulation library"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_library(
  ${PROJECT_NAME}
  STATIC
  ${PROJECT_SOURCE_DIR}/include/vipra/vipra.cpp
)

add_subdirectory(${PROJECT_SOURCE_DIR}/include/vipra)

include(FetchContent)
message(STATUS "Getting nlohmann_json")
FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(nlohmann_json)
message(STATUS "Done Getting nlohmann_json")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ExternalAntlr4Cpp)
include_directories(${ANTLR4_INCLUDE_DIRS})

find_package(MPI REQUIRED)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  nlohmann_json::nlohmann_json
  antlr4_static
  MPI::MPI_CXX
)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/include/vipra/vipra_behaviors
  ${ANTLR4CPP_INCLUDE_DIRS}
)