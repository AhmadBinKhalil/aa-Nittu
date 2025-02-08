// sequential_wavefront.cpp
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
    // Set matrix size n (default is 1000, or specify via command line)
    int n = (argc > 1) ? atoi(argv[1]) : 1000;
    
    // Create an n x n matrix initialized with 0.0
    vector<vector<double>> M(n, vector<double>(n, 0.0));

    // Process each diagonal in order (wavefront style)
    // k = 0 corresponds to the main diagonal; k > 0 for the upper-triangular part.
    for (int k = 0; k < n; k++) {
        int numElements = n - k;  // There are (n-k) elements in the k-th diagonal
        for (int m = 0; m < numElements; m++) {
            if (k == 0) {
                // For the main diagonal, initialize M[m][m] to (m+1)/n
                M[m][m] = double(m + 1) / n;
            } else {
                // For the off-diagonals, compute the element M[m][m+k] as the dot product:
                //   dot = sum_{r=0}^{k-1} M[m][r] * M[r][m+k]
                double dot = 0.0;
                for (int r = 0; r < k; r++) {
                    dot += M[m][r] * M[r][m + k];
                }
                M[m][m + k] = dot;
            }
        }
    }

    // Report that the computation has finished and print one sample output.
    cout << "Sequential wavefront computation finished." << endl;
    cout << "M[0][" << n - 1 << "] = " << M[0][n - 1] << endl;

    return 0;
}
