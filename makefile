CC=gcc
CCFLAGS = -g -std=c99

all: pgmEcho pgmReduce pgmComp pgm_ascii_to_binary pgm_binary_to_ascii

clean:
	rm -rf pgmEcho pgmReduce pgmComp pgm_ascii_to_binary pgm_binary_to_ascii

pgmEcho:	pgmEcho.c
	${CC} ${CCFLAGS} -o pgmEcho pgmEcho.c PGM_READ_WRITE.c

pgmReduce:  pgmReduce.c PGM_READ_WRITE.c
	${CC} ${CCFLAGS} -o pgmReduce pgmReduce.c PGM_READ_WRITE.c

pgmComp:  pgmComp.c PGM_READ_WRITE.c
	${CC} ${CCFLAGS} -o pgmComp pgmComp.c PGM_READ_WRITE.c

pgm_ascii_to_binary:  pgm_ascii_to_binary.c PGM_READ_WRITE.c PGM_CONVERT.h
	${CC} ${CCFLAGS} -o pgma2b pgm_ascii_to_binary.c PGM_READ_WRITE.c

pgm_binary_to_ascii:  pgm_binary_to_ascii.c PGM_READ_WRITE.c PGM_CONVERT.h
	${CC} ${CCFLAGS} -o pgmb2a pgm_binary_to_ascii.c PGM_READ_WRITE.c
