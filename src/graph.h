#ifndef graph_h
#define graph_h

#include "utils.h"

//typedef unsigned char col_;
//typedef unsigned int idx_;
typedef int col_;
typedef int idx_;

struct Graph{
	idx_ n;                // number of nodes (order of G)
	idx_ m;                // number of edges (size of G)
	col_ *C;               // colour list, of length n
	idx_ *Nv;              // vertices degree, of length n
	idx_ **E;              // adjacency list, for each vertex v of length Nv[v]
};

// Allocation and deallocation
struct Graph * newGraph(idx_ n);
void freeGraph(struct Graph *G);

// IO functions
void displayGraph(struct Graph *G);
struct Graph * readGraph(char *filename);
struct Graph * readIGraph(char *filename);
void writeGraph(char *filename, struct Graph *G);
void exportIGraph(char *filename, struct Graph *G);
void writeAdjacencyMatrix(char *file, struct Graph *G);

// Graph generation
struct Graph * ErdosRenyi(idx_ n, float p);
void assignRandomColoring(struct Graph *G, int c);

#endif /* graph_h */
