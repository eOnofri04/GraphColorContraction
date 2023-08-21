#include "../src/graphContraction.h"

#include <argp.h>

#define DEFAULT_FILENAME "simpleSample"
#define MAX_ITERS 10
const char *argp_program_version = "v1.0";
const char *argp_program_bug_address = "<eonofri@uniroma3.it>";
static char HELP_TOOL[] = "Colour contraction of the given graph\v";
static char REQUIRED_ARGS[] = "";

static struct argp_option argsOptions[] = {
	{ "path", 'p', "filename", 0, "Relative path of the graph to be contracted from `"DATA_DIR"` folder." },
	{ "maxiters", 'm', "maxiters", 0, "Maximum number of algorithms iterations." },
	{ "verbose", 'v', 0, 0, "Produces verbose output." },
	{ "intermediate", 'i', 0, 0, "Saves intermediate contraction (along with initial graph)." },
	{ 0 } 
};

struct arguments {
	bool intermediate;
	bool verbose;
	char *filename;
	unsigned short maxiters;
};

struct arguments * createArgs( ){
	struct arguments *theArgs;

	theArgs = malloc(sizeof(struct arguments));
	if (theArgs == NULL){
		fprintf(stderr, "ERROR in doGraphContraction.c/createArgs() : Unable to mallocate theArgs\n");
		exit(ALLOC_ERR);
	}
	theArgs->filename = DEFAULT_FILENAME;
	theArgs->maxiters = MAX_ITERS;
	theArgs->intermediate = false;
	theArgs->verbose = false;

	return theArgs;
}

static error_t argParse(int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;

	switch (key) {
		case 'p': arguments->filename = arg; break;
		case 'i': arguments->intermediate = true; break;
		case 'v': arguments->verbose = true; break;
		case ARGP_KEY_ARG: fprintf(stdout, "WARNING: Unknown argument parsed: %s", arg); break;
		case ARGP_KEY_END: break;
		default: return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp argp = { argsOptions, argParse, REQUIRED_ARGS, HELP_TOOL, 0, 0, 0 };


int main( int argc, char* argv[] ){
	struct Graph *G;
	int i;
	bool hasFinished = false;
	clock_t t0, t1, dt=0;
	long wt, wdt=0;
	char filepath[2*BUFFER_SIZE];
	FILE *logout;
	struct arguments *theArgs = createArgs( );

	argp_parse(&argp, argc, argv, 0, 0, theArgs);

	logout = fopen("./"LOGS_DIR"log-graph-contraction.txt", "w");


	fprintf(stdout, "------------------Begin---------------\n\n");
	fflush(stdout);
	fprintf(logout, "------------------Begin---------------\n\n");
	fflush(logout);

	snprintf(filepath, 2*BUFFER_SIZE, "%s%s%s", DATA_DIR, theArgs->filename, GRAPH_SUFFIX);

	fprintf(stdout, "Reading the graph from the file %s... ", filepath);
	fflush(stdout);
	fprintf(logout, "Reading the graph from the file %s... ", filepath);
	fflush(logout);
	walltic();
	t0 = clock();
	//-----------------------++
	G = readGraph(filepath); //
	//-----------------------++
	t1 = clock();
	wt = walltoc();
	fprintf(stdout, "OK.\n");
	if (theArgs->verbose){
		fprintf(stdout, "|V| = %d, |E| = %d\n", G->n, G->m);
		fprintf(stdout, "Exec time: %lu ms.\n", 1000 * (t1 - t0) / CLOCKS_PER_SEC);
		fprintf(stdout, "Wall time: %lu ms.\n", wt);
	}
	fflush(stdout);
	fprintf(logout, "OK.\n");
	fprintf(logout, "|V| = %d, |E| = %d\n", G->n, G->m);
	fprintf(logout, "Exec time: %lu ms.\n", 1000 * (t1 - t0) / CLOCKS_PER_SEC);
	fprintf(logout, "Wall time: %lu ms.\n", wt);
	fflush(logout);
	

	if (theArgs->intermediate){
		snprintf(filepath, 2*BUFFER_SIZE, "%s%s_%d%s", RESULTS_DIR, theArgs->filename, 0, GRAPH_SUFFIX);
		fprintf(stdout, "Writing the original graph on the file %s... ", filepath);
		fflush(stdout);
		fprintf(logout, "Writing the original graph on the file %s... ", filepath);
		fflush(logout);
		walltic();
		t0 = clock();
		//-----------------------++
		writeGraph(filepath, G); //
		//-----------------------++
		t1 = clock();
		wt = walltoc();
		fprintf(stdout, "OK.\n\n");
		if (theArgs->verbose){
			fprintf(stdout, "Exec time: %lu ms.\n", 1000 * (t1 - t0) / CLOCKS_PER_SEC);
			fprintf(stdout, "Wall time: %lu ms.\n", wt);
		}
		fflush(stdout);
		fprintf(logout, "OK.\n");
		fprintf(logout, "Exec time: %lu ms.\n", 1000 * (t1 - t0) / CLOCKS_PER_SEC);
		fprintf(logout, "Wall time: %lu ms.\n", wt);
		fflush(logout);
	}

	fprintf(stdout, "Multicontraction phase...\n\n");
	fflush(stdout);
	fprintf(logout, "Multicontraction phase...\n\n");
	fflush(logout);
	for (i = 1; i < theArgs->maxiters && !hasFinished; i++){
		fprintf(stdout, "\tIteration no.%d... ", i);
		fflush(stdout);
		fprintf(logout, "\tIteration no.%d... ", i);
		fflush(logout);
		walltic();
		t0 = clock();
		//---------------------------------------++
		hasFinished = !graphColorContraction(G); //
		//---------------------------------------++
		t1 = clock();
		wt = walltoc();
		dt += (t1 - t0);
		wdt += wt;
		fprintf(stdout, "OK.\n");
		if (theArgs->verbose){
			fprintf(stdout, "\t|V| = %d, |E| = %d\n", G->n, G->m);
			fprintf(stdout, "\tIteration exec time: %lu ms.\n", 1000 * (t1 - t0) / CLOCKS_PER_SEC);
			fprintf(stdout, "\tIteration wall time: %lu ms.\n", wt);
		}
		fflush(stdout);
		fprintf(logout, "OK.\n");
		fprintf(logout, "\t|V| = %d, |E| = %d\n", G->n, G->m);
		fprintf(logout, "\tIteration exec time: %lu ms.\n", 1000 * (t1 - t0) / CLOCKS_PER_SEC);
		fprintf(logout, "\tIteration wall time: %lu ms.\n", wt);
		fflush(logout);

		if (theArgs->intermediate && !hasFinished){
			snprintf(filepath, 2*BUFFER_SIZE, "%s%s_%d%s", RESULTS_DIR, theArgs->filename, i, GRAPH_SUFFIX);
			fprintf(stdout, "\tWriting the contracted graph on the file %s... ", filepath);
			fflush(stdout);
			fprintf(logout, "\tWriting the contracted graph on the file %s... ", filepath);
			fflush(logout);
			walltic();
			t0 = clock();
			//-----------------------++
			writeGraph(filepath, G); //
			//-----------------------++
			t1 = clock();
			wt = walltoc();
			fprintf(stdout, "OK.\n");
			if (theArgs->verbose){
				fprintf(stdout, "\tExec time: %lu ms.\n", 1000 * (t1 - t0) / CLOCKS_PER_SEC);
				fprintf(stdout, "\tWall time: %lu ms.\n", wt);
			}
			fflush(stdout);
			fprintf(logout, "OK.\n");
			fprintf(logout, "\tExec time: %lu ms.\n", 1000 * (t1 - t0) / CLOCKS_PER_SEC);
			fprintf(logout, "\tWall time: %lu ms.\n", wt);
			fflush(logout);
		}

		fprintf(stdout, "\n");
		fflush(stdout);
		fprintf(logout, "\n");
		fflush(logout);
	}
	fprintf(stdout, "Concluding the Multicontraction phase... ");
	fflush(stdout);
	fprintf(logout, "Concluding the Multicontraction phase... ");
	fflush(logout);

	fprintf(stdout, "%s.\n\n", hasFinished ? "SUCCESS" : "FAILURE");
	fflush(stdout);
	fprintf(logout, "%s.\n\n", hasFinished ? "SUCCESS" : "FAILURE");
	fflush(logout);

	snprintf(filepath, 2*BUFFER_SIZE, "%s%s_final%s", RESULTS_DIR, theArgs->filename, GRAPH_SUFFIX);
	fprintf(stdout, "Writing the final graph on the file %s... ", filepath);
	fflush(stdout);
	fprintf(logout, "Writing the final graph on the file %s... ", filepath);
	fflush(logout);
	walltic();
	t0 = clock();
	//-----------------------++
	writeGraph(filepath, G); //
	//-----------------------++
	t1 = clock();
	wt = walltoc();
	fprintf(stdout, "OK.\n\n");
	if (theArgs->verbose){
		fprintf(stdout, "Exec time: %lu ms.\n", 1000 * (t1 - t0) / CLOCKS_PER_SEC);
		fprintf(stdout, "Wall time: %lu ms.\n\n", wt);
	}
	fflush(stdout);
	fprintf(logout, "OK.\n");
	fprintf(logout, "Exec time: %lu ms.\n", 1000 * (t1 - t0) / CLOCKS_PER_SEC);
	fprintf(logout, "Wall time: %lu ms.\n\n", wt);
	fflush(logout);

	fprintf(stdout, "Number of iterations: %d + 1 (to determine the stopping condition)\n", i-2);
	if (theArgs->verbose){
		fprintf(stdout, "|V| = %d, |E| = %d\n", G->n, G->m);
		fprintf(stdout, "Total exec time: %lu ms.\n", 1000 * dt / CLOCKS_PER_SEC);
		fprintf(stdout, "Total wall time: %lu ms.\n\n", wdt);
	}
	fflush(stdout);
	fprintf(stdout, "OK.\n\n");

	fprintf(logout, "Number of iterations: %d + 1 (to determine the stopping condition)\n", i-2);
	fprintf(logout, "|V| = %d, |E| = %d\n", G->n, G->m);
	fprintf(logout, "Total exec time: %lu ms.\n", 1000 * dt / CLOCKS_PER_SEC);
	fprintf(logout, "Total wall time: %lu ms.\n\n", wdt);
	fflush(logout);

	fprintf(stdout, "------------------End---------------\n");
	fflush(stdout);
	fprintf(logout, "------------------End---------------\n");
	fflush(logout);

	return 0;
}