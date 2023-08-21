#ifndef graphContraction_h
#define graphContraction_h

#include "graph.h"

struct ContractionMapping{
	idx_ n;                // order of the original graph
	idx_ n1;               // order of the contracted graph
	idx_ na;               // current size of cSize and revBecomes, either n or n'
	idx_ *Csize;           // color clusters size, of length na
	idx_ *becomes;         // beta mapping, of length n
	idx_ **revBecomes;     // reverse beta mapping, of length na, for each vertex v of length Csize[v] 
};

// Allocation and deallocation
struct ContractionMapping * newContractionMapping(idx_ n);
void freeContractionMapping(struct ContractionMapping *cMap);

// Inner Functions
void becomesInitialization(struct Graph *G, struct ContractionMapping *cMap);
void becomesUpdate(struct ContractionMapping *cMap);
void evaluateClusterSize(struct ContractionMapping *cMap);
void extractReverseBecomesMapping(struct ContractionMapping *cMap);
void revBecomesCompacting(struct ContractionMapping *cMap);
void becomesCompacting(struct ContractionMapping *cMap);
void edgesDestinationUpdate(struct Graph *G, struct ContractionMapping *cMap);
void colorClusterMerge(struct Graph *G, struct ContractionMapping *cMap);
void vertexContraction(struct Graph *G, struct ContractionMapping *cMap);

// Groupping Functions
struct ContractionMapping * evaluateContractionMapping(struct Graph *G);
void applyGraphContraction(struct Graph *G, struct ContractionMapping *cMap);

// Outer Functions
bool graphColorContraction(struct Graph *G);

// IO functions
void displayContractionMapping(struct ContractionMapping *cMap);

#endif /* graphContraction_h */
