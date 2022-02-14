
#include "perfetto_hpc.h"

#include <perfetto.h>

#include <fstream>

namespace perfetto_hpc
{

static std::unique_ptr<perfetto::TracingSession> tracing_session;

void initialize()
{
  perfetto::TracingInitArgs args;
  args.backends = perfetto::kInProcessBackend;
  perfetto::Tracing::Initialize(args);

  perfetto::TraceConfig cfg;
  cfg.add_buffers()->set_size_kb(1024); // Record up to 1 MiB.
  auto* ds_cfg = cfg.add_data_sources()->mutable_config();
  ds_cfg->set_name("track_event");
  // ds_cfg->set_track_event_config_raw(track_event_cfg.SerializeAsString());

  tracing_session = perfetto::Tracing::NewTrace();
  tracing_session->Setup(cfg);
  tracing_session->StartBlocking();
}

void finalize()
{
  tracing_session->StopBlocking();
  std::vector<char> trace_data(tracing_session->ReadTraceBlocking());

  std::ofstream output;
  output.open("atrace-" + std::to_string(0) + ".perfetto",
              std::ios::out | std::ios::binary);
  output.write(trace_data.data(), trace_data.size());
  output.close();
}

} // namespace perfetto_hpc
