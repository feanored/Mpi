cmake_minimum_required(VERSION 3.12)
project(MPICpp CXX)

# Find MPI package
find_package(MPI REQUIRED)

# Create executable
add_executable(Pi pi.cpp)

# Include MPI headers
target_include_directories(Pi PRIVATE ${MPI_CXX_INCLUDE_PATH})

# Link MPI libraries
target_link_libraries(Pi ${MPI_CXX_LIBRARIES})

# Set MPI flags
set_target_properties(Pi PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
set_target_properties(Pi PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")

# Enable C++11
set_property(TARGET Pi PROPERTY CXX_STANDARD 11)

# Set compilation flags (optional: adjust as needed)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")

# Enable MPI support in CMake (optional: adjust as needed)
include_directories(SYSTEM ${MPI_CXX_INCLUDE_PATH})
