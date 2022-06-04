cmake_minimum_required(VERSION 3.14)

if(POLICY CMP0068)
  cmake_policy(SET CMP0068 NEW)
  set(CMAKE_BUILD_WITH_INSTALL_NAME_DIR ON)
endif()

if(POLICY CMP0075)
  cmake_policy(SET CMP0075 NEW)
endif()

if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

# CMP0116: Ninja generators transform `DEPFILE`s from `add_custom_command()` New
# in CMake 3.20. https://cmake.org/cmake/help/latest/policy/CMP0116.html
if(POLICY CMP0116)
  cmake_policy(SET CMP0116 OLD)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Generate a CompilationDatabase (compile_commands.json file) for our build, for
# use by clang_complete, YouCompleteMe, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

project(ubench LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(AddPwropt)
# include(AddPerfect)
include(AddGoogleBenchmark)
include(AddXNNPACK)
# include(AddEigen)
include(AddXTL)
include(AddXSimd)
include(AddXTensor)

find_package(OpenMP REQUIRED)

if(CMAKE_C_COMPILER MATCHES ".*(clang|emcc).*" OR CMAKE_C_COMPILER_ID MATCHES
                                                  ".*(Clang|emcc).*")
  set(COMPILER_IS_CLANG TRUE)
endif()

if(CMAKE_COMPILER_IS_GNUCC
   OR CMAKE_COMPILER_IS_GNUCXX
   OR CMAKE_C_COMPILER MATCHES ".*(gcc|clang|emcc).*"
   OR CMAKE_C_COMPILER_ID MATCHES ".*(GCC|Clang|emcc).*")
  set(IS_GCC_LIKE TRUE)
else()
  set(IS_GCC_LIKE FALSE)
endif()

if(IS_GCC_LIKE AND NOT COMPILER_IS_CLANG)
  set(COMPILER_IS_GCC TRUE)
endif()

if(COMPILER_IS_CLANG)
  add_definitions(-DCOMPILER_IS_CLANG=1)
  add_definitions(-Rpass=loop-vectorize)
  add_definitions(-ferror-limit=1)
endif(COMPILER_IS_CLANG)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -march=native")
  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} -Wall -fno-omit-frame-pointer -O0")
endif()

# from https://stackoverflow.com/a/61933129
function(target_link_libraries_system target scope)
  set(libs ${ARGN})
  foreach(lib ${libs})
    get_target_property(lib_include_dirs ${lib} INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(${target} SYSTEM ${scope} ${lib_include_dirs})
    target_link_libraries(${target} ${scope} ${lib})
  endforeach(lib)
endfunction(target_link_libraries_system)

macro(append_list_if condition value)
  if(${condition})
    foreach(list ${ARGN})
      list(APPEND ${list} ${value})
    endforeach()
  endif()
endmacro()

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)
