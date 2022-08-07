
add_library(perfetto_hpc STATIC)
target_sources(perfetto_hpc PRIVATE perfetto_hpc.cxx)
target_include_directories(perfetto_hpc PUBLIC include)
target_link_libraries(perfetto_hpc PUBLIC perfetto::perfetto)

add_wrapped_file(pmpi_wrap.cxx pmpi_wrap.tpl.cxx "-f")
target_sources(perfetto_hpc PRIVATE pmpi_wrap.cxx)
target_link_libraries(perfetto_hpc PUBLIC MPI::MPI_CXX MPI::MPI_C MPI::MPI_Fortran)

add_subdirectory(tests)
