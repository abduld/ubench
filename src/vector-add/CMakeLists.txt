add_library(vector-add OBJECT naive.cpp)

target_link_libraries(vector-add PUBLIC utils benchmark::benchmark)
pwropt_target_compile_options(vector-add PUBLIC pwr_werror pwr_lto pwr_lefticus)
pwropt_target_cxx_latest(vector-add PUBLIC)
