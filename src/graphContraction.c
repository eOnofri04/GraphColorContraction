#include "graphContraction.h"

struct ContractionMapping * newContractionMapping(idx_ n){
	struct ContractionMapping *cMap;

	cMap = malloc(sizeof(struct ContractionMapping));
	if (cMap == NULL){
		fprintf(stderr, "ERROR in graphContraction.c/newContractionMapping() : Unable to mallocate cMap\n");
		exit(ALLOC_ERR);
	}
	cMap->revBecomes = calloc(n, sizeof(idx_ *));
	if (cMap->revBecomes == NULL){
		fprintf(stderr, "ERROR in graphContraction.c/newContractionMapping() : Unable to callocate cMap->revBecomes of size %d\n", n);
		exit(ALLOC_ERR);
	}
	cMap->becomes = malloc(n * sizeof(idx_));
	if (cMap->becomes == NULL){
		fprintf(stderr, "ERROR in graphContraction.c/newContractionMapping() : Unable to mallocate cMap->becomes of size %d\n", n);
		exit(ALLOC_ERR);
	}
	cMap->Csize = calloc(n, sizeof(idx_));
	if (cMap->Csize == NULL){
		fprintf(stderr, "ERROR in graphContraction.c/newContractionMapping() : Unable to callocate cMap->Csize of size %d\n", n);
		exit(ALLOC_ERR);
	}
	cMap->n = n;
	cMap->na = n;
	cMap->n1 = 0;

	return cMap;
}

void becomesInitialization(struct Graph *G, struct ContractionMapping *cMap){
	idx_ i, j;  // vertex ids
	idx_ j_idx; // vertex index
	idx_ b;     // current best choice for becomes
	col_ c;     // colour of the current vertex

	for (i = 0; i < G->n; i++){
		c = G->C[i];
		b = i;
		for (j_idx = 0; j_idx < G->Nv[i]; j_idx++){
			j = G->E[i][j_idx];
			if (c == G->C[j] && j < b)  b = j;
		}
		cMap->becomes[i] = b;
	}

	return ;
}

void becomesUpdate(struct ContractionMapping *cMap){
	idx_ i; // vertex id

	for (i = 1; i < cMap->n; i++)  cMap->becomes[i] = cMap->becomes[cMap->becomes[i]];

	return ;
}

void evaluateClusterSize(struct ContractionMapping *cMap){
	idx_ i; // vertex id

	for (i = 0; i < cMap->n; i++) {  cMap->Csize[cMap->becomes[i]]++;  }

	for (i = 0; i < cMap->n; i++) {  cMap->n1 += (cMap->Csize[i] > 0);  }
	
	return ;
}

void extractReverseBecomesMapping(struct ContractionMapping *cMap){
	idx_ i, i1;  // vertex ids
	idx_ j_bdx;  // revbecomes index
	idx_ *Ibdxs; // counter for j_bdx of revBecomes[i1]
	
	Ibdxs = calloc(cMap->n, sizeof(idx_));
	if (Ibdxs == NULL){
		fprintf(stderr, "ERROR in graphContraction.c/extractReverseBecomesMapping() : Unable to callocate Ibdxs of size %d\n", cMap->n);
		exit(ALLOC_ERR);
	}

	for (i = 0; i < cMap->n; i++) {
		if (cMap->Csize[i] > 0) {
			cMap->revBecomes[i] = malloc(cMap->Csize[i] * sizeof(idx_));
			if (cMap->revBecomes[i] == NULL){
				fprintf(stderr, "ERROR in graphContraction.c/extractReverseBecomesMapping() : Unable to mallocate cMap->revBecomes[%d] of size %d\n", i, cMap->Csize[i]);
				exit(ALLOC_ERR);
			}
		} else {
			cMap->revBecomes[i] = NULL;
		}
	}

	for (i = 0; i < cMap->n; i++){
		i1 = cMap->becomes[i];
		j_bdx = Ibdxs[i1]++;
		cMap->revBecomes[i1][j_bdx] = i;
	}

	free(Ibdxs);

	return ;
}

void revBecomesCompacting(struct ContractionMapping *cMap){
	idx_ i, i1 = 0;     // vertex id, explicit assignment needed since i1 it is used as counter
	idx_ *Csize1;       // compact version of Csize
	idx_ **revBecomes1; // compact version of revBecomes

	Csize1 = calloc(cMap->n1, sizeof(idx_));
	if (Csize1 == NULL){
		fprintf(stderr, "ERROR in graphContraction.c/revBecomesCompacting() : Unable to callocate Csize1 of size %d\n", cMap->n1);
		exit(ALLOC_ERR);
	}

	revBecomes1 = calloc(cMap->n1, sizeof(idx_ *));
	if (revBecomes1 == NULL){
		fprintf(stderr, "ERROR in graphContraction.c/revBecomesCompacting() : Unable to callocate revBecomes1 of size %d\n", cMap->n1);
		exit(ALLOC_ERR);
	}

	for (i = 0; i < cMap->n; i++){
		if (cMap->revBecomes[i] != NULL){
			Csize1[i1] = cMap->Csize[i];
			revBecomes1[i1] = cMap->revBecomes[i];
			i1++;
		}
	}
	
	free(cMap->Csize);
	cMap->Csize = Csize1;

	// We only free revBecomes array since revBecomes[.] are still used in revBecomes1
	free(cMap->revBecomes);
	cMap->revBecomes = revBecomes1;

	cMap->na = cMap->n1;

	return ;
}

void becomesCompacting(struct ContractionMapping *cMap){
	idx_ i1;    // vertex id
	idx_ j_idx; // vertex index
	
	for (i1 = 0; i1 < cMap->n1; i1++)
		for (j_idx = 0; j_idx < cMap->Csize[i1]; j_idx++)
			cMap->becomes[cMap->revBecomes[i1][j_idx]] = i1;
	
	return ;
}

void edgesDestinationUpdate(struct Graph *G, struct ContractionMapping *cMap){
	idx_ i;    // vertex id
	idx_ j_idx; // vertex index

	for (i = 0; i < G->n; i++)
		for (j_idx = 0; j_idx < G->Nv[i]; j_idx++)
			G->E[i][j_idx] = cMap->becomes[G->E[i][j_idx]];

	return ;
}

void colorClusterMerge(struct Graph *G, struct ContractionMapping *cMap){
	idx_ a_idx;     // index of a
	idx_ i1, j, k1; // vertex ids
	idx_ j_bdx;     // revBecomes index
	idx_ k_idx;     // vertex index
	idx_ m1 = 0;    // order of G'
	idx_ d;         // temporary value for Nv1[i1]
	idx_ *Nv1;      // neighbourhood degrees Nv for G', of size n1
	idx_ **E1;      // adjacency list E for G', of size n1
	idx_ *e;        // temporary reference to E1[i1], ie sparse version of a
	bool *a;        // temporary dense n1-length bit-array for the adjacency list e

	Nv1 = calloc(cMap->n1, sizeof(idx_));
	if (Nv1 == NULL){
		fprintf(stderr, "ERROR in graphContraction.c/colorClusterMerge() : Unable to callocate Nv1 of size %d\n", cMap->n1);
		exit(ALLOC_ERR);
	}
	E1 = malloc(cMap->n1 * sizeof(idx_ *));
	if (E1 == NULL){
		fprintf(stderr, "ERROR in graphContraction.c/colorClusterMerge() : Unable to mallocate E1 of size %d\n", cMap->n1);
		exit(ALLOC_ERR);
	}
	a = malloc(cMap->n1 * sizeof(bool));
	if (a == NULL){
		fprintf(stderr, "ERROR in graphContraction.c/colorClusterMerge() : Unable to mallocate a of size %d\n", cMap->n1);
		exit(ALLOC_ERR);
	}

	for (i1 = 0; i1 < cMap->n1; i1++){
		// Reset a
		for (a_idx = 0; a_idx < cMap->n1; a_idx++)  a[a_idx] = false;
		d = 0;
		
		// Fill a & deallocate old adjacency vectors
		// cycle over revBecomes[i1] vertices j
		for (j_bdx = 0; j_bdx < cMap->Csize[i1]; j_bdx++){
			j = cMap->revBecomes[i1][j_bdx];
			// cycle over the neighbours of j
			for (k_idx = 0; k_idx < G->Nv[j]; k_idx++){
				k1 = G->E[j][k_idx];
				// condition on i1 can be dropped if self-loop are allowed
				if (i1 != k1 && !a[k1]){
					d++;
					a[k1] = true;
				}
			}
			free(G->E[j]);
		}

		// Convert from bit-array a to adjacency list e
		e = malloc(d * sizeof(idx_));
		if (e == NULL){
			fprintf(stderr, "ERROR in graphContraction.c/colorClusterMerge() : Unable to mallocate e of size %d\n", d);
			exit(ALLOC_ERR);
		}
		k_idx = 0;
		for (a_idx = 0; a_idx < cMap->n1; a_idx++)
			if (a[a_idx])
				e[k_idx++] = a_idx;

		Nv1[i1] = d;
		E1[i1] = e;
		m1 += d;
		
	}

	free(a);

	free(G->Nv);
	G->Nv = Nv1;

	free(G->E);
	G->E = E1;

	G->m = m1;

	return ;
}

void vertexContraction(struct Graph *G, struct ContractionMapping *cMap){
	idx_ i1;  // vertex id
	col_ *C1; // Colour array C for G', of size n1
	
	C1 = malloc(cMap->n1 * sizeof(col_));
	if (C1 == NULL){
		fprintf(stderr, "ERROR in graphContraction.c/vertexContraction() : Unable to mallocate C1 of size %d\n", cMap->n1);
		exit(ALLOC_ERR);
	}

	for (i1 = 0; i1 < cMap->n1; i1++)  C1[i1] = G->C[cMap->revBecomes[i1][0]];

	free(G->C);
	G->C = C1;

	G->n = cMap->n1;
}

struct ContractionMapping * evaluateContractionMapping(struct Graph *G){
	struct ContractionMapping *cMap;

	cMap = newContractionMapping(G->n);
	becomesInitialization(G, cMap);
	becomesUpdate(cMap);
	evaluateClusterSize(cMap);
	extractReverseBecomesMapping(cMap);
	revBecomesCompacting(cMap);
	becomesCompacting(cMap);

	return cMap;
}

void applyGraphContraction(struct Graph *G, struct ContractionMapping *cMap){
	
	edgesDestinationUpdate(G, cMap);
	colorClusterMerge(G, cMap);
	vertexContraction(G, cMap);

	return;
}

bool graphColorContraction(struct Graph *G){
	struct ContractionMapping *cMap;
	bool doContract;
	
	cMap = evaluateContractionMapping(G);

	doContract = (cMap->n != cMap->n1);
	if (doContract)  applyGraphContraction(G, cMap);

	freeContractionMapping(cMap);

	return doContract;
}

void displayContractionMapping(struct ContractionMapping *cMap){
	idx_ i, j;

	fprintf(stdout, "###################\n");
	fprintf(stdout, "Contraction Mapping\n");
	fprintf(stdout, "###################\n");
	fprintf(stdout, "n = %d, n1 = %d\n", cMap->n, cMap->n1);
	fprintf(stdout, "Csize:\n");
	for (i = 0; i < cMap->na; i++){
		fprintf(stdout, " - %d : %d\n", i, cMap->Csize[i]);
	}
	fprintf(stdout, "becomes:\n");
	for (i = 0; i < cMap->n; i++){
		fprintf(stdout, " - %d : %d\n", i, cMap->becomes[i]);
	}
	fprintf(stdout, "revbecomes:\n");
	for (i = 0; i < cMap->na; i++){
		fprintf(stdout, " - %d :", i);
		if (cMap->revBecomes[i] != NULL)
			for (j = 0; j < cMap->Csize[i]; j++)
				fprintf(stdout, " %d", cMap->revBecomes[i][j]);
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "###################\n");

	return;
}

void freeContractionMapping(struct ContractionMapping *cMap){
	
	free(cMap->becomes);
	free(cMap->Csize);
	
	for (int i = 0; i < cMap->na; i ++){
		if (cMap->revBecomes[i] != NULL)  free(cMap->revBecomes[i]);
	}
	free(cMap->revBecomes);
	
	return;
}