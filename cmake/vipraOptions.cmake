
option(VIPRA_BUILD_EXECUTABLE OFF "Build as an executable")
option(VIPRA_DEBUG_MODE OFF "Build in debug mode")
option(VIPRA_PERF_TESTING OFF "Enable performance profiling")
option(VIPRA_USE_GPERF OFF "Use Gperftools for profiling")
option(VIPRA_USE_MPI OFF "Enable MPI for parameter sweeps")
option(VIPRA_TIME_SIM OFF "Output Timings for simulation runs")
option(VIPRA_TESTING OFF "Build VIPRA Tests")

if ( VIPRA_DEBUG_MODE )
  message("Building in debug mode")
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment")
  set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment")
else()
  message("Building in release mode")
  set(CMAKE_BUILD_TYPE Release)
endif()

if ( VIPRA_PERF_TESTING )
  add_compile_options(-pg -g)
  add_definitions(-DVIPRA_PERF_TESTING)
endif()

if ( VIPRA_USE_GPERF )
  target_link_libraries(${VIPRA_EXECUTABLE_NAME} PUBLIC tcmalloc_and_profiler)
endif()

# Only Include MPI if the option is turned on
if ( VIPRA_USE_MPI )
  add_definitions(-DVIPRA_USE_MPI)
  vipra_add_library(
    PROJECT ${VIPRA_LIBRARY_NAME}
    LIBNAME MPI
    LINK MPI::MPI_CXX
  )
  include_directories(SYSTEM ${MPI_CXX_INCLUDE_PATH})
endif()

if ( VIPRA_TIME_SIM )
  add_definitions(-DVIPRA_TIME_SIM)
endif()