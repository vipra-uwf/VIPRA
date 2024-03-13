file(GLOB
  fields_srcs
  *.cpp
  *.hpp
  */*.cpp
  */*.hpp
)

target_sources(
  ${VIPRA_PROJECT_NAME}
  PRIVATE
  ${fields_srcs}
)