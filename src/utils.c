#include "utils.h"

struct timeval timestart;
struct timeval timestop;

/**
 * @brief Start/restart the time counter
 * 
 */
void walltic( void ){
	gettimeofday(&timestart, NULL);
	return;
}

/**
 * @brief Elapses the time since last tic was raised (in milliseconds)
 * 
 * @return long elapsed time
 */
long walltoc( void ){
	gettimeofday(&timestop, NULL);
	long start = (long)(timestart.tv_sec) * 1000 + (long)timestart.tv_usec / 1000;
	long stop = (long)(timestop.tv_sec) * 1000 + (long)timestop.tv_usec / 1000;
	return stop-start;
}

void setSeeds( void ){
	srand( SEED );
	srand48( SEED );
}