#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>
#include <chrono>

constexpr int MASTER = 0;

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == MASTER)
        std::cout << "Calculating..." << std::endl;

    const int total_points = 1000000000;
    int points_inside_circle = 0;

    srand(time(NULL) + rank);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < total_points / size; ++i) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            points_inside_circle++;
        }
    }

    int total_inside_circle;
    MPI_Reduce(&points_inside_circle, &total_inside_circle, 1, MPI_INT, MPI_SUM, MASTER, MPI_COMM_WORLD);

    auto end_time = std::chrono::high_resolution_clock::now();

    if (rank == MASTER) {
        double pi_approximation = 4.0 * total_inside_circle / total_points;
        std::cout << "Approximation of pi: " << pi_approximation << std::endl;

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "Time taken: " << duration.count() / 1e6 << " seconds" << std::endl;
    }

    MPI_Finalize();

    return 0;
}
