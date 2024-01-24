
cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME vipra_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_BUILD_TYPE Debug)

project(
  ${PROJECT_NAME}
  VERSION 0.1
  DESCRIPTION "VIPRA Pedestrian Dynamics Simulation"
  LANGUAGES CXX
)

add_executable(
  ${PROJECT_NAME}
  testing.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(../ VIPRA)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  GTest::gtest_main
  GTest::gmock_main
  VIPRA
)

add_subdirectory(vipra)