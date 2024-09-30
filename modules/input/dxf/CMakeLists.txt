
vipra_module(
  NAME
    DXF
  SOURCES
    dxf.cpp
    drw_reader.cpp
  TYPE
    Input
)

# I hate this library oh so so so very much
set(LIBDXFRW_BUILD_DWG2DXF FALSE CACHE BOOL "Why are these not options???")
set(LIBDXFRW_BUILD_DOC FALSE CACHE BOOL "Why are these not options???")

set(TEMP_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS ON)

vipra_add_library (
  PROJECT DXF
  LIBNAME libdxfrw
  LINK dxfrw
  GIT_URL https://github.com/LibreCAD/libdxfrw.git
  GIT_TAG LC2.2.0
)

target_include_directories(
  DXF
  PUBLIC
  ${CMAKE_BINARY_DIR}/_deps/libdxfrw-src/src
)

set(BUILD_SHARED_LIBS ${TEMP_BUILD_SHARED_LIBS})