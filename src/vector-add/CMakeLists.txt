set(SRCS naive.cpp naive_aligned.cpp tiling.cpp psimd.cpp xsimd.cpp)
append_list_if(OpenMP_CXX_FOUND omp_simd.cpp SRCS)
append_list_if(COMPILER_IS_CLANG clang_simd.cpp SRCS)
if(TARGET Eigen3::Eigen)
  list(APPEND SRCS eigen.cpp)
endif()

set(COMPILE_DEFS TARGETNAME=vector-add)
append_list_if(COMPILER_IS_CLANG COMPILER_IS_CLANG COMPILE_DEFS)


add_library(vector-add OBJECT ${SRCS})

target_link_libraries(vector-add PUBLIC utils)
target_link_libraries_system(vector-add PUBLIC benchmark::benchmark xsimd
                             xtensor psimd)
target_compile_definitions(vector-add PUBLIC ${COMPILE_DEFS})
if(OpenMP_CXX_FOUND)
  target_link_libraries(vector-add PUBLIC OpenMP::OpenMP_CXX)
endif()
if(TARGET Eigen3::Eigen)
  target_link_libraries(vector-add PUBLIC Eigen3::Eigen)
endif()
pwropt_target_compile_options(vector-add PUBLIC pwr_werror pwr_lefticus)
