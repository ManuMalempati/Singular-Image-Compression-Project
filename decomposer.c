#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "svdcmp.h"

#define NR_END 1
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))

double** transpose(double **matrix, int rows, int columns);

int decomposer(double **a, int m, int n) {
    // Example matrix A
    int m = 2, n = 3;
    double **a = dmatrix(1, m, 1, n);
    a[1][1] = 1.0; a[1][2] = 0.0; a[1][3] = 1.0;
    a[2][1] = 0.0; a[2][2] = 1.0; a[2][3] = 0.0;

    // Allocate storage for W and V
    double *w = dvector(1, n);
    double **v = dmatrix(1, n, 1, n);

    // Perform SVD
    svdcmp(a, m, n, w, v);

    // Determine rank (non-zero singular values)
    int rank = 0;
    for (int i = 1; i <= n; i++) {
        if (fabs(w[i]) > 1e-10) {
            rank++;
        }
    }

    // Allocate space for truncated U, W, V and V_T
    double **U_reduced = dmatrix(1, m, 1, rank);
    double *W_reduced = dvector(1, rank);
    double **V_reduced = dmatrix(1, n, 1, rank);
    double **V_T = dmatrix(1, rank, 1, n);


    // Fill the reduced matrices
    int reduced_col = 1;
    for (int i = 1; i <= n; i++) {
        if (fabs(w[i]) > 1e-10) {
            for (int j = 1; j <= m; j++) {
                U_reduced[j][reduced_col] = a[j][i];
            }
            W_reduced[reduced_col] = w[i];
            for (int j = 1; j <= n; j++) {
                V_reduced[j][reduced_col] = v[j][i];
            }
            reduced_col++;
        }
    }

    // Fill V_T
    V_T = transpose(V_reduced, n, rank);

    // Print the truncated SVD results
    printf("Matrix U (m x rank):\n");
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= rank; j++) {
            printf("%8.4f ", U_reduced[i][j]);
        }
        printf("\n");
    }

    printf("\nSingular values (W):\n");
    for (int i = 1; i <= rank; i++) {
        printf("%8.4f ", W_reduced[i]);
    }
    printf("\n");

    printf("\nMatrix V (n x rank):\n");
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= rank; j++) {
            printf("%8.4f ", V_reduced[i][j]);
        }
        printf("\n");
    }

    printf("\nMatrix V_T (rank x n):\n");
    for (int i = 1; i <= rank; i++) {
        for (int j = 1; j <= n; j++) {
            printf("%8.4f ", V_T[i][j]);
        }
        printf("\n");
    }

    // Free memory
    free_dmatrix(a, 1, m, 1, n);
    free_dvector(w, 1, n);
    free_dmatrix(v, 1, n, 1, n);
    free_dmatrix(U_reduced, 1, m, 1, rank);
    free_dvector(W_reduced, 1, rank);
    free_dmatrix(V_reduced, 1, n, 1, rank);
    free_dmatrix(V_T, 1, rank, 1, n);

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