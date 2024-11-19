#include <stdio.h>
#include "image_utils.h"
#include "svdcmp.h"

int main(int argc, char** argv) {
    const char *input_filename = "sample.bmp";
    const char *output_filename = "output.bmp";
    int width, height;

    // Extract pixel values into matrix
    int **matrix = read_grayscale_bmp(input_filename, &width, &height);

    if (matrix) {
        printf("Height: %d\n", height);
        printf("Width: %d\n", width);

        // Print some grayscale values for debugging
        for (int i = 0; i < height && i < 10; i++) {
            for (int j = 0; j < width && j < 10; j++) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }

        // Now we adjust matrix for one based indexing
        double **a = dmatrix(1, height, 1, width);

        if(a == NULL){
            printf("Failed to allocate memory for matrix a.\n");
            return 0;
        }
        else{
            printf("Successfully allocated memory.\n");
        }

        for(int i=1; i<= height; i++){
            for(int j=1; j<= width; j++){
                a[i][j] = matrix[i-1][j-1];
            }
        }

        // Initialise matrices to store the decomposition.
        double **U_reduced = NULL, *W_reduced = NULL, **V_reduced = NULL, **V_T = NULL;

        printf("Entering the function\n");

        // Decompose matrix & store decomposed data.
        decomposer(a, height, width, &U_reduced, &W_reduced, &V_reduced, &V_T);

        //generates a greyscale image using the extracted pixel values.
        write_grayscale_bmp(output_filename, matrix, width, height);

        free_matrix(matrix, height);
        free_dmatrix(U_reduced, 1, height, 1, width);
        free_dvector(W_reduced, 1, width);
        free_dmatrix(V_reduced, 1, width, 1, width);
        free_dmatrix(V_T, 1, width, 1, width);
        free_dmatrix(a, 1, height, 1, width);
        printf("Done.\n");
        
    } else {
        printf("Failed to read the BMP file.\n");
    }

    return 0;
}



// gcc -Wall -o masterfile.exe masterfile.c svdcmp.c decomposer.c image_utils.c -lm
// .\masterfile