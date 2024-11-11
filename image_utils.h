#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

int** read_grayscale_bmp(const char *filename, int *width, int *height);
void free_matrix(int **matrix, int height);
void write_grayscale_bmp(const char *filename, int **matrix, int width, int height); // Ensure this line is present

#endif // IMAGE_UTILS_H
