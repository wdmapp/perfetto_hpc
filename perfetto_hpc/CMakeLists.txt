
add_library(perfetto_hpc STATIC)
target_sources(perfetto_hpc PRIVATE perfetto_hpc.cxx track_event.cxx)
target_include_directories(perfetto_hpc PUBLIC include)
target_link_libraries(perfetto_hpc PUBLIC perfetto::perfetto)

add_wrapped_file(pmpi_wrap.cxx pmpi_wrap.tpl.cxx "-f")
target_sources(perfetto_hpc PRIVATE pmpi_wrap.cxx pmpi.cxx)
target_include_directories(perfetto_hpc PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(perfetto_hpc PUBLIC MPI::MPI_CXX MPI::MPI_C MPI::MPI_Fortran)

if (PERFETTO_HPC_ROCTRACER)
    target_sources(perfetto_hpc PRIVATE hip_api.cxx)
    target_link_libraries(perfetto_hpc PRIVATE roc::roctracer)
    target_compile_definitions(perfetto-hpc PRIVATE PERFETTO_HPC_ROCTRACER)
endif()

set(target "perfetto_hpc")

# foreach(property LINK_LIBRARIES;INTERFACE_LINK_LIBRARIES;INTERFACE_INCLUDE_DIRECTORIES)
#     get_target_property(value ${target} ${property})
#     message("${target} ${property} = ${value}")
# endforeach()

add_subdirectory(tests)
