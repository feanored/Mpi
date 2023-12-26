program mpi_example
  use mpi
  implicit none

  integer :: rank, size, i, total_points, points_inside_circle, total_inside_circle
  double precision :: x, y, pi_approximation
  integer :: start_time, end_time

  call MPI_Init(MPI_COMM_WORLD, i)
  call MPI_Comm_rank(MPI_COMM_WORLD, rank, i)
  call MPI_Comm_size(MPI_COMM_WORLD, size, i)

  total_points = 1000000
  points_inside_circle = 0

  call SYSTEM_CLOCK(start_time)

  call random_seed()
  call random_number(x)
  call random_number(y)

  do i = 1, total_points / size
    x = rand()
    y = rand()

    if (x**2 + y**2 <= 1.0d0) then
      points_inside_circle = points_inside_circle + 1
    end if
  end do

  call MPI_Reduce(points_inside_circle, total_inside_circle, 1, MPI_INTEGER, MPI_SUM, MASTER, MPI_COMM_WORLD, i)

  call SYSTEM_CLOCK(end_time)

  if (rank == MASTER) then
    pi_approximation = 4.0d0 * total_inside_circle / total_points
    print *, "Approximation of pi: ", pi_approximation
    print *, "Time taken: ", real(end_time - start_time) / CPU_TIME_RATE(), " seconds"
  end if

  call MPI_Finalize(i)

contains

  function rand() result(r)
    real(8) :: r
    call random_number(r)
  end function rand

end program mpi_example