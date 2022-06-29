set(SRCS clblas.cpp)

add_library(gemm OBJECT ${SRCS})


message(STATUS "XXXX BLAS Libraries ${BLAS_LIBRARIES} -- ${BLAS_PACKAGE_DIR}")
target_link_libraries(gemm PUBLIC utils ${BLAS_LIBRARIES})
target_link_libraries_system(gemm PUBLIC benchmark::benchmark)
pwropt_target_compile_options(gemm PUBLIC pwr_werror pwr_lefticus)
