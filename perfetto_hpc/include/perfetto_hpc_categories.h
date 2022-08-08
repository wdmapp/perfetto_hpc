
#pragma once

#include <perfetto.h>

PERFETTO_DEFINE_CATEGORIES(
  perfetto::Category("perfetto_hpc").SetDescription("Events from perfetto_hpc"),
  perfetto::Category("hip_api").SetDescription("HIP API Events (AMD GPUs)"),
  perfetto::Category("mpi").SetDescription("MPI Events"),
  perfetto::Category("app").SetDescription("Events from main application"));
