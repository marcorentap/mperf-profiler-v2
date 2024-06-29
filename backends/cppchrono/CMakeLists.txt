add_library(CPPChronoBackend SHARED
  CPPChrono.cpp
)

list(APPEND BACKENDS CPPChronoBackend)
set(BACKENDS "${BACKENDS}" PARENT_SCOPE)
