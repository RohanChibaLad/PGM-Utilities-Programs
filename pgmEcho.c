#include "PGM_INP_OUT.h"
#include <stdio.h>

#define EXIT_NO_ERRORS 0
#define EXIT_WRONG_ARG_COUNT 1

int main(int argc, char **argv){
  /* main() */
	/* check for correct number of arguments */
	if (argc != 3)	
		{ /* wrong arg count */
		/* print an error message        */
		printf("ERROR: Bad Argument Count\n", argv[0]);
		/* and return an error code      */
		return EXIT_WRONG_ARG_COUNT;
		} /* wrong arg count */
  
  //defining structure for read and write parameter
  struct variables vars;

  //read and write the file (echo)
  if (pgmRead(argv[1], &vars) == -8) {
    return 2;
  }
  pgmRead(argv[1], &vars);
  pgmWrite(argv[2], &vars);

  // return string and 0 value

  //printf("ECHOED\n");
  return 0;
}