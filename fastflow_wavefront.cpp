// fastflow_wavefront.cpp
#include <iostream>
#include <cstdlib>
#include <vector>
#include <ff/parallel_for.hpp>  // FastFlow parallel_for
using namespace std;

int main(int argc, char* argv[]){
    // Read matrix size from the command line (default 1000)
    int n = (argc > 1) ? atoi(argv[1]) : 1000;
    // Allocate an n x n matrix initialized to 0.0
    vector<vector<double>> M(n, vector<double>(n, 0.0));

    // FastFlow parallel_for instance
    ff::ParallelFor pf;

    // Process each diagonal (offset k=0,1,...,n-1)
    for (int k = 0; k < n; k++){
        int num_elements = n - k;  // there are num_elements elements in the k–th diagonal
        pf.parallel_for(0, num_elements, 1, [&, k](long m) {
            if(k == 0) {
                // Initialize the main diagonal: M[m][m] = (m+1)/n
                M[m][m] = double(m+1) / n;
            } else {
                // Compute M[m][m+k] as the dot product of
                //    the first k elements of row m and
                //    the first k elements of column (m+k)
                double dot = 0.0;
                for (int r = 0; r < k; r++){
                    dot += M[m][r] * M[r][m+k];
                }
                M[m][m+k] = dot;
            }
        });
    }

    // For verification print one value.
    cout << "FastFlow wavefront computation finished." << endl;
    cout << "M[0][" << n-1 << "] = " << M[0][n-1] << endl;
    return 0;
}
