
#include "perfetto_hpc_categories.h"

PERFETTO_TRACK_EVENT_STATIC_STORAGE();

namespace perfetto_hpc
{
namespace track_event
{

void Register()
{
  perfetto::TrackEvent::Register();
}

void Flush()
{
  perfetto::TrackEvent::Flush();
}

void SetProcessName(const std::string& name)
{
  perfetto::ProcessTrack process_track = perfetto::ProcessTrack::Current();
  perfetto::protos::gen::TrackDescriptor desc = process_track.Serialize();
  desc.mutable_process()->set_process_name(name);
  perfetto::TrackEvent::SetTrackDescriptor(process_track, desc);
}

} // namespace track_event
} // namespace perfetto_hpc
