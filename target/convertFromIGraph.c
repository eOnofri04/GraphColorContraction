#include "../src/graphContraction.h"

#define DEFAULT_OUTPUT_FILENAME "output_graph"

#include <argp.h>

const char *argp_program_version = "v1.0";
const char *argp_program_bug_address = "<eonofri@uniroma3.it>";
static char HELP_TOOL[] = "Convert the given graph from IGraph format\v";
static char REQUIRED_ARGS[] = "input_filename";

static struct argp_option argsOptions[] = {
	{ "output", 'o', "output", 0, "Output file relative destination." },
	{ "colours", 'c', "colours", 0, "Select the number of colours." },
	{ "verbose", 'v', 0, 0, "Produces verbose output." },
	{ 0 }
};

struct arguments {
    col_ cmax;
	char *inputFile;
    char *outputFile;
	bool verbose;
};

struct arguments * createArgs( ){
	struct arguments *theArgs;

	theArgs = malloc(sizeof(struct arguments));
	if (theArgs == NULL){
		fprintf(stderr, "ERROR in convertFromIGraph.c/createArgs() : Unable to mallocate theArgs\n");
		exit(ALLOC_ERR);
	}
	
    theArgs->cmax = 1;
	theArgs->inputFile  = NULL;
    theArgs->outputFile = DEFAULT_OUTPUT_FILENAME;
	theArgs->verbose = false;

	return theArgs;
}

static error_t argParse(int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;

	switch (key) {
		case 'c':
            if (sscanf(arg, "%d", &(arguments->cmax)) != 1) {
				fprintf(stderr, "ERROR in convertFromIGraph.c/argParse() : Unable to read the number of colours.\n");
				exit(INPUT_ERR);
			}
            break;
		case 'o':  arguments->outputFile = arg;  break;
		case 'v':  arguments->verbose = true;   break;
		case ARGP_KEY_ARG:
			if (state->arg_num > 0){  argp_usage(state);  }
			arguments->inputFile = arg;
			break;
		case ARGP_KEY_END:
			if (state->arg_num < 1){  argp_usage(state);  }
			break;
		default: return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp argp = { argsOptions, argParse, REQUIRED_ARGS, HELP_TOOL, 0, 0, 0 };

int main( int argc, char* argv[] ){
    struct Graph *G;
	struct arguments *theArgs;
    char infilepath[2*BUFFER_SIZE];
	char outfilepath[2*BUFFER_SIZE];

	theArgs = createArgs();

	argp_parse(&argp, argc, argv, 0, 0, theArgs);

    setSeeds();

	snprintf(infilepath, 2*BUFFER_SIZE, "%s%s%s", DATA_DIR, theArgs->inputFile, GRAPH_SUFFIX);
	snprintf(outfilepath, 2*BUFFER_SIZE, "%s%s%s%s", DATA_DIR, GEN_FOLDER, theArgs->outputFile, GRAPH_SUFFIX);

	if (theArgs->verbose){
		fprintf(stdout, "Reading graph from IGraph format file:\n\t%s\n", infilepath);
	}
    G = readIGraph(infilepath);
	if (theArgs->verbose){
		fprintf(stdout, "Reading complete\n\n");
	}

    if (theArgs->verbose){
		fprintf(stdout, "Assigning random colouring with %d colours... ", theArgs->cmax);
	}
    assignRandomColoring(G, theArgs->cmax);
	if (theArgs->verbose){
		fprintf(stdout, "OK.\n\n");
	}

	if (theArgs->verbose){
		fprintf(stdout, "Writing the graph to:\n\t%s\n", outfilepath);
	}
    writeGraph(outfilepath, G);
	if (theArgs->verbose){
		fprintf(stdout, "Export complete\n\n");
	}

    return 0;    
}