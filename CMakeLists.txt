cmake_minimum_required(VERSION 3.12)
project(MPICpp CXX)

# Find MPI package
find_package(MPI REQUIRED)

# List of projects
set(project_list "pi")

foreach(project IN LISTS project_list)

    # Create executable
    add_executable(${project} ${project}.cpp)

    # Include MPI headers
    target_include_directories(${project} PRIVATE ${MPI_CXX_INCLUDE_PATH})

    # Link MPI libraries
    target_link_libraries(${project} ${MPI_CXX_LIBRARIES})

    # Set MPI flags
    set_target_properties(${project} PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
    set_target_properties(${project} PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")

endforeach()