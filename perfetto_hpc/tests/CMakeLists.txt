
add_executable(test_simple test_simple.cxx)
target_link_libraries(test_simple PRIVATE perfetto_hpc)

add_executable(test_mpi test_mpi.cxx)
target_link_libraries(test_mpi PRIVATE perfetto_hpc MPI::MPI_CXX)

add_executable(test_mpi_fortran test_mpi_fortran.F90)
target_link_libraries(test_mpi_fortran PRIVATE perfetto_hpc MPI::MPI_Fortran)
