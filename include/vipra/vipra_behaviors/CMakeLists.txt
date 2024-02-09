
message(STATUS "Setting Up ANTLR")
LIST( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/antlr_setup )
include( ExternalAntlr4Cpp )
link_directories( ${ANTLR4CPP_LIBS} )
message(STATUS "Done Setting Up ANTLR")

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  antlr4_static
)
