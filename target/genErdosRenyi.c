#include "../src/graph.h"

#include <argp.h>

const char *argp_program_version = "v1.0";
const char *argp_program_bug_address = "<eonofri@uniroma3.it>";
static char HELP_TOOL[] = "Generation of a binomial Erdos-Renyi graph\v";
static char REQUIRED_ARGS[] = "";

static struct argp_option argsOptions[] = {
	{ "nodes",   'n', "#nodes", 0, "Number of nodes." },
	{ "prob",    'p', "probability", 0, "Probability of having an edge between two nodes." },
	{ "colours", 'c', "#colours", 0, "Number of colours." },
	{ "verbose", 'v', 0, 0, "Produces verbose output." },
	{ 0 }
};

struct arguments {
	idx_ n;
	float p;
	col_ c;
	bool verbose;
};

struct arguments * createArgs( ){
	struct arguments *theArgs;

	theArgs = malloc(sizeof(struct arguments));
	if (theArgs == NULL){
		fprintf(stderr, "ERROR in genErdosRenyi.c/createArgs() : Unable to mallocate theArgs\n");
		exit(ALLOC_ERR);
	}
	theArgs->n = -1;
	theArgs->p = -1;
	theArgs->c = -1;
	theArgs->verbose = false;

	return theArgs;
}

static error_t argParse(int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;

	switch (key) {
		case 'n':
			if (sscanf(arg, "%d", &(arguments->n)) != 1) {
				fprintf(stderr, "ERROR in genErdosRenyi.c/argParse() : Unable to read the number of nodes.\n");
				exit(INPUT_ERR);
			}
			break;
		case 'p':
			if (sscanf(arg, "%f", &(arguments->p)) != 1) {
				fprintf(stderr, "ERROR in genErdosRenyi.c/argParse() : Unable to read the probability parameter.\n");
				exit(INPUT_ERR);
			}
			break;
		case 'c':
			if (sscanf(arg, "%d", &(arguments->c)) != 1) {
				fprintf(stderr, "ERROR in genErdosRenyi.c/argParse() : Unable to read the number of colours.\n");
				exit(INPUT_ERR);
			}
			break;
		case 'v':
			arguments->verbose = true;
		case ARGP_KEY_ARG: break;
		case ARGP_KEY_END: break;
		default: return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp argp = { argsOptions, argParse, REQUIRED_ARGS, HELP_TOOL, 0, 0, 0 };

int main(int argc, char *argv[]){
	struct Graph *G;
	FILE *fp = NULL;
	int serial = -1;
	char filename[BUFFER_SIZE];
	clock_t t0, t1;
	long wt, wdt=0;

	struct arguments *theArgs = createArgs( );

	argp_parse(&argp, argc, argv, 0, 0, theArgs);

	setSeeds();

	while (theArgs->n < 1){
		printf("Insert the number of nodes:\n>> ");
		if ( scanf("%d", &theArgs->n) != 1 ){
			fprintf(stderr, "ERROR in genErdosRenyi.c/main() : scanf(d, &n)\n");
			exit(SCANF_ERR);
		}
	}

	while (theArgs->p < 0 || theArgs->p > 1){
		printf("Insert the probability parameter p:\n>> ");
		if ( scanf("%f", &theArgs->p) != 1 ){
			fprintf(stderr, "ERROR in genErdosRenyi.c/main() : scanf(f, &p)\n");
			exit(SCANF_ERR);
		}
	}

	while (theArgs->c < 1){
		printf("Insert the number of colours:\n>> ");
		if ( scanf("%d", &theArgs->c) != 1 ){
			fprintf(stderr, "ERROR in genErdosRenyi.c/main() : scanf(d, &c)\n");
			exit(SCANF_ERR);
		}
	}

	if (theArgs->c > theArgs->n){
		fprintf(stdout, "WARNING in genErdosRenyi.c/main() : : colour number is greater than the number of nodes.\n");
	}

	if (theArgs->verbose){
		fprintf(stdout, "Begin Erdos-Renyi graph generation... ");
		fflush(stdout);
	}
	walltic();
	t0 = clock();
	//--------------------------------------++
	G = ErdosRenyi(theArgs->n, theArgs->p); //
	//--------------------------------------++
	t1 = clock();
	wt = walltoc();
	if (theArgs->verbose){
		fprintf(stdout, "OK\n");
		fprintf(stdout, "|V| = %d, |E| = %d\n", G->n, G->m);
		fprintf(stdout, "Exec time: %lu ms.\n", 1000 * (t1 - t0) / CLOCKS_PER_SEC);
		fprintf(stdout, "Wall time: %lu ms.\n", wt);
		fflush(stdout);
	}

	if (theArgs->verbose){
		fprintf(stdout, "Colour generation... ");
		fflush(stdout);
	}
	walltic();
	t0 = clock();
	//-----------------------------------++
	assignRandomColoring(G, theArgs->c); //
	//-----------------------------------++
	t1 = clock();
	wt = walltoc();
	if (theArgs->verbose){
		fprintf(stdout, "OK\n");
		fprintf(stdout, "Exec time: %lu ms.\n", 1000 * (t1 - t0) / CLOCKS_PER_SEC);
		fprintf(stdout, "Wall time: %lu ms.\n", wt);
		fflush(stdout);
	}

	fp = fopen("./"CONFIG_DIR""SERIAL_FILE, "r");
	if ( fscanf(fp, "%d", &serial) != 1 ){
		fprintf(stderr, "ERROR in genErdosRenyi.c/main() : fscanf(fp, d, &serial)\n");
		exit(SCANF_ERR);
	}
	fclose(fp);

	snprintf(filename, BUFFER_SIZE, "%s%s%05d_%s(%d,%4.3f,%d).txt", DATA_DIR, GEN_FOLDER, serial, ER_SUFFIX, theArgs->n, theArgs->p, theArgs->c);

	fp = fopen("./"CONFIG_DIR""SERIAL_FILE, "w");
	fprintf(fp, "%d", serial+1);
	fclose(fp);

	if (theArgs->verbose){
		fprintf(stdout, "Writing the graph on the file %s... ", filename);
		fflush(stdout);
	}
	walltic();
	t0 = clock();
	//-----------------------++
	writeGraph(filename, G); //
	//-----------------------++
	t1 = clock();
	wt = walltoc();
	if (theArgs->verbose){
		fprintf(stdout, "OK\n");
		fprintf(stdout, "Exec time: %lu ms.\n", 1000 * (t1 - t0) / CLOCKS_PER_SEC);
		fprintf(stdout, "Wall time: %lu ms.\n", wt);
		fflush(stdout);
	}
	
	return 0;
}