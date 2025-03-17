cmake_minimum_required(VERSION 3.19)
set(BADL_LIBRARY_NAME BADL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(
  BADL_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(
  BADL_BASE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/badl
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/badl_funcs.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/utils.cmake)

add_compile_definitions(-DBADL_BUILD_MACRO_ON)

project(
  ${BADL_LIBRARY_NAME}
  VERSION 0.0.1
  DESCRIPTION ""
  LANGUAGES CXX
)

add_library(
  ${BADL_LIBRARY_NAME}
  INTERFACE
)

target_include_directories(
  ${BADL_LIBRARY_NAME}
  INTERFACE
  ${BADL_INCLUDE_DIRS}
)

execute_process(
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/cmake/agent.py ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR} ${BADL_AGENT_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)

if(result EQUAL 0)
    message(STATUS " Python utility executed successfully: ${output}")
else()
    message(ERROR " Python utility failed with error code ${result}: ${error}")
endif()