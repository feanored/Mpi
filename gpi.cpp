#include <iostream>
#include <cmath>
#include <mpi.h>

const double PI = 3.141592653589793238462643;
const int MASTER = 0;

int main(int argc, char* argv[]) {
	int done = 0, num_figs, rank, size;
	double parcial_pi, pi, h, sum, x;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	while (!done) {
		if (rank == MASTER) {
			std::cout << "Enter the number of figures: (0 quits) ";
			std::cin >> num_figs;
		}
		
		MPI_Bcast(&num_figs, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
		if (num_figs == 0) break;

		h = 1. / (double)num_figs;
		sum = 0.;
		for (int i = rank + 1; i <= num_figs; i += size) {
			x = h * ((double)i - 0.5);
			sum += 4. / (1. + x * x);
		}
		parcial_pi = h * sum;

		MPI_Reduce(&parcial_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
		
		if (rank == MASTER)
			printf("Pi is aproximately %.16f, Error is %.16f\n",
				pi, fabs(pi - PI));
	}

	MPI_Finalize();
	return 0;
}