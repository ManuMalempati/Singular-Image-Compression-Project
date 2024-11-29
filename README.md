# Image compression project

This project is a fun and interactive image-guessing game that is built using the concept of singular value decomposition in Linear Algebra. The game challenges users to guess the image by progressively providing hints where each hint reveals a more detailed version of the original image (using more singular values from the decomposition).

Process:
1. Provide the bmp image file and the name of the image.
2. The image is decomposed and represented using 3 matrices U, W (the singular values) and V.
3. The user is provided with hints where each hint generates a more detailed image.
4. If the user is asked for to guess until it is correct or they can ask for more hints or exit the game.

Aim of the project:
To demonstrate a solid understanding of the C programming language and present an application of Linear algebra through innovatively designing an engaging game that utilizes SVD as a tool for achieving image compression.

Instructions:
Input the BMP file and its name.
Specify the number of hints to be used.
Give the device to the player.
GThe player must guess the image name or use hints or exit the game as needed.
Each image generated through the use of hints will be added to the folder, you will need to click on the image to see it (it does not open automatically).

Use the following commands in a cmd terminal within visual studio code after opening the directory to run the code.
gcc -Wall -o masterfile.exe masterfile.c svdcmp.c decomposer.c image_utils.c -lm
.\masterfile

Requirements:
BMP Images: Only BMP files are supported.
C Compiler: Ensure GCC or a similar C compiler is installed.
Libraries: math.h, stdlib.h, and ctype.h for processing