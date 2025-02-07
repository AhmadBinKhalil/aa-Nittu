// mpi_wavefront.cpp
#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

int main(int argc, char* argv[]){
    MPI_Init(&argc,&argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    // Read matrix size from the command line (default 1000)
    int n = (argc > 1) ? atoi(argv[1]) : 1000;
    // Allocate an n x n matrix (all processes have a full copy)
    vector<vector<double>> M(n, vector<double>(n, 0.0));

    // Initialize main diagonal: for each m, set M[m][m] = (m+1)/n
    for (int m = 0; m < n; m++){
        M[m][m] = double(m+1) / n;
    }

    // Process off-diagonals k = 1, …, n-1
    for (int k = 1; k < n; k++){
        int num_elements = n - k; // for this diagonal, m=0,...,num_elements-1 corresponds to M[m][m+k]

        // Divide the indices m among the processes (block distribution)
        int base = num_elements / size;
        int rem  = num_elements % size;
        int local_start, local_count;
        if(rank < rem) {
            local_count = base + 1;
            local_start = rank * local_count;
        } else {
            local_count = base;
            local_start = rank * local_count + rem;
        }
        int local_end = local_start + local_count;

        // Each process computes its share into a local vector
        vector<double> local_diag(local_count, 0.0);
        for (int m = local_start; m < local_end; m++){
            double dot = 0.0;
            for (int r = 0; r < k; r++){
                dot += M[m][r] * M[r][m+k];
            }
            local_diag[m - local_start] = dot;
        }

        // Gather the computed diagonal values from all processes.
        // (They form an array of length num_elements.)
        vector<int> recvcounts(size), displs(size);
        for (int p = 0; p < size; p++){
            recvcounts[p] = (p < rem) ? base + 1 : base;
        }
        displs[0] = 0;
        for (int p = 1; p < size; p++){
            displs[p] = displs[p-1] + recvcounts[p-1];
        }
        vector<double> full_diag(num_elements, 0.0);
        MPI_Allgatherv(local_diag.data(), local_count, MPI_DOUBLE,
                       full_diag.data(), recvcounts.data(), displs.data(), MPI_DOUBLE,
                       MPI_COMM_WORLD);
        // Update the matrix M with the gathered diagonal
        for (int m = 0; m < num_elements; m++){
            M[m][m+k] = full_diag[m];
        }
    }

    if(rank == 0){
        cout << "MPI wavefront computation finished." << endl;
        cout << "M[0][" << n-1 << "] = " << M[0][n-1] << endl;
    }
    MPI_Finalize();
    return 0;
}
