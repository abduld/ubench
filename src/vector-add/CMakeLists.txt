add_library(vector-add OBJECT naive.cpp naive_aligned.cpp)

target_link_libraries(vector-add PUBLIC utils)
target_link_libraries_system(vector-add PUBLIC benchmark::benchmark xsimd
                             xtensor)
pwropt_target_compile_options(vector-add PUBLIC pwr_werror pwr_lto pwr_lefticus)
