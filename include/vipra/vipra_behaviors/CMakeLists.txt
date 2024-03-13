
file(GLOB
  antlr_srcs
  ${CMAKE_CURRENT_SOURCE_DIR}/_grammar/generated/*.cpp
)

target_sources(
  ${VIPRA_PROJECT_NAME}
  PRIVATE
  ${antlr_srcs}
  ${CMAKE_CURRENT_SOURCE_DIR}/selectors/pedestrian_groups.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/attributes/attributes.cpp
)