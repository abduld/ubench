add_subdirectory(utils)
add_subdirectory(memcpy)
add_subdirectory(vector-add)
add_subdirectory(gemm)

add_executable(ubench "main.cpp")
target_link_libraries(ubench PRIVATE vector-add gemm memcpy benchmark::benchmark)
