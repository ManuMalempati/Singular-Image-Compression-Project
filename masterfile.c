#include <stdio.h>
#include "image_utils.h"

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

        //generate a greyscale image using the extracted pixel values.
        write_grayscale_bmp(output_filename, matrix, width, height);
        free_matrix(matrix, height);
        
    } else {
        printf("Failed to read the BMP file.\n");
    }

    return 0;
}



// gcc -Wall -o masterfile.exe masterfile.c image_utils.c
// .\masterfile