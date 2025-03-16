# Image Compression Project

This project is a fun and interactive image-guessing game that is built using the concept of singular value decomposition in Linear Algebra. The game challenges users to guess the image by progressively providing hints where each hint reveals a more detailed version of the original image (using more singular values from the decomposition).

## Process:
1. Provide the BMP image file and the name of the image.  
2. The image is decomposed and represented using 3 matrices: U, W (the singular values), and V.  
3. The user is provided with hints where each hint generates a more detailed image.  
4. The user is asked to guess until the answer is correct, or they can ask for more hints or exit the game.

## Aim of the Project:
To demonstrate a solid understanding of the C programming language and present an application of Linear Algebra through innovatively designing an engaging game that utilizes SVD as a tool for achieving image compression.

## Instructions:
1. Input the BMP file and its name.  
2. Specify the number of hints to be used.  
3. Give the device to the player.  
4. The player must guess the image name, use hints, or exit the game as needed.  
5. Each image generated through the use of hints will be added to the folder. You will need to click on the image to see it (it does not open automatically).

## Requirements:
1. BMP Images: Only BMP files are supported.
2. C Compiler: GCC or a similar C compiler is required.
3. Libraries: math.h, stdlib.h, ctype.h

Use the following commands in a `cmd` terminal within Visual Studio Code after opening the directory to run the code:

```bash
gcc -Wall -o masterfile.exe masterfile.c svdcmp.c decomposer.c image_utils.c -lm
.\masterfile.exe
