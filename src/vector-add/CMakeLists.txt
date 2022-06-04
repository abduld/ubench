add_library(vector-add OBJECT naive.cpp)

target_link_libraries(vector-add PUBLIC benchmark::benchmark)
