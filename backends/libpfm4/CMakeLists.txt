add_library(LibPFM4Backend SHARED
  LibPFM4.cpp
)
target_link_libraries(LibPFM4Backend -lpfm)

list(APPEND BACKENDS LibPFM4Backend)
set(BACKENDS "${BACKENDS}" PARENT_SCOPE)
