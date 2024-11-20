#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

double** read_grayscale_bmp(const char *filename, int *width, int *height);
void free_matrix(double **matrix, int height);
void write_grayscale_bmp(const char *filename, double **matrix, int width, int height); // Ensure this line is present

#endif // IMAGE_UTILS_H
