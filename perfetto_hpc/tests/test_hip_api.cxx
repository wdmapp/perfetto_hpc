
#include "perfetto_hpc_categories.h"

/*
Copyright (c) 2015-2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <iostream>

// hip header file
#include <hip/hip_runtime.h>

#ifndef ITERATIONS
#define ITERATIONS 2
#endif
#define WIDTH 1024

#define NUM (WIDTH * WIDTH)

#define THREADS_PER_BLOCK_X 4
#define THREADS_PER_BLOCK_Y 4
#define THREADS_PER_BLOCK_Z 1

// Device (Kernel) function, it must be void
// hipLaunchParm provides the execution configuration
__global__ void matrixTranspose(/*hipLaunchParm lp, */ float* out, float* in,
                                const int width)
{
  int x = hipBlockDim_x * hipBlockIdx_x + hipThreadIdx_x;
  int y = hipBlockDim_y * hipBlockIdx_y + hipThreadIdx_y;

  out[y * width + x] = in[x * width + y];
}

// CPU implementation of matrix transpose
void matrixTransposeCPUReference(float* output, float* input,
                                 const unsigned int width)
{
  for (unsigned int j = 0; j < width; j++) {
    for (unsigned int i = 0; i < width; i++) {
      output[i * width + j] = input[j * width + i];
    }
  }
}

int iterations = ITERATIONS;
void start_tracing();
void stop_tracing();

int main()
{
  float* Matrix;
  float* TransposeMatrix;
  float* cpuTransposeMatrix;

  float* gpuMatrix;
  float* gpuTransposeMatrix;

  hipDeviceProp_t devProp;
  hipGetDeviceProperties(&devProp, 0);

  std::cout << "Device name " << devProp.name << std::endl;

  int i;
  int errors;

  while (iterations-- > 0) {
    TRACE_EVENT("app", "iteration");

    Matrix = (float*)malloc(NUM * sizeof(float));
    TransposeMatrix = (float*)malloc(NUM * sizeof(float));
    cpuTransposeMatrix = (float*)malloc(NUM * sizeof(float));

    // initialize the input data
    for (i = 0; i < NUM; i++) {
      Matrix[i] = (float)i * 10.0f;
    }

    // allocate the memory on the device side
    hipMalloc((void**)&gpuMatrix, NUM * sizeof(float));
    hipMalloc((void**)&gpuTransposeMatrix, NUM * sizeof(float));

    // Memory transfer from host to device
    hipMemcpy(gpuMatrix, Matrix, NUM * sizeof(float), hipMemcpyHostToDevice);

    // Lauching kernel from host
    hipLaunchKernelGGL(
      matrixTranspose,
      dim3(WIDTH / THREADS_PER_BLOCK_X, WIDTH / THREADS_PER_BLOCK_Y),
      dim3(THREADS_PER_BLOCK_X, THREADS_PER_BLOCK_Y), 0, 0, gpuTransposeMatrix,
      gpuMatrix, WIDTH);

    // Memory transfer from device to host
    hipMemcpy(TransposeMatrix, gpuTransposeMatrix, NUM * sizeof(float),
              hipMemcpyDeviceToHost);

    // CPU MatrixTranspose computation
    matrixTransposeCPUReference(cpuTransposeMatrix, Matrix, WIDTH);

    // verify the results
    errors = 0;
    double eps = 1.0E-6;
    for (i = 0; i < NUM; i++) {
      if (std::abs(TransposeMatrix[i] - cpuTransposeMatrix[i]) > eps) {
        errors++;
      }
    }
    if (errors != 0) {
      printf("FAILED: %d errors\n", errors);
    } else {
      printf("PASSED!\n");
    }

    // free the resources on device side
    hipFree(gpuMatrix);
    hipFree(gpuTransposeMatrix);

    // free the resources on host side
    free(Matrix);
    free(TransposeMatrix);
    free(cpuTransposeMatrix);
  }

  return errors;
}

/////////////////////////////////////////////////////////////////////////////
// HIP/HCC Callbacks/Activity tracing
/////////////////////////////////////////////////////////////////////////////
#include <roctracer_hip.h>
#include <roctracer_hcc.h>

// Macro to check ROC-tracer calls status
#define ROCTRACER_CALL(call)                                                   \
  do {                                                                         \
    int err = call;                                                            \
    if (err != 0) {                                                            \
      std::cerr << roctracer_error_string() << std::endl << std::flush;        \
      abort();                                                                 \
    }                                                                          \
  } while (0)

// Activity tracing callback
//   hipMalloc id(3) correlation_id(1):
//   begin_ns(1525888652762640464) end_ns(1525888652762877067)
void activity_callback(const char* begin, const char* end, void* arg)
{
  const roctracer_record_t* record =
    reinterpret_cast<const roctracer_record_t*>(begin);
  const roctracer_record_t* end_record =
    reinterpret_cast<const roctracer_record_t*>(end);
  fprintf(stdout, "\tActivity records:\n");
  fflush(stdout);
  while (record < end_record) {
    const char* name = roctracer_op_string(record->domain, record->op, 0);
    fprintf(stdout, "\t%s\tcorrelation_id(%lu) time_ns(%lu:%lu) \
                      device_id(%d) queue_id(%lu)",
            name, record->correlation_id, record->begin_ns, record->end_ns,
            record->device_id, record->queue_id);
    // if (record->kind == hc::HSA_OP_ID_COPY)
    //   fprintf(stdout, " bytes(0x%zx)", record->bytes);
    fprintf(stdout, "\n");
    fflush(stdout);
    ROCTRACER_CALL(roctracer_next_record(record, &record));
  }
}

// Start tracing routine
void start_tracing()
{
  std::cout << "# START #############################" << std::endl
            << std::flush;
  // Allocating tracing pool
  roctracer_properties_t properties{};
  properties.buffer_size = 0x1000;
  properties.buffer_callback_fun = activity_callback;
  ROCTRACER_CALL(roctracer_open_pool(&properties));
  ROCTRACER_CALL(roctracer_enable_domain_activity(ACTIVITY_DOMAIN_HIP_API));
}

// Stop tracing routine
void stop_tracing()
{
  ROCTRACER_CALL(roctracer_disable_domain_activity(ACTIVITY_DOMAIN_HIP_API));
  ROCTRACER_CALL(roctracer_close_pool());
  std::cout << "# STOP  #############################" << std::endl
            << std::flush;
}
/////////////////////////////////////////////////////////////////////////////
