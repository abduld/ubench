set(SRCS naive.cpp naive_aligned.cpp tiling.cpp)
append_list_if(OpenMP_CXX_FOUND omp_simd.cpp SRCS)
append_list_if(COMPILER_IS_CLANG clang_simd.cpp SRCS)

add_library(vector-add OBJECT "${SRCS}")

target_link_libraries(vector-add PUBLIC utils)
target_link_libraries_system(vector-add PUBLIC benchmark::benchmark xsimd
                             xtensor)
if(OpenMP_CXX_FOUND)
  target_link_libraries(vector-add PUBLIC OpenMP::OpenMP_CXX)
endif()
pwropt_target_compile_options(vector-add PUBLIC pwr_werror pwr_lto pwr_lefticus)
