add_library(utils benchmark.cpp)

target_link_libraries(utils PUBLIC XNNPACK cpuinfo benchmark::benchmark)
