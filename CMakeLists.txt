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

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include
)

include(FetchContent)
message(STATUS "Getting nlohmann_json")
FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(nlohmann_json)
message(STATUS "Done Getting nlohmann_json")

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  nlohmann_json::nlohmann_json
)