set(SRCS cblas.cpp ruy.cpp mlas.cpp)

if(TARGET Eigen3::Eigen)
  list(APPEND SRCS eigen.cpp)
endif()

add_library(gemm OBJECT ${SRCS})

if(TARGET Eigen3::Eigen)
  target_link_libraries(gemm PUBLIC Eigen3::Eigen)
endif()

target_link_libraries(gemm PUBLIC utils onnxruntime_mlas ${BLAS_LIBRARIES})
target_link_libraries_system(gemm PUBLIC ruy benchmark::benchmark)
pwropt_target_compile_options(gemm PUBLIC pwr_werror pwr_lefticus)
