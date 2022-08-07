
#include "perfetto_hpc.h"
#include "perfetto_hpc_pmpi.h"

#include <mpi.h>

#include <perfetto.h>

namespace perfetto_hpc
{
namespace pmpi
{

void mpi_init_after()
{
  MPI_Comm_rank(MPI_COMM_WORLD, &::perfetto_hpc::mpi_rank);
  PERFETTO_DLOG("mpi_init_after rank=%d", ::perfetto_hpc::mpi_rank);
}

} // namespace pmpi
} // namespace perfetto_hpc
