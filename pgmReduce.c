#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "PGM_READ_WRITE.h"

#define EXIT_NO_ERRORS 0
#define EXIT_WRONG_ARG_COUNT 1

int pgmReduction(char *inFile, char *scale_factor, char *outFile)
{

    struct variables input_image; //Structure for inputted image
    struct variables scaled_image; //Structure for outputed image

    long n = strtol(scale_factor, NULL, 10); //Need to convert string to int for scale_factor to be able to use it

    pgmRead(inFile, &input_image);

    scaled_image.commentLine = input_image.commentLine;
    scaled_image.maxGray = input_image.maxGray;
    scaled_image.magic_number[0] = input_image.magic_number[0];
    scaled_image.magic_number[1] = input_image.magic_number[1];
    scaled_image.magic_Number = input_image.magic_Number;

    //Calculates the new dimensions using the scale_factor above from the command line
    scaled_image.width = (ceil((float)input_image.width -1 ) / n);
    scaled_image.height = (ceil((float)input_image.height -1 ) / n);
    scaled_image.nImageBytes = scaled_image.width * scaled_image.height * sizeof(unsigned char);
    scaled_image.imageData = (unsigned char *)malloc(scaled_image.nImageBytes);

    int input_imagenumCells = input_image.width * input_image.height;
    int scaled_imagenumCells = 0;

    int row = 0;
    int col = 0;
    unsigned char cell;

    for (int i = 0; i < input_imagenumCells; i++)
    {
        cell = input_image.imageData[input_image.width * row + col]; //Gets the coordinates

        if (row % n == 0 && col % n == 0) //If the current coordinate should be in the new image
        {
            scaled_image.imageData[scaled_imagenumCells] = cell;
            scaled_imagenumCells++;
        }

        col++;
        if (col >= input_image.width)
        {
            col = 0;
            row++;
        }
    }

    pgmWrite(outFile, &scaled_image);
    return 0;
}

int main(int argc, char **argv)
{
    /* main() */
    /* check for correct number of arguments */
    if (argc != 4)
    { /* wrong arg count */
        /* print an error message        */
        printf("Usage: %s  inputImage.pgm reduction_scale_factor outputImage.pgm\n", argv[0]);
        /* and return an error code      */
        return EXIT_WRONG_ARG_COUNT;
    } /* wrong arg count */

    // reduce the image

    pgmReduction(argv[1], argv[2], argv[3]);

    // return string and 0

    //printf("PGM image has been reduced\n");
    return 0;
}
