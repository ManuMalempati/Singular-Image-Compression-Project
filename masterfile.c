#include <stdio.h>
#include "image_utils.h"
#include "svdcmp.h"
#include <math.h>
#include <stdlib.h>

double** column_row_scalar_multiply(double *col, double *row, int col_size, int row_size, double scalar);
void add_matrices(double **matrix1, double **matrix2, int rows, int columns);
int strcmp(char s1[], char s2[]);

int main(int argc, char** argv) {
    char input_filename[1024];
    char answer[1024];
    printf("Please enter the filename of the image you want to guess (Max 1024 characters): ");
    scanf("%s", input_filename);
    printf("Please enter the name of the image (Max 1024 characters): ");
    scanf("%s", answer);
    printf("Loading %s...\n", input_filename);
    const char *output_filename = "output.bmp";
    int width, height, hints;

    // The number of hints, is the number of rows, columns and singular values to include incrementally from the decomposed matrices.

    // Extract pixel values into matrix
    double **matrix = read_grayscale_bmp(input_filename, &width, &height);

    printf("How many hints?: ");
    scanf("%d", &hints);

    if (matrix) {
        printf("Height: %d\n", height);
        printf("Width: %d\n", width);

        // Print some grayscale values for debugging
        for (int i = 0; i < height && i < 10; i++) {
            for (int j = 0; j < width && j < 10; j++) {
                printf("%f ", matrix[i][j]);
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
        /* We have to pass the addresses of the matrices, otherwise the decomposer function will end up receiving a copy of the pointers.
           Passing the address, allows allocation as follows: *U_reduced = ()malloc()...
           *U_reduced (dereferenced) gives the address of **U_reduced so that memory can then be allocated.
        */
        decomposer(a, height, width, &U_reduced, &W_reduced, &V_reduced, &V_T);

        int singular_values = 0;

        if(height >= width){
            singular_values = height;
        }
        else{
            singular_values = width;
        }


        //int increment_by = singular_values/hints;
        int increment_by = 5;

        int counter = 1;
        

        double **result = dmatrix(1, height, 1, width);
        // Initialize result matrix to zero
        for (int i = 1; i <= height; i++) {
            for (int j = 1; j <= width; j++) {
                result[i][j] = 0.0;
            }
        }

        char filename[100];

        printf("Please give the device to the other user to guess.\n");
        
        for(int i=0; i < hints; i++){
            // pick counter-counter+increment_by rows from U_reduced, values from W_reduced and columns from V_T
            
            for(int j=counter; j<counter+increment_by && j<= singular_values; j++){
                double *U_slice = (double*)calloc(height, sizeof(double));
                double *V_slice = (double*)calloc(width, sizeof(double));
                if(U_slice == NULL || V_slice == NULL){
                    return 0;
                }
                double singular_value = W_reduced[j];
                for(int k=1; k<=height; k++){
                    U_slice[k-1] = U_reduced[k][j];
                }
                for(int k=1; k<=width; k++){
                    V_slice[k-1] = V_T[j][k];
                }

                double **temp_result = column_row_scalar_multiply(U_slice, V_slice, height, width, singular_value);
                add_matrices(result, temp_result, height, width);
                
                free(U_slice);
                free(V_slice);
                free_dmatrix(temp_result, 1, height, 1, width);
            }
            counter = counter + increment_by;
            sprintf(filename, "output_hint_%d.bmp", i+1);
            write_grayscale_bmp(filename, result, width, height);
            char guess[100];
            printf("%s has been generated.\nType your guess (type 'h' to use the next hint, type '0' to exit): ", filename);
            scanf("%s", guess);
            while((strcmp(guess, answer) != 0) && (strcmp(guess, "h") != 0 )){
                printf("Incorrect.\n");
                printf("Type your guess (type 'h' to use the next hint, type '0' to exit): ");
                scanf("%s", guess);
            }
            if(strcmp(guess, answer) == 0){
                printf("Correct! You guessed the answer with %d hint(s).\n", i);
                // FIX THIS, YOU HAVENT FREED ALL THE MEMORY YET
                return 0;
            }

        }

        //generates a greyscale image using the extracted pixel values.
        write_grayscale_bmp(output_filename, matrix, width, height);
        free_dmatrix(result, 1, height, 1, width);
        free_matrix(matrix, height);
        free_dmatrix(U_reduced, 1, height, 1, width);
        free_dvector(W_reduced, 1, width);
        free_dmatrix(V_reduced, 1, width, 1, width);
        free_dmatrix(V_T, 1, width, 1, width);
        free_dmatrix(a, 1, height, 1, width);
        //printf("You have failed to guess the correct answer :(\n");
        printf("Done.\n");

        
    } else {
        printf("Failed to read the BMP file.\n");
    }

    return 0;
}

int strcmp(char s1[], char s2[]){
    int counter=0;
    while(1){
        if(s1[counter] != s2[counter]){
            return s1[counter] - s2[counter];
        }
        if(s1[counter] == '\0' && s2[counter] == '\0'){
            return 0;
        }
        counter++;
    }
}

// Helper function for column-row matrix multiplication with a scalar
double** column_row_scalar_multiply(double *col, double *row, int col_size, int row_size, double scalar) {
    double **result = (double **)malloc((col_size + 1) * sizeof(double *));
    if (result == NULL) {
        perror("Failed to allocate memory for result matrix.");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i <= col_size; i++) {
        result[i] = (double *)malloc((row_size + 1) * sizeof(double));
        if (result[i] == NULL) {
            perror("Failed to allocate memory for row.");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 1; i <= col_size; i++) {
        for (int j = 1; j <= row_size; j++) {
            result[i][j] = scalar * col[i-1] * row[j-1];
        }
    }

    return result;
}

// Function to add a matrix to an existing matrix
void add_matrices(double **matrix1, double **matrix2, int rows, int columns) {
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= columns; j++) {
            matrix1[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
}




// gcc -Wall -o masterfile.exe masterfile.c svdcmp.c decomposer.c image_utils.c -lm
// .\masterfile