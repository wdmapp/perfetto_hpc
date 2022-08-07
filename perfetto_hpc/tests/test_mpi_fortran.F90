
program main
    use mpi
    implicit none

    integer :: ierr
    integer :: rank
    integer :: sum

    call MPI_Init(ierr)
    call MPI_Comm_rank(MPI_COMM_WORLD, rank, ierr)
#if 1
    call MPI_Allreduce(rank, sum, 1, MPI_INTEGER, MPI_SUM, MPI_COMM_WORLD, ierr)
#else
    sum = rank
    call MPI_Allreduce(MPI_IN_PLACE, sum, 1, MPI_INTEGER, MPI_SUM, MPI_COMM_WORLD, ierr)
#endif
    if (rank == 0) print *, 'sum', sum
    call MPI_Finalize(ierr)
end program
