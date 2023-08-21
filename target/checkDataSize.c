#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main( void ){
	printf("Size of data type:\n");
	printf(" - double = %lu\n", sizeof(double));
	printf(" - float  = %lu\n", sizeof(float) );
	printf(" - bool   = %lu\n", sizeof(bool)  );
	printf(" - char   = %lu\n", sizeof(char)  );
	printf(" - int    = %lu\n", sizeof(int)   );
	return 0;
}