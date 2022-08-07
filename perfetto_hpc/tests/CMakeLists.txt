
add_executable(test_simple test_simple.cxx)
target_link_libraries(test_simple PRIVATE perfetto_hpc)

add_executable(test_mpi test_mpi.cxx)
#FIXME, shouldn't have to add MPI_Fortran
target_link_libraries(test_mpi PRIVATE perfetto_hpc MPI::MPI_CXX MPI::MPI_Fortran)

add_executable(test_mpi_fortran test_mpi_fortran.F90)
target_link_libraries(test_mpi_fortran PRIVATE perfetto_hpc MPI::MPI_Fortran)

if (PERFETTO_HPC_ROCTRACER)
    add_executable(test_hip_api test_hip_api.cxx)
    target_link_libraries(test_hip_api PRIVATE perfetto_hpc hip::device roc::roctracer)
endif()
