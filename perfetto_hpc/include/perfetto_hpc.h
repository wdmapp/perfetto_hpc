
#pragma once

namespace perfetto_hpc
{

void initialize();
void finalize();
void start_tracing();
void stop_tracing();
void trace_begin(const char* str);
void trace_end();

} // namespace perfetto_hpc
