file(GLOB
  pedmodel_srcs
  *.cpp
  *.hpp
  */*.cpp
  */*.hpp
)

target_sources(
  ${VIPRA_PROJECT_NAME}
  PRIVATE
  ${pedmodel_srcs}
)