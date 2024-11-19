#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "svdcmp.h"

#define NR_END 1
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))

double** transpose(double **matrix, int rows, int columns);

int decomposer(double **a, int m, int n, double ***U_reduced, double **W_reduced, double ***V_reduced, double ***V_T) {
    printf("Entered function\n");

    // Allocate storage for W and V
    double *w = dvector(1, n);
    double **v = dmatrix(1, n, 1, n);

    printf("Checkpoint 1\n");

    // Perform SVD
    svdcmp(a, m, n, w, v);

    // Determine rank (non-zero singular values)
    int rank = 0;
    for (int i = 1; i <= n; i++) {
        if (fabs(w[i]) > 1e-10) {
            rank++;
        }
    }

    printf("Checkpoint 2\n");

    // Allocate space for truncated U, W, V, and V_T inside the decomposer
    *U_reduced = dmatrix(1, m, 1, rank);
    *W_reduced = dvector(1, rank);
    *V_reduced = dmatrix(1, n, 1, rank);
    *V_T = dmatrix(1, rank, 1, n);

    // Fill the reduced matrices
    int reduced_col = 1;
    for (int i = 1; i <= n; i++) {
        if (fabs(w[i]) > 1e-10) {
            for (int j = 1; j <= m; j++) {
                (*U_reduced)[j][reduced_col] = a[j][i];
            }
            (*W_reduced)[reduced_col] = w[i];
            for (int j = 1; j <= n; j++) {
                (*V_reduced)[j][reduced_col] = v[j][i];
            }
            reduced_col++;
        }
    }

    // Fill V_T
    *V_T = transpose(*V_reduced, n, rank);

    printf("Checkpoint 3\n");

    // Free memory used for temporary matrices
    free_dvector(w, 1, n);
    free_dmatrix(v, 1, n, 1, n);

    printf("Checkpoint 4\n");

    return 0;
}

// Function to find transpose of a matrix (1-xbased indexing)
double** transpose(double **matrix, int rows, int columns) {
    double **matrix_t = (double **)malloc((columns + 1) * sizeof(double *));
    for (int i = 1; i <= columns; i++) {
        matrix_t[i] = (double *)malloc((rows + 1) * sizeof(double));
    }
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= columns; j++) {
            matrix_t[j][i] = matrix[i][j];
        }
    }
    return matrix_t;
}