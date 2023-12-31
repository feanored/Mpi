#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <functional>
#include <mpi.h>

using namespace std::chrono;

#define randf() ((double) rand()) / ((double) (RAND_MAX))

const short MASTER = 0;

struct Log {
    int f;
    double x;
    double y;
};

void static logfile(double x, double y, int f) {
    Log value{ f, x, y };

    std::ofstream outFile("log.bin", std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file for writing." << std::endl;
        exit(1);
    }

    outFile.write( (char *) &value, sizeof(Log));

    outFile.close();
}

// Baseline operation function (addition)
inline double addition(double x) {
    return x + x;
}

// Multiplication
inline double multiplication(double x) {
    return x * x;
}

// Composite operation (for Pi)
inline double compositeAtan(double x) {
    return 4 * atan(x);
}

// Function to benchmark given a math function
long long benchmarkFunction(int iterations, double (*mathFunction)(double), int f) {
    double result = 0.0, x = 0.0;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        x = randf();
        result = mathFunction(x);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();

    // Use the result variable outside the loop to prevent optimization
    logfile(x, result, f);

    return duration;
}

int main(int argc, char* argv[]) {
    int size, rank;
    const int _SIZE = 5;
    const int iterations = 250000000;
    long long 
        duration_p[_SIZE]{}, 
        duration_max[_SIZE]{}, 
        duration_min[_SIZE]{}, 
        duration_sum[_SIZE]{};
    steady_clock::time_point start;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(666 + rank);

    if (rank == MASTER) {
        std::cout << "Running.." << std::endl;
        start = high_resolution_clock::now();
    }

    duration_p[0] = benchmarkFunction(iterations / size, addition, 1);
    duration_p[1] = benchmarkFunction(iterations / size, multiplication, 2);
    duration_p[2] = benchmarkFunction(iterations / size, atan, 3);
    duration_p[3] = benchmarkFunction(iterations / size, acos, 4);
    duration_p[4] = benchmarkFunction(iterations / size, compositeAtan, 5);

    for (int j = 0; j < _SIZE; ++j) {
        MPI_Reduce(&duration_p[j], &duration_max[j], 1, MPI_LONG_LONG, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&duration_p[j], &duration_min[j], 1, MPI_LONG_LONG, MPI_MIN, 0, MPI_COMM_WORLD);
        MPI_Reduce(&duration_p[j], &duration_sum[j], 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    if (rank == MASTER) {
        std::cout << "Benchmark (addition):       " << duration_max[0] / 1000. << " s" << std::endl;
        std::cout << "Benchmark (multiplication): " << duration_max[1] / 1000. << " s" << std::endl;
        std::cout << "Benchmark (atan):           " << duration_max[2] / 1000. << " s" << std::endl;
        std::cout << "Benchmark (acos):           " << duration_max[3] / 1000. << " s" << std::endl;
        std::cout << "Benchmark (4*atan):         " << duration_max[4] / 1000. << " s" << std::endl;

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();
        std::cout << "Time elapsed on Master process: " << duration / 1000. << " s" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
