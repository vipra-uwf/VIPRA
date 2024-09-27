
vipra_module(
  NAME
    DXF
  SOURCES
    dxf.cpp
  TYPE
    Input
)

set(LIBDXFRW_BUILD_DWG2DXF FALSE CACHE BOOL "Why are these not options???")
set(LIBDXFRW_BUILD_DOC FALSE CACHE BOOL "Why are these not options???")

vipra_add_library (
  LIBNAME libdxfrw
  LINK dxfrw
  GIT_URL https://github.com/LibreCAD/libdxfrw.git
  GIT_TAG LC2.2.0
)