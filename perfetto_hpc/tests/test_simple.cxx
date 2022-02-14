
#include "perfetto_hpc.h"

int main(int argc, char** argv)
{
  perfetto_hpc::initialize();
  perfetto_hpc::finalize();
}
