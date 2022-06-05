#include "args.h"
#include "utils/benchmark.h"

#include <benchmark/benchmark.h>
#include <psimd.h>
#include <vector>

#include "stdlib.h"

//  return create_binary_elementwise_nd_f32(
//     output_min,
//     output_max,
//     flags,
//     xnn_operator_type_add_nd_f32,
//     &xnn_params.f32.vadd,
//     add_op_out);
