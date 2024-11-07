#include "PGM_CONVERT.h"
#include <stdio.h>
#include <string.h>

#define EXIT_WRONG_ARG_COUNT 1

int pgma2b(char *inFile, char *outFile)
{

    struct variables first_image; //Creates a structure for the first image
    struct variables final_image; //Creates a structure for the final image

    pgmRead(inFile, &first_image); //Reads the first image

    if (strcmp((char *)first_image.magic_number, "P5") == 0) // Checks if the wrong file type has been inputed
    {
        printf("The input file was not of type: binary PGM");
        return 1;
    }

    // Writes the new file as ASCII

    final_image.magic_number[0] = 'P';
    final_image.magic_number[1] = '2';

    final_image.commentLine = first_image.commentLine;

    final_image.width = first_image.width;
    final_image.height = first_image.height;
    final_image.maxGray = first_image.maxGray;

    final_image.imageData = first_image.imageData;

    final_image.nImageBytes = first_image.nImageBytes;

    //Outputs it to the new file
    pgmWrite(outFile, &first_image);
    return 0;
}

int main(int argc, char **argv)
{
    /* main() */
    /* check for correct number of arguments */
    if (argc != 3)
    { /* wrong arg count */
        /* print an error message        */
        printf("Usage: %s  inputImage.pgm outputImage.pgm\n", argv[0]);
        /* and return an error code      */
        return EXIT_WRONG_ARG_COUNT;
    } /* wrong arg count */

    pgma2b(argv[1], argv[2]);

    // return string and 0

    printf("Converted binary PGM to ASCII PGM\n");
    return 0;
}