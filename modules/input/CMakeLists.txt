file(GLOB
  input_srcs
  *.cpp
  *.hpp
  */*.cpp
  */*.hpp
)

target_sources(
  ${VIPRA_PROJECT_NAME}
  PRIVATE
  ${input_srcs}
)