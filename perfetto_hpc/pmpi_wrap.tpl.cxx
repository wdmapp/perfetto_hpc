#include "perfetto_hpc_categories.h"
#include "perfetto_hpc_pmpi.h"

#include <iostream>

extern void* mpi_fortran_in_place_;

void* buffer_f2c(void* buf)
{
  if (buf == &mpi_fortran_in_place_) {
    return MPI_IN_PLACE;
  } else {
    return buf;
  }
}

// clang-format off

// Initialize MPI
{{fn name MPI_Init}}
  {
    TRACE_EVENT("mpi", "{{name}}");
    {{callfn}}
    perfetto_hpc::pmpi::mpi_init_after();
  }
{{endfn}}

{{fn name MPI_Init_thread}}
  {
    TRACE_EVENT("mpi", "{{name}}");
    {{callfn}}
    perfetto_hpc::pmpi::mpi_init_after();
  }
{{endfn}}

// Finalize MPI
{{fn name MPI_Finalize}}
  {
    TRACE_EVENT("mpi", "{{name}}");
    {{callfn}}
  }

  perfetto::TrackEvent::Flush();
{{endfn}}

{{fn name MPI_Send MPI_Recv MPI_Allreduce MPI_Reduce MPI_Wait MPI_Waitany
MPI_Waitall MPI_Waitsome MPI_Gather MPI_Gatherv MPI_Scatter MPI_Scatterv
MPI_Allgather MPI_Allgatherv MPI_Alltoall MPI_Alltoallv MPI_Alltoallw
MPI_Bcast MPI_Sendrecv MPI_Barrier MPI_Isend MPI_Irecv}}
  {
    TRACE_EVENT("mpi", "{{name}}");
    {{callfn}}
  }
{{endfn}}
