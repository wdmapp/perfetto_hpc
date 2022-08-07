// clang-format off

#include "perfetto_hpc_categories.h"

#include <iostream>

// Initialize MPI
{{fn name MPI_Init}}
  {
    TRACE_EVENT("mpi", "{{name}}");
    {{callfn}}
  }
{{endfn}}

{{fn name MPI_Init_thread}}
  {
    TRACE_EVENT("mpi", "{{name}}");
    {{callfn}}
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
