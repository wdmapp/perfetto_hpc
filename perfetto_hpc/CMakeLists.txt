
add_library(perfetto_hpc STATIC)
target_sources(perfetto_hpc PRIVATE perfetto_hpc.cxx)
target_include_directories(perfetto_hpc PUBLIC include)
target_link_libraries(perfetto_hpc PUBLIC perfetto::perfetto)

add_subdirectory(tests)
