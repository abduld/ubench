add_library(memcpy OBJECT memcpy.cpp)

target_link_libraries(memcpy PUBLIC utils)
target_link_libraries_system(memcpy PUBLIC benchmark::benchmark xsimd xtensor)
pwropt_target_compile_options(memcpy PUBLIC pwr_werror pwr_lefticus)
