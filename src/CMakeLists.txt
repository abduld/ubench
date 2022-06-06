add_subdirectory(utils)
add_subdirectory(memcpy)
add_subdirectory(vector-add)

add_executable(ubench "main.cpp")
target_link_libraries(ubench PRIVATE vector-add memcpy benchmark::benchmark)
