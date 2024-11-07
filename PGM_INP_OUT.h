#include "structure.h"

int pgmRead (char* filename, struct variables* vars);
int pgmWrite (char* outputFile, struct variables* varsjr);
int pgmWriteBinary (char* outputFile, struct variables* varsjr);