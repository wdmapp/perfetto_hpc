
#pragma once

#include <string>

namespace perfetto_hpc
{

void initialize();
void finalize();
void start_tracing();
void stop_tracing();
void trace_begin(const char* str);
void trace_end();

extern int mpi_rank;

} // namespace perfetto_hpc
