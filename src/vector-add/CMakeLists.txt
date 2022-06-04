set(SRCS naive.cpp naive_aligned.cpp)
append_list_if(OpenMP_CXX_FOUND SRCS omp_simd.cpp)

add_library(vector-add OBJECT ${SRCS})

target_link_libraries(vector-add PUBLIC utils OpenMP::OpenMP_CXX)
target_link_libraries_system(vector-add PUBLIC benchmark::benchmark xsimd
                             xtensor)
pwropt_target_compile_options(vector-add PUBLIC pwr_werror pwr_lto pwr_lefticus)
