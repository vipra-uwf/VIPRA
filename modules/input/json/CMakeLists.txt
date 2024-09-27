
vipra_module(
  NAME
    JSON
  SOURCES
    json.cpp
  TYPE
    Input
)

vipra_add_library(
  LIBNAME nlohmann_json
  GIT_URL https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)