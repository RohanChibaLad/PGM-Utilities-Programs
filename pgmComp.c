#include "pgmComp.h"
#include <stdio.h>

#define EXIT_NO_ERRORS 0
#define EXIT_WRONG_ARG_COUNT 1

int pgmCompare(char *filename1, char *filename2)
{

    struct variables first_image; //Creates a structure for the first file
    struct variables second_image; //Creates a structure for the second file

    pgmRead(filename1, &first_image);
    pgmRead(filename2, &second_image);

    // large if statement that will compare all properties of both files and either output IDENTICAL or DIFFERENT

    if (first_image.width != second_image.width || first_image.height != second_image.height || first_image.maxGray != second_image.maxGray) //Compares all of the properties of both files -- If the width and height are not equal
    {

        // first check if the dimensions and maxGray are equal (preliminary check)

        printf("Width and Height is different.\n");
        return 0;
    }

    for (int i = 0; i < (first_image.height * first_image.width); i++) //Loops through each value in the array and validates whether they are equal or not
    {

        if (first_image.imageData[i] != second_image.imageData[i]) //Loops through the pixels
        {
            printf("Data is different.\n");
            return 0;
        }
    }

    //If the loop ends without failure then files are logically equivalent
    printf("The images are identical\n");
    return 0;
}

int main(int argc, char **argv)
{
    /* main() */
    /* check for correct number of arguments */
    if (argc != 3)
    { /* wrong arg count */
        /* print an error message        */
        printf("Usage: %s input_file input_file\n", argv[0]);
        /* and return an error code      */
        return EXIT_WRONG_ARG_COUNT;
    } /* wrong arg count */

    // compare the two files
    pgmCompare(argv[1], argv[2]);
}