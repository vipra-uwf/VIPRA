vipra_add_all()

vipra_add_library(
  LIBNAME nlohmann_json
  GIT_URL https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

set(LIBDXFRW_BUILD_DWG2DXF FALSE CACHE BOOL "Why are these not options???")
set(LIBDXFRW_BUILD_DOC FALSE CACHE BOOL "Why are these not options???")

vipra_add_library (
  LIBNAME libdxfrw
  LINK dxfrw
  GIT_URL https://github.com/LibreCAD/libdxfrw.git
  GIT_TAG LC2.2.0
)