#ifndef utils_h
#define utils_h

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define SERIAL_FILE "serial.number"
#define DATA_DIR "data/"
#define CONFIG_DIR "config/"
#define LOGS_DIR "logs/"
#define RESULTS_DIR "results/"
#define GEN_FOLDER "generated/"
#define ER_SUFFIX "ER"
#define CONTRACTED_SUFFIX "_c"
#define MATRIX_SUFFIX "_m"
#define GRAPH_SUFFIX ".txt"
#define SEED 1234
#define BUFFER_SIZE 1024

#define FOPEN_ERR -4
#define SCANF_ERR -5
#define INPUT_ERR -6
#define ALLOC_ERR -7

#define DEBUG(x) fprintf(stdout, x)
#define DEBUGint(x) fprintf(stdout, "%d", x);

void setSeeds( void );

void walltic( void );
long walltoc( void );

#endif /* utils_h */
