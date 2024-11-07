#include "PGM_READ_WRITE.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_NO_ERRORS 0
#define EXIT_WRONG_ARG_COUNT 1
#define EXIT_BAD_INPUT_FILE 2
#define EXIT_BAD_OUTPUT_FILE 3

#define MAGIC_NUMBER_RAW_PGM 0x3550
#define MAGIC_NUMBER_ASCII_PGM 0x3250
#define MIN_IMAGE_DIMENSION 1
#define MAX_IMAGE_DIMENSION 65536
#define MAX_COMMENT_LINE_LENGTH 128

int pgmRead (char* filename, struct variables* vars) {
/* the magic number		         */
	/* stored as two bytes to avoid	         */
	/* problems with endianness	         */
	/* Raw:    0x5035 or P5		         */
	/* ASCII:  0x5032 or P2		         */
  	vars->magic_number[0] = '0';
  	vars->magic_number[1] = '0';
	vars->magic_Number = (unsigned short *) vars->magic_number;
	
	/* we will store ONE comment	         */
	vars-> commentLine = NULL;

	/* the logical width & height	         */
	/* note: cannot be negative	         */
	vars-> width = 0;
  	vars-> height = 0;

	/* maximum gray value (assumed)	         */
	/* make it an integer for ease	         */
	vars-> maxGray = 255;

	/* pointer to raw image data	         */
	vars-> imageData = NULL;
	
	/* now start reading in the data         */
	/* try to open the file for text I/O     */
	/* in ASCII mode b/c the header is text  */
	FILE *inputFile = fopen(filename, "r");

	/* if it fails, return error code        */
	if (inputFile == NULL)
		return EXIT_BAD_INPUT_FILE;

	/* read in the magic number              */
	vars-> magic_number[0] = getc(inputFile);
	vars-> magic_number[1] = getc(inputFile);

	/* sanity check on the magic number      */
	//if (vars-> magic_Number != MAGIC_NUMBER_ASCII_PGM)

  //just compares to the string version of P2 or P5 rather than decimal representation
  if (strcmp((char*) vars->magic_number, "P2") == 0 && strcmp((char*) vars->magic_number, "P5") == 0)
		{ /* failed magic number check   */
		/* be tidy: close the file       */
		fclose(inputFile);

		/* print an error message */
    //printf("error1");
		printf("Error: Bad Dimensions %s\n", filename);	
		
		/* and return                    */
		return EXIT_BAD_INPUT_FILE;
		} /* failed magic number check   */

	/* scan whitespace if present            */
	int scanCount = fscanf(inputFile, " ");

	/* check for a comment line              */
	char nextChar = fgetc(inputFile);
	if (nextChar == '#')
		{ /* comment line                */
		/* allocate buffer               */
		vars-> commentLine = (char *) malloc(MAX_COMMENT_LINE_LENGTH);
		/* fgets() reads a line          */
		/* capture return value          */
		char *commentString = fgets(vars-> commentLine, MAX_COMMENT_LINE_LENGTH, inputFile);
		/* NULL means failure            */
		if (commentString == NULL)
			{ /* NULL comment read   */
			/* free memory           */
			free(vars-> commentLine);
			/* close file            */
			fclose(inputFile);

			/* print an error message */
      //printf("error2");
			printf("Error: Failed to read pgm image from file %s\n", filename);	
		
			/* and return            */
			return EXIT_BAD_INPUT_FILE;
			} /* NULL comment read   */
		} /* comment line */
	else
		{ /* not a comment line */
		/* put character back            */
		ungetc(nextChar, inputFile);
		} /* not a comment line */

	/* read in width, height, grays          */
	/* whitespace to skip blanks             */
	if (vars->magic_number[0] == 'P' && vars->magic_number[1] == '2') {
	scanCount = fscanf(inputFile, " %u %u %u", &(vars-> width), &(vars-> height), &(vars-> maxGray));
	}

	/* sanity checks on size & grays         */
	/* must read exactly three values        */
	if 	(
		(scanCount != 3				)	||
		(vars-> width 	< MIN_IMAGE_DIMENSION	) 	||
		(vars-> width 	> MAX_IMAGE_DIMENSION	) 	||
		(vars-> height < MIN_IMAGE_DIMENSION	) 	||
		(vars-> height > MAX_IMAGE_DIMENSION	) 	||
		(vars-> maxGray	!= 255		)
		)
		{ /* failed size sanity check    */
		/* free up the memory            */
		free(vars-> commentLine);

		/* be tidy: close file pointer   */
		fclose(inputFile);

		/* print an error message */
    //printf("error3");
		printf("Error: Failed to read pgm image from file %s\n", filename);	
		
		/* and return                    */
		return EXIT_BAD_INPUT_FILE;
		} /* failed size sanity check    */

	/* allocate the data pointer             */
	vars->nImageBytes = vars->width * vars->height * sizeof(unsigned char);
	vars->imageData = (unsigned char *) malloc(vars->nImageBytes);

	/* sanity check for memory allocation    */
	if (vars->imageData == NULL)
		{ /* malloc failed */
		/* free up memory                */
		free(vars->commentLine);

		/* close file pointer            */
		fclose(inputFile);

		/* print an error message */
    //printf("error4");
		printf("Error: Failed to read pgm image from file %s\n", filename);	
		
		/* return error code             */
		return EXIT_BAD_INPUT_FILE;
		} /* malloc failed */

	/* pointer for efficient read code       */
	for (unsigned char *nextGrayValue = vars->imageData; nextGrayValue < vars->imageData + vars->nImageBytes; nextGrayValue++)
		{ /* per gray value */
		/* read next value               */
		int grayValue = -1;

      //if statement to read binary or read ascii
      
     // if (strcmp((char*) vars->magic_number, "P2") == 1){
        int scanCount = fscanf(inputFile, " %u", &grayValue);
     // }
     // else {
     //   int scanCount = fread(vars-> imageData, sizeof(unsigned char),vars->width * vars->height , inputFile);
    //  }
		
    //printf("%d \n", grayValue);

		/* sanity check	                 */
		if ((scanCount != 1) || (grayValue < 0) || (grayValue > 255))
			{ /* fscanf failed */
			/* free memory           */
			free(vars->commentLine);
			free(vars->imageData);	

			/* close file            */
			fclose(inputFile);

			/* print error message   */
      //printf("error5 %d\n", grayValue);
			printf("Error: Failed to read pgm image from file %s\n", filename);	
		
			/* and return            */
			return EXIT_BAD_INPUT_FILE;
			} /* fscanf failed */

		/* set the pixel value           */
		*nextGrayValue = (unsigned char) grayValue;
		} /* per gray value */

	/* we're done with the file, so close it */
	fclose(inputFile);
  return EXIT_NO_ERRORS;
}

int pgmWrite (char* outputFileName, struct variables* varsjr) {
  /* open a file for writing               */
	FILE *outputfile = fopen(outputFileName, "w");

	/* check whether file opening worked     */
	if (outputfile == NULL)
		{ /* NULL output file */
		/* free memory                   */
		free(varsjr-> commentLine);
		free(varsjr-> imageData);

		/* print an error message        */
		printf("Error: Failed to write pgm image to file %s\n", outputFileName);	

		/* return an error code          */
		return EXIT_BAD_OUTPUT_FILE;
		} /* NULL output file */
	
	/* write magic number, size & gray value */
	size_t nBytesWritten = fprintf(outputfile, "P2\n%d %d\n%d\n", varsjr-> width, varsjr-> height, varsjr-> maxGray);

	/* check that dimensions wrote correctly */
	if (nBytesWritten < 0)
		{ /* dimensional write failed    */
		/* free memory                   */
		free(varsjr-> commentLine);
		free(varsjr-> imageData);

		/* print an error message        */
		printf("Error: Failed to write pgm image to file %s\n", outputFileName);	

		/* return an error code          */
		return EXIT_BAD_OUTPUT_FILE;
		} /* dimensional write failed    */

        /* pointer for efficient write code      */
        for (unsigned char *nextGrayValue = varsjr-> imageData; nextGrayValue < varsjr-> imageData + varsjr-> nImageBytes; nextGrayValue++)
                { /* per gray value */
		/* get next char's column        */
		int nextCol = (nextGrayValue - varsjr-> imageData + 1) % varsjr-> width;

		/* write the entry & whitespace  */
    //write to binary if need
    if (strcmp((char*) varsjr->magic_number, "P5") == 1){
      nBytesWritten = fwrite(varsjr-> imageData, sizeof(unsigned char), (size_t)(varsjr->nImageBytes) ,outputfile);
    }
    else {
      nBytesWritten = fprintf(outputfile, "%d%c", *nextGrayValue, (nextCol ? ' ' : '\n') );
    }
		

		/* sanity check on write         */
		if (nBytesWritten < 0)
			{ /* data write failed   */
			/* free memory           */
			free(varsjr-> commentLine);
			free(varsjr-> imageData);

			/* print error message   */
			printf("Error: Failed to write pgm image to file %s\n", outputFileName);	

			/* return an error code  */
			return EXIT_BAD_OUTPUT_FILE;
			} /* data write failed   */
		} /* per gray value */

	/* at this point, we are done and can exit with a success code */
	return EXIT_NO_ERRORS;
}
