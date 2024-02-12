file(GLOB
  goals_srcs
  *.cpp
  *.hpp
  */*.cpp
  */*.hpp
)

target_sources(
  ${PROJECT_NAME}
  PRIVATE
  ${goals_srcs}
)