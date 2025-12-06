
target_sources(
  ${VIPRA_LIBRARY_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/generated/BehaviorVisitor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/generated/BehaviorLexer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/generated/BehaviorParser.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/generated/BehaviorBaseVisitor.cpp
)