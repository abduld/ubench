find_package(Git)
if(NOT GIT_FOUND)
    message(FATAL_ERROR "git not found!")
endif()


set(ONNXRUNTIME_GIT_URL https://github.com/microsoft/onnxruntime.git)
set(ONNXRUNTIME_GIT_TAG v1.11.1)

execute_process(
  COMMAND             ${GIT_EXECUTABLE} clone ${ONNXRUNTIME_GIT_URL} --branch ${ONNXRUNTIME_GIT_TAG} --depth 1
  WORKING_DIRECTORY   ${CMAKE_BINARY_DIR})

function(onnxruntime_add_static_library target_name)
  add_library(${target_name} STATIC ${ARGN})
endfunction()

set(ONNXRUNTIME_ROOT ${CMAKE_BINARY_DIR}/onnxruntime/onnxruntime)
include(${ONNXRUNTIME_ROOT}/../cmake/onnxruntime_mlas.cmake)

add_compile_definitions(BUILD_MLAS_NO_ONNXRUNTIME)
include_directories(SYSTEM ${ONNXRUNTIME_ROOT}/core/mlas/inc)
