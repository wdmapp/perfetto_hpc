
add_library(perfetto_hpc STATIC)
target_sources(perfetto_hpc PRIVATE perfetto_hpc.cxx)
target_include_directories(perfetto_hpc PUBLIC include)
target_link_libraries(perfetto_hpc PUBLIC perfetto::perfetto)

add_wrapped_file(perfetto_pmpi.cxx perfetto_pmpi.tpl.cxx)
target_sources(perfetto_hpc PRIVATE perfetto_pmpi.cxx)
target_link_libraries(perfetto_hpc PUBLIC MPI::MPI_CXX)

add_subdirectory(tests)
