
program main
    use mpi
    implicit none

    integer :: ierr

    call MPI_Init(ierr)
    call MPI_Finalize(ierr)
end program
