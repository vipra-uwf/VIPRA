

add_subdirectory(_grammar)

target_sources(
  ${VIPRA_PROJECT_NAME}
  PUBLIC
  selectors/pedestrian_groups.cpp
)