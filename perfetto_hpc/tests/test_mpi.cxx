
#include "perfetto_hpc.h"

#include <mpi.h>

#include <unistd.h>

int main(int argc, char** argv)
{
  perfetto_hpc::initialize();
  perfetto_hpc::trace_begin("main");

  MPI_Init(&argc, &argv);
  perfetto_hpc::trace_begin("sleep");
  usleep(1000);
  perfetto_hpc::trace_end();
  MPI_Finalize();

  perfetto_hpc::trace_end();
  perfetto_hpc::finalize();
}
