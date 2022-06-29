
include(ExternalProject)

if (WIN32)
  SET(ONNXRUNTIME_URL "https://github.com/microsoft/onnxruntime/releases/download/v1.11.1/onnxruntime-win-x64-1.11.1.zip")
elseif(CMAKE_APPLE_SILICON_PROCESSOR)
  set(ONNXRUNTIME_URL "https://github.com/microsoft/onnxruntime/releases/download/v1.11.1/onnxruntime-osx-arm64-1.11.1.tgz")
elseif(APPLE)
  SET(ONNXRUNTIME_URL "https://github.com/microsoft/onnxruntime/releases/download/v1.11.1/onnxruntime-osx-x86_64-1.11.1.tgz")
else ()
  SET(ONNXRUNTIME_URL "https://github.com/microsoft/onnxruntime/releases/download/v1.11.1/onnxruntime-linux-x64-1.11.1.tgz")
endif()

SET(ONNXRUNTIME_PROJECT        "onnxruntime_")
SET(ONNXRUNTIME_PREFIX_DIR     ${CMAKE_BINARY_DIR}/onnxruntime)
SET(ONNXRUNTIME_SOURCE_DIR     ${CMAKE_BINARY_DIR}/onnxruntime/src/${ONNXRUNTIME_PROJECT})
SET(ONNXRUNTIME_INSTALL_DIR    ${CMAKE_BINARY_DIR}/install/onnxruntime)
SET(ONNXRUNTIME_INC_DIR        "${ONNXRUNTIME_INSTALL_DIR}/include")
SET(ONNXRUNTIME_LIB_DIR        "${ONNXRUNTIME_INSTALL_DIR}/lib")
SET(CMAKE_BUILD_RPATH "${CMAKE_BUILD_RPATH}" "${ONNXRUNTIME_LIB_DIR}")


INCLUDE_DIRECTORIES(${ONNXRUNTIME_INC_DIR}) # For ONNXRUNTIME code to include internal headers.
if (WIN32)
  SET(ONNXRUNTIME_SOURCE_LIB "${ONNXRUNTIME_SOURCE_DIR}/lib/onnxruntime.dll" CACHE FILEPATH "ONNXRUNTIME source library." FORCE)
  SET(ONNXRUNTIME_SHARED_LIB "${ONNXRUNTIME_INSTALL_DIR}/lib/onnxruntime.dll" CACHE FILEPATH "ONNXRUNTIME shared library." FORCE)
  SET(ONNXRUNTIME_LIB "${ONNXRUNTIME_INSTALL_DIR}/lib/onnxruntime.lib" CACHE FILEPATH "ONNXRUNTIME static library." FORCE)
elseif (APPLE)
  SET(ONNXRUNTIME_SOURCE_LIB "${ONNXRUNTIME_SOURCE_DIR}/lib/libonnxruntime.dylib" CACHE FILEPATH "ONNXRUNTIME source library." FORCE)
  SET(ONNXRUNTIME_LIB "${ONNXRUNTIME_INSTALL_DIR}/lib/libonnxruntim.dylib" CACHE FILEPATH "ONNXRUNTIME static library." FORCE)
  SET(ONNXRUNTIME_SHARED_LIB ${ONNXRUNTIME_LIB} CACHE FILEPATH "ONNXRUNTIME shared library." FORCE)
else ()
  SET(ONNXRUNTIME_SOURCE_LIB "${ONNXRUNTIME_SOURCE_DIR}/lib/libonnxruntime.so" CACHE FILEPATH "ONNXRUNTIME source library." FORCE)
  SET(ONNXRUNTIME_LIB "${ONNXRUNTIME_INSTALL_DIR}/lib/libonnxruntime.so" CACHE FILEPATH "ONNXRUNTIME static library." FORCE)
  SET(ONNXRUNTIME_SHARED_LIB ${ONNXRUNTIME_LIB} CACHE FILEPATH "ONNXRUNTIME shared library." FORCE)
endif ()

ExternalProject_Add(
  ${ONNXRUNTIME_PROJECT}
  URL                 ${ONNXRUNTIME_URL}
  PREFIX              ${ONNXRUNTIME_PREFIX_DIR}
  DOWNLOAD_NO_PROGRESS  1
  CONFIGURE_COMMAND     ""
  BUILD_COMMAND         ""
  UPDATE_COMMAND        ""
  INSTALL_COMMAND       ${CMAKE_COMMAND} -E copy ${ONNXRUNTIME_SOURCE_LIB} ${ONNXRUNTIME_LIB} &&
                        ${CMAKE_COMMAND} -E copy_directory ${ONNXRUNTIME_SOURCE_DIR}/include ${ONNXRUNTIME_INC_DIR}
  BUILD_BYPRODUCTS      ${ONNXRUNTIME_LIB}
)

ADD_LIBRARY(onnxruntime STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET onnxruntime PROPERTY IMPORTED_LOCATION ${ONNXRUNTIME_LIB})
ADD_DEPENDENCIES(onnxruntime ${ONNXRUNTIME_PROJECT})
