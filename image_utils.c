#include "image_utils.h"
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
typedef struct {
    unsigned char bfType[2];
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

int** read_grayscale_bmp(const char *filename, int *width, int *height) {
    FILE *fIn = fopen(filename, "rb");
    if (!fIn) {
        perror("Error opening file");
        return NULL;
    }

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fIn);
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fIn);

    if (fileHeader.bfType[0] != 'B' || fileHeader.bfType[1] != 'M') {
        fprintf(stderr, "Not a valid BMP file\n");
        fclose(fIn);
        return NULL;
    }

    *width = infoHeader.biWidth;
    *height = abs(infoHeader.biHeight);
    int stride = ((*width * 3) + 3) & ~3; // Align to 4 bytes for 24-bit BMP

    int **matrix = (int **)malloc(*height * sizeof(int *));
    if (!matrix) {
        perror("Memory allocation failed");
        fclose(fIn);
        return NULL;
    }

    for (int i = 0; i < *height; i++) {
        matrix[i] = (int *)malloc(*width * sizeof(int));
        if (!matrix[i]) {
            perror("Memory allocation failed");
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            fclose(fIn);
            return NULL;
        }
    }

    fseek(fIn, fileHeader.bfOffBits, SEEK_SET);

    for (int y = 0; y < *height; y++) {
        for (int x = 0; x < *width; x++) {
            unsigned char rgb[3];
            fread(rgb, sizeof(unsigned char), 3, fIn);
            unsigned char gray = (unsigned char)(0.299 * rgb[2] + 0.587 * rgb[1] + 0.114 * rgb[0]);
            matrix[y][x] = gray;
        }
        fseek(fIn, stride - (*width * 3), SEEK_CUR); // Skip padding bytes
    }

    fclose(fIn);
    return matrix;
}

void free_matrix(int **matrix, int height) {
    for (int i = 0; i < height; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void write_grayscale_bmp(const char *filename, int **matrix, int width, int height) {
    FILE *fOut = fopen(filename, "wb");
    if (!fOut) {
        perror("Error opening file for writing");
        return;
    }

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    int stride = (width + 3) & ~3; // Align to 4 bytes
    int imageSize = stride * height;

    fileHeader.bfType[0] = 'B';
    fileHeader.bfType[1] = 'M';
    fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + imageSize + 256 * 4; // Include palette size
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * 4;

    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biWidth = width;
    infoHeader.biHeight = height; // Positive to indicate bottom-up bitmap
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 8; // 8 bits per pixel for grayscale
    infoHeader.biCompression = 0;
    infoHeader.biSizeImage = imageSize;
    infoHeader.biXPelsPerMeter = 0;
    infoHeader.biYPelsPerMeter = 0;
    infoHeader.biClrUsed = 256;
    infoHeader.biClrImportant = 256;

    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fOut);
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fOut);

    // Write grayscale palette
    for (int i = 0; i < 256; i++) {
        unsigned char gray[4] = {i, i, i, 0};
        fwrite(gray, sizeof(unsigned char), 4, fOut);
    }

    // Write pixel data from top to bottom
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char gray = (unsigned char)matrix[y][x];
            fwrite(&gray, sizeof(unsigned char), 1, fOut);
        }
        for (int p = 0; p < (stride - width); p++) {
            unsigned char padding = 0;
            fwrite(&padding, sizeof(unsigned char), 1, fOut);
        }
    }

    fclose(fOut);
}
