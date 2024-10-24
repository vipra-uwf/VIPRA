

# Gets a list of all directories in a directory
macro(SUBDIRLIST result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")

  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()

  set(${result} ${dirlist})
endmacro()

# Adds all .cpp in children directories files to the project
macro(vipra_add_all)
  file(GLOB_RECURSE TEMP_SOURCES *.cpp)

  target_sources(
    ${VIPRA_LIBRARY_NAME}
    PRIVATE
    ${TEMP_SOURCES}
  )
endmacro()

macro(vipra_add_base_modules)
  set(VIPRA_BASE_MODULES_DIR ${VIPRA_BASE_DIR}/include/vipra/base_modules)

  file(GLOB TEMP_SOURCES ${VIPRA_BASE_MODULES_DIR}/*/*.cpp ${VIPRA_BASE_MODULES_DIR}/*/*/*.cpp)

  target_sources(
    ${VIPRA_LIBRARY_NAME}
    PUBLIC
    ${TEMP_SOURCES}
  )
endmacro()

# Fetchs and adds a library from a given URL, or finds and adds it if no URL is provided
macro(vipra_add_library)
  set(options)
  set(oneValueArgs PROJECT LIBNAME URL LINK GIT_URL GIT_TAG)
  set(multiValueArgs TURN_ON TURN_OFF)
  cmake_parse_arguments(GET_LIB "${options}" "${oneValueArgs}"
    "${multiValueArgs}" ${ARGN})

  if(NOT GET_LIB_PROJECT)
    message(FATAL_ERROR "vipra_add_library missing PROJECT")
  endif()

  if(NOT GET_LIB_LIBNAME)
    message(FATAL_ERROR "vipra_add_library missing LIBNAME")
  endif()

  foreach( opt IN LISTS GET_LIB_TURN_ON )
    message(STATUS "Turning ON " ${GET_LIB_LIBNAME} ": "  ${opt})
    set (${opt} ON)
  endforeach()

  foreach( opt IN LISTS GET_LIB_TURN_OFF )
    message(STATUS "Turning OFF " ${GET_LIB_LIBNAME} ": "  ${opt})
    set (${opt} ON)
  endforeach()
  
  if(NOT GET_LIB_LINK)
      set(GET_LIB_LINK ${GET_LIB_LIBNAME})
  endif()

  
  if (TARGET ${GET_LIB_LIBNAME}_interface)
    message(STATUS "Using Pre-loaded " ${GET_LIB_LIBNAME})

    target_include_directories(${GET_LIB_PROJECT} PUBLIC ${${GET_LIB_LIBNAME}_interface_INCLUDE_DIRS})

    target_link_libraries(
        ${GET_LIB_PROJECT}
        PUBLIC
        ${GET_LIB_LIBNAME}_interface
    )
  else()

    # interface not already created for library
    find_package(${GET_LIB_LIBNAME} QUIET)

    if (NOT ${GET_LIB_LIBNAME}_FOUND)
    if( NOT GET_LIB_URL AND NOT GET_LIB_GIT_URL )
      message(STATUS "Searching Locally")
      find_package(${GET_LIB_LIBNAME} REQUIRED)
    else()
      message(STATUS "Getting " ${GET_LIB_LIBNAME})
      include(FetchContent)

      if ( GET_LIB_GIT_URL )
        message(STATUS "Fetching From Git " ${GET_LIB_GIT_URL} ":" ${GET_LIB_GIT_TAG})
        FetchContent_Declare(${GET_LIB_LIBNAME} GIT_REPOSITORY ${GET_LIB_GIT_URL} GIT_TAG ${GET_LIB_GIT_TAG})
      else()
        message(STATUS "Fetching From URL " ${GET_LIB_URL})
        FetchContent_Declare(${GET_LIB_LIBNAME} URL ${GET_LIB_URL})
      endif()
      
      FetchContent_MakeAvailable(${GET_LIB_LIBNAME})
      FetchContent_GetProperties(${GET_LIB_LIBNAME})

      if(NOT ${GET_LIB_LIBNAME}_POPULATED)
        message(FATAL_ERROR "Unable to Get " ${GET_LIB_LIBNAME})
      endif()

    endif()
    endif()

    target_include_directories(${GET_LIB_PROJECT} PUBLIC ${${GET_LIB_LIBNAME}_INCLUDE_DIRS})

    target_link_libraries(
      ${GET_LIB_PROJECT}
      PUBLIC
      ${GET_LIB_LINK}
    )

    # create an interface library to look for in the future
    add_library(${GET_LIB_LIBNAME}_interface INTERFACE)
    target_include_directories(${GET_LIB_LIBNAME}_interface INTERFACE ${${GET_LIB_LIBNAME}_INCLUDE_DIRS})
    target_link_libraries(${GET_LIB_LIBNAME}_interface INTERFACE ${GET_LIB_LINK})

    message(STATUS "Done Getting " ${GET_LIB_LIBNAME})
  endif()
endmacro()

macro(vipra_find_all_modules)
  # Search recursively for all files named 'module'
  file(GLOB_RECURSE MODULE_FILES RELATIVE ${CMAKE_SOURCE_DIR} "*/vipra.module")

  # Extract unique directories containing 'module'
  set(VIPRA_MODULE_DIRECTORIES)
  foreach(MODULE_FILE ${MODULE_FILES})
    get_filename_component(DIR ${MODULE_FILE} DIRECTORY)
    list(APPEND VIPRA_MODULE_DIRECTORIES ${DIR})
  endforeach()

  # Remove duplicate entries
  list(REMOVE_DUPLICATES VIPRA_MODULE_DIRECTORIES)
endmacro(vipra_find_all_modules)

macro(vipra_module)
  set(options)
  set(oneValueArgs NAME TYPE)
  set(multiValueArgs SOURCES)
  cmake_parse_arguments(VIPRA_MODULE "${options}" "${oneValueArgs}"
    "${multiValueArgs}" ${ARGN})

  set(
    VIPRA_MODULE_TYPES
    goals
    model
    input
    output
    map
    pedestrians
  )

  string(TOLOWER ${VIPRA_MODULE_TYPE} VIPRA_MODULE_TYPE)

  if (NOT DEFINED VIPRA_MODULE_TYPE)
    message(FATAL_ERROR "${VIPRA_MODULE_NAME} : TYPE was not provided")
  endif()
  if (NOT VIPRA_MODULE_TYPE IN_LIST VIPRA_MODULE_TYPES)
    message(FATAL_ERROR "${VIPRA_MODULE_NAME} : ${VIPRA_MODULE_TYPE} is not a valid type\nOptions: ${VIPRA_MODULE_TYPES}")
  endif()

  project(${VIPRA_MODULE_NAME})

  # TODO(rolland): need to properly link the json module instead of cramming it into every module
  add_library(
    ${VIPRA_MODULE_NAME}
    SHARED
    ${VIPRA_MODULE_SOURCES}
  )

  target_link_libraries(
    ${VIPRA_MODULE_NAME}
    PUBLIC
    ${VIPRA_LIBRARY_NAME}
  )

  target_include_directories(
    ${VIPRA_MODULE_NAME}
    PUBLIC
    ${VIPRA_INCLUDE_DIRS}
  )

  set_target_properties(
    ${VIPRA_MODULE_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${VIPRA_BASE_DIR}/install/${VIPRA_MODULE_TYPE}
  )
endmacro(vipra_module)