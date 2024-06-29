cmake_minimum_required(VERSION 3.20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)

project(MPerf LANGUAGES CXX)
find_package(Kokkos REQUIRED)

add_subdirectory(core)
target_include_directories(MPerf SYSTEM
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Host Architecture
set(MPERF_HOST_ARCH "" CACHE STRING "Host device architecture")
string(TOUPPER "${MPERF_HOST_ARCH}" MPERF_HOST_ARCH)
string(REGEX MATCH "INTELSKL|INTELSKX|INTELKNL" MPERF_HOST_ARCH "${MPERF_HOST_ARCH}")
if ("${MPERF_HOST_ARCH}" STREQUAL "")
  message(FATAL_ERROR "MPERF_HOST_ARCH not defined or invalid")
endif()
add_compile_definitions(MPERF_HOST_ARCH_${MPERF_HOST_ARCH})

set(BACKENDS)

# MPerf backends
add_subdirectory(backends/cppchrono)

target_link_libraries(MPerf ${BACKENDS})
foreach(BACKEND IN LISTS BACKENDS)
  target_include_directories(${BACKEND} SYSTEM 
    PUBLIC
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  )
endforeach()

message("MPerf host architecture: ${MPERF_HOST_ARCH}")
message("MPerf enabled backends: ${BACKENDS}")

# Install lib and headers
install(
  TARGETS MPerf ${BACKENDS}
  EXPORT MPerfConfig
)
install(
  DIRECTORY include/
  DESTINATION include
)

# Export
install(
  EXPORT MPerfConfig
  FILE MPerfConfig.cmake
  NAMESPACE MPerf::
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/MPerf
)
