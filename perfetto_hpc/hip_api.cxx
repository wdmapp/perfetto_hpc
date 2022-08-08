
#include "perfetto_hpc_categories.h"
#include <roctracer_hip.h>
#include <roctracer_hcc.h>

namespace perfetto_hpc
{
namespace hip_api
{

#define ROCTRACER_CALL(call)                                                   \
  do {                                                                         \
    int err = call;                                                            \
    if (err != ROCTRACER_STATUS_SUCCESS) {                                     \
      std::cerr << __FILE__ << ":" << __LINE__ << roctracer_error_string()     \
                << std::endl                                                   \
                << std::flush;                                                 \
      abort();                                                                 \
    }                                                                          \
  } while (0)

// HIP API callback function
void hip_api_callback(uint32_t domain, uint32_t cid, const void* callback_data,
                      void* arg)
{
  const hip_api_data_t* data =
    reinterpret_cast<const hip_api_data_t*>(callback_data);

#if 0
  fprintf(stdout, "<%s id(%u)\tcorrelation_id(%lu) %s> ",
          roctracer_op_string(ACTIVITY_DOMAIN_HIP_API, cid, 0), cid,
          data->correlation_id,
          (data->phase == ACTIVITY_API_PHASE_ENTER) ? "on-enter" : "on-exit");
#endif
  if (data->phase == ACTIVITY_API_PHASE_ENTER) {
#if 0
    switch (cid) {
      case HIP_API_ID_hipMemcpy:
        fprintf(stdout, "dst(%p) src(%p) size(0x%x) kind(%u)",
                data->args.hipMemcpy.dst, data->args.hipMemcpy.src,
                (uint32_t)(data->args.hipMemcpy.sizeBytes),
                (uint32_t)(data->args.hipMemcpy.kind));
        break;
      case HIP_API_ID_hipMalloc:
        fprintf(stdout, "ptr(%p) size(0x%x)", data->args.hipMalloc.ptr,
                (uint32_t)(data->args.hipMalloc.size));
        break;
      case HIP_API_ID_hipFree:
        fprintf(stdout, "ptr(%p)", data->args.hipFree.ptr);
        break;
      case HIP_API_ID_hipModuleLaunchKernel:
        fprintf(stdout, "kernel(\"%s\") stream(%p)",
                hipKernelNameRef(data->args.hipModuleLaunchKernel.f),
                data->args.hipModuleLaunchKernel.stream);
        break;
      default: break;
    }
#endif
    TRACE_EVENT_BEGIN("hip_api", perfetto::StaticString(roctracer_op_string(
                                   ACTIVITY_DOMAIN_HIP_API, cid, 0)));
  } else {
#if 0
    switch (cid) {
      case HIP_API_ID_hipMalloc:
        fprintf(stdout, "*ptr(0x%p)", *(data->args.hipMalloc.ptr));
        break;
      default: break;
    }
#endif
    TRACE_EVENT_END("hip_api");
  }
#if 0
  fprintf(stdout, "\n");
  fflush(stdout);
#endif
}

void start_tracing()
{
  ROCTRACER_CALL(roctracer_set_properties(ACTIVITY_DOMAIN_HIP_API, NULL));
  ROCTRACER_CALL(roctracer_enable_domain_callback(ACTIVITY_DOMAIN_HIP_API,
                                                  hip_api_callback, NULL));
}

void stop_tracing()
{
  ROCTRACER_CALL(roctracer_disable_domain_callback(ACTIVITY_DOMAIN_HIP_API));
}

class Observer : public perfetto::TrackEventSessionObserver
{
public:
  Observer() { perfetto::TrackEvent::AddSessionObserver(this); }
  ~Observer() { perfetto::TrackEvent::RemoveSessionObserver(this); }

  void OnSetup(const perfetto::DataSourceBase::SetupArgs&) override
  {
    // Called when tracing session is configured. Note tracing isn't active yet,
    // so track events emitted here won't be recorded.
    PERFETTO_DLOG("on setup");
  }

  void OnStart(const perfetto::DataSourceBase::StartArgs&) override
  {
    // Called when a tracing session is started. It is possible to emit track
    // events from this callback.
    PERFETTO_DLOG("on start");
    start_tracing();
  }

  void OnStop(const perfetto::DataSourceBase::StopArgs&) override
  {
    // Called when a tracing session is stopped. It is still possible to emit
    // track events from this callback.
    PERFETTO_DLOG("on stop");
    stop_tracing();
  }
};

static std::unique_ptr<perfetto_hpc::hip_api::Observer> observer;

void start_observer()
{
  PERFETTO_DLOG("start observer");
  observer.reset(new Observer());
}

void stop_observer()
{
  PERFETTO_DLOG("stop observer");
  observer.reset();
}

} // namespace hip_api
} // namespace perfetto_hpc
