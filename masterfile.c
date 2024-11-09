#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void freeImage(unsigned char *imageData);
void loadImage(const char *filename, unsigned char **imageData, int *width, int *height, int *channels);

void loadImage(const char *filename, unsigned char **imageData, int *width, int *height, int *channels) { 
    // Load the image 
    *imageData = stbi_load(filename, width, height, channels, 0); 
    if (*imageData == NULL) { 
        printf("Error in loading the image\n"); 
        exit(1); 
    } 
} 
 
void freeImage(unsigned char *imageData) { 
    stbi_image_free(imageData); 
} 
 
int main() { 
    const char *filename = "highway.png"; // Change this to your image file 
    unsigned char *imageData; 
    int width, height, channels; 
 
    loadImage(filename, &imageData, &width, &height, &channels); 
 
    // Print image dimensions and channels 
    printf("Image width: %d, height: %d, channels: %d\n", width, height, channels); 
 
    // Access pixel data (for example, print the first pixel) 
    if (channels >= 3) { 
        printf("First pixel (R, G, B): (%d, %d, %d)\n",  
               imageData[0], imageData[1], imageData[2]); 
    } 
 
    // Free the image memory 
    freeImage(imageData); 
    return 0; 
} 
