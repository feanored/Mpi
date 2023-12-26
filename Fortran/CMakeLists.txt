cmake_minimum_required(VERSION 3.12)
project(MPIFortran Fortran)
enable_language(Fortran)  

# Set the path to MS-MPI
set(MPI_ROOT "C:/Program Files (x86)/Microsoft SDKs/MPI")

# Find MPI package
find_package(MPI REQUIRED)

# Set include directories
include_directories(${MPI_INCLUDE_PATH})

# Create executable
add_executable(Pi pi.f90)

# Link MPI library
target_link_libraries(Pi ${MPI_Fortran_LIBRARIES})

# Set MPI flags
set_target_properties(Pi PROPERTIES COMPILE_FLAGS "${MPI_Fortran_COMPILE_FLAGS}")
set_target_properties(Pi PROPERTIES LINK_FLAGS "${MPI_Fortran_LINK_FLAGS}")
