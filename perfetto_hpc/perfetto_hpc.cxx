
#include "perfetto_hpc.h"
#include "perfetto_hpc_categories.h"

#include <perfetto.h>

#include <fstream>

// TODO
// * prevent double init / finalize
// * thread safety
// * make things configurable via env
// * keep track of tracing enabled

PERFETTO_TRACK_EVENT_STATIC_STORAGE();

namespace perfetto_hpc
{

int mpi_rank = 0;

static std::unique_ptr<perfetto::TracingSession> tracing_session;

void initialize()
{
  perfetto::TracingInitArgs args;
  args.backends = perfetto::kInProcessBackend;
  perfetto::Tracing::Initialize(args);

  perfetto::TrackEvent::Register();
  perfetto::protos::gen::TrackEventConfig track_event_cfg;
  track_event_cfg.add_disabled_categories("");
  track_event_cfg.add_enabled_categories("*");

  perfetto::TraceConfig cfg;
  cfg.add_buffers()->set_size_kb(1024); // Record up to 1 MiB.
  auto* ds_cfg = cfg.add_data_sources()->mutable_config();
  ds_cfg->set_name("track_event");
  // ds_cfg->set_track_event_config_raw(track_event_cfg.SerializeAsString());

  tracing_session = perfetto::Tracing::NewTrace();
  tracing_session->Setup(cfg);
  PERFETTO_DLOG("initialized");

  start_tracing();
}

void start_tracing()
{
  tracing_session->StartBlocking();

  TRACE_EVENT_BEGIN("perfetto_hpc", "tracing");
}

void stop_tracing()
{
  TRACE_EVENT_END("perfetto_hpc");

  tracing_session->StopBlocking();
}

void finalize()
{
  stop_tracing();

  std::vector<char> trace_data(tracing_session->ReadTraceBlocking());

  PERFETTO_DLOG("writing trace file");
  std::ofstream output;
  output.open("atrace-" + std::to_string(mpi_rank) + ".perfetto",
              std::ios::out | std::ios::binary);
  output.write(trace_data.data(), trace_data.size());
  output.close();
  PERFETTO_DLOG("finalized");
}

void trace_begin(const char* str)
{
  TRACE_EVENT_BEGIN("app", perfetto::DynamicString(str));
}

void trace_end()
{
  TRACE_EVENT_END("app");
}

namespace detail
{

struct InitializerFinalizer
{
  InitializerFinalizer() { initialize(); }
  ~InitializerFinalizer() { finalize(); }
};

static InitializerFinalizer auto_init_final;

}; // namespace detail

} // namespace perfetto_hpc
