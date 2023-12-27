#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <functional>
#include <mpi.h>

using namespace std;
using namespace chrono;

#define randf() ((double) rand()) / ((double) (RAND_MAX))

void logfile(double value) {
    const double data[] = { value };

    // Open a binary file for writing
    ofstream outFile("log.bin", std::ios::binary);

    if (!outFile.is_open()) {
        std::cerr << "Error opening file for writing." << endl;
        exit(1);
    }

    // Convert data to ascii binary
    outFile.write(reinterpret_cast<const char*>(data), sizeof(data));

    // Close the file
    outFile.close();
}

// Baseline operation function (addition)
inline double baselineOperation(double x) {
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
long long benchmarkFunction(int iterations, double (*mathFunction)(double)) {
    double result = 0.0;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        result = mathFunction(randf());
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();

    // Use the result variable outside the loop to prevent optimization
    logfile(result);

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

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(666 + rank);

    duration_p[0] = benchmarkFunction(iterations / size, baselineOperation);
    duration_p[1] = benchmarkFunction(iterations / size, multiplication);
    duration_p[2] = benchmarkFunction(iterations / size, atan);
    duration_p[3] = benchmarkFunction(iterations / size, acos);
    duration_p[4] = benchmarkFunction(iterations / size, compositeAtan);

    for (int j = 0; j < _SIZE; ++j) {
        MPI_Reduce(&duration_p[j], &duration_max[j], 1, MPI_LONG_LONG, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&duration_p[j], &duration_min[j], 1, MPI_LONG_LONG, MPI_MIN, 0, MPI_COMM_WORLD);
        MPI_Reduce(&duration_p[j], &duration_sum[j], 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        cout << "Benchmark (addition):       " << duration_sum[0] / size << " ms" << endl;
        cout << "Benchmark (multiplication): " << duration_sum[1] / size << " ms" << endl;
        cout << "Benchmark (atan):           " << duration_sum[2] / size << " ms" << endl;
        cout << "Benchmark (acos):           " << duration_sum[3] / size << " ms" << endl;
        cout << "Benchmark (4*atan):         " << duration_sum[4] / size << " ms" << endl;
    }

    MPI_Finalize();
    return 0;
}
