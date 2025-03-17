
function(badl_add_all)
  file(GLOB_RECURSE TEMP_SOURCES *.cpp)

  target_sources(
    ${BADL_LIBRARY_NAME}
    PRIVATE
    ${TEMP_SOURCES}
  )
endfunction()