
#include "perfetto_hpc.h"

#include <unistd.h>

int main(int argc, char** argv)
{
  perfetto_hpc::initialize();
  perfetto_hpc::trace_begin("main");
  usleep(1000);
  perfetto_hpc::trace_end();
  perfetto_hpc::finalize();
}
