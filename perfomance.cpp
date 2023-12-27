#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <functional>

using namespace std;
using namespace chrono;

#define randf() ((double) rand()) / ((double) (RAND_MAX))

void logfile(double value) {
    const double data[] = { value };

    // Open a binary file for writing
    ofstream outFile("log.bin", std::ios::binary);

    // Check if the file is open
    if (!outFile.is_open()) {
        std::cerr << "Error opening file for writing." << endl;
        exit(1);
    }

    // Write binary data to the file    
    outFile.write(reinterpret_cast<const char*>(data), sizeof(value));

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

// Function to benchmark a given math function
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

int main() {
    srand(666);
    const int iterations = 100000000; // Adjust the number of iterations as needed
    long long duration;

    // Benchmark baseline function
    duration = benchmarkFunction(iterations, baselineOperation);
    cout << "Benchmark (addition): " << duration << " ms" << endl;

    // Benchmark multiplication function
    duration = benchmarkFunction(iterations, multiplication);
    cout << "Benchmark (multiplication): " << duration << " ms" << endl;

    // Benchmark atan function
    duration = benchmarkFunction(iterations, atan);
    cout << "Benchmark (atan): " << duration << " ms" << endl;

    // Benchmark acos function
    duration = benchmarkFunction(iterations, acos);
    cout << "Benchmark (acos): " << duration << " ms" << endl;

    // Benchmark composite atan function
    duration = benchmarkFunction(iterations, compositeAtan);
    cout << "Benchmark (4*atan): " << duration << " ms" << endl;

    return 0;
}
