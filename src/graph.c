#include "graph.h"

struct Graph * newGraph(idx_ n){
	struct Graph *G;

	G = malloc(sizeof(struct Graph));
	if (G == NULL){
		fprintf(stderr, "ERROR in graph.c/newGraph() : Unable to mallocate G\n");
		exit(ALLOC_ERR);
	}
	G->n = n;
	G->m = 0;
	G->C = calloc(n, sizeof(col_));
	if (G->C == NULL){
		fprintf(stderr, "ERROR in graph.c/newGraph() : Unable to callocate G->C of size %d\n", n);
		exit(ALLOC_ERR);
	}
	G->Nv = calloc(n, sizeof(idx_));
	if (G->Nv == NULL){
		fprintf(stderr, "ERROR in graph.c/newGraph() : Unable to callocate G->Nv of size %d\n", n);
		exit(ALLOC_ERR);
	}
	G->E = malloc(n * sizeof(idx_ *));
	if (G->E == NULL){
		fprintf(stderr, "ERROR in graph.c/newGraph() : Unable to mallocate G->E of size %d\n", n);
		exit(ALLOC_ERR);
	}

	return G;
}

struct Graph * readGraph(char *filename){
	idx_ i, j, n;
	struct Graph *G;
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL){
		fprintf(stderr, "ERROR in graph.c/readGraph() : fopen(filename, \"r\")\n");
		exit(FOPEN_ERR);
	}

	if ( fscanf(fp, "N : %d\nC : ", &n) != 1 ){
		fprintf(stderr, "ERROR in graph.c/readGraph() : fscanf(fp, d, &n)\n");
		exit(SCANF_ERR);
	}
	G = newGraph(n);

	for (i = 0; i < G->n; i++){
		if ( fscanf(fp, "%d", &(G->C[i])) != 1 ){
			fprintf(stderr, "ERROR in graph.c/readGraph() : fscanf(fp, d, &(G->V[i]->w->color))\n");
			exit(SCANF_ERR);
		}
	}

	
	for (i = 0; i < G->n; i++){
		if ( fscanf(fp, "%d : ", &(G->Nv[i])) != 1 ){
			fprintf(stderr, "ERROR in graph.c/readGraph() : fscanf(fp, d, &(G->Nv[i]))\n");
			exit(SCANF_ERR);
		}
		G->E[i] = malloc(G->Nv[i] * sizeof(idx_));
		if (G->E[i] == NULL){
			fprintf(stderr, "ERROR in graph.c/readGraph() : Unable to mallocate G->E[%d] of size %d\n", i, G->Nv[i]);
			exit(ALLOC_ERR);
		}
		for (j = 0; j < G->Nv[i]; j++){
			if ( fscanf(fp, "%d", &(G->E[i][j])) != 1 ){
				fprintf(stderr, "ERROR in graph.c/readGraph() : fscanf(fp, d, &(G->E[%d][%d]))\n", i, j);
				exit(SCANF_ERR);
			}
		}
		G->m += G->Nv[i];
	}
	
	fclose(fp);

	return G;
}

struct Graph * readIGraph(char *filename){
	idx_ n, i, j, id, deg, k_idx, a_idx;
	struct Graph *G;
	FILE *fp;
	bool *a, endl, found;
	char tmp;

	// evaluate N
	fp = fopen(filename, "r");
	if (fp == NULL){
		fprintf(stderr, "ERROR in graph.c/readIGraph() : first fopen(filename, \"r\")\n");
		exit(FOPEN_ERR);
	}
	n = 0;
	for (tmp = getc(fp); tmp != EOF; tmp = getc(fp))  if( tmp == '\n' )  n++;
	fclose(fp);

	G = newGraph( n );

	// evaluate adjacency list
	fp = fopen(filename, "r");
	if (fp == NULL){
		fprintf(stderr, "ERROR in graph.c/readIGraph() : second fopen(filename, \"r\")\n");
		exit(FOPEN_ERR);
	}
	a = malloc(n * sizeof(bool));
	if (a == NULL){
		fprintf(stdout, "ERROR in graph.c/readIGraph(): unable to mallocate a of size %d", n);
		exit(ALLOC_ERR);
	}

	for (i = 0; i < n; i++){
		//clear bitarray
		for (j = 0; j < n; j++)
			a[j] = false;
		//read new adjancy row and evaluate degree       
		id = 0;
		deg = 0;
		endl = false;
		found = false;
		while( !endl ){
			tmp = getc(fp);
			if (tmp == ' '){
				a[id] = true;
				deg++;
				id = 0;
			} else if (tmp == '\n' || tmp == EOF){
				if (found){
					a[id] = true;
					deg++;
				}
				endl = true;
			} else {
				id = id*10+(tmp-'0');
				found = true;
			}
		}
		// write adjacency row
		G->E[i] = malloc(deg * sizeof(idx_));
		if (G->E[i] == NULL){
			fprintf(stderr, "ERROR in graph.c/readIGraph() : Unable to mallocate G->E[%d] of size %d\n", i, deg);
			exit(ALLOC_ERR);
		}
		G->Nv[i] = deg;
		G->m += deg;
		k_idx = 0;
		for (a_idx = 0; a_idx < n; a_idx++)
			if (a[a_idx])
				G->E[i][k_idx++] = a_idx;
	}

	fclose(fp);
	free(a);

	return G;
}

void exportIGraph(char *filename, struct Graph *G){
	idx_ i, j;
	FILE *fp;
	fp = fopen(filename, "w");
	if (fp == NULL){
		fprintf(stderr, "ERROR in graph.c/exportIGraph() : fopen(filename, \"w\")\n");
		exit(FOPEN_ERR);
	}

	for (i = 0; i < G->n; i++){
		for (j = 0; j < G->Nv[i]; j++){
			if (j > 0)  fprintf(fp, " ");
			fprintf(fp, "%d", G->E[i][j]);
		}
		fprintf(fp, "\n");
	}
	
	fclose(fp);

	return ;
}

void writeGraph(char *filename, struct Graph *G){
	idx_ i, j;
	FILE *fp;
	fp = fopen(filename, "w");
	if (fp == NULL){
		fprintf(stderr, "ERROR in graph.c/writeGraph() : fopen(filename, \"w\")\n");
		exit(FOPEN_ERR);
	}

	fprintf(fp, "N : %d\nC :", G->n);

	for (i = 0; i < G->n; i++){
		fprintf(fp, " %d", G->C[i]);
	}

	
	for (i = 0; i < G->n; i++){
		fprintf(fp, "\n\t%d :", G->Nv[i]);
		for (j = 0; j < G->Nv[i]; j++){
			fprintf(fp, " %d", G->E[i][j]);
		}
	}
	
	fclose(fp);

	return ;
}

/**
 * @brief export on `filename` the adjacency matrix with colors.
 * 
 * Colors are reported as the first line of the adjacecy matrix.
 * 
 * @param filename 
 * @param G 
 */
void writeAdjacencyMatrix(char *filename, struct Graph *G){
	idx_ i, j, k;
	FILE *fp;
	fp = fopen(filename, "w");
	if (fp == NULL){
		fprintf(stderr, "ERROR in graph.c/writeAdjacencyMatrix() : fopen(filename, \"w\")\n");
		exit(FOPEN_ERR);
	}

	for (i = 0; i < G->n; i++){
		fprintf(fp, "%3d ", G->C[i]);
	}

	
	for (i = 0; i < G->n; i++){
		fprintf(fp, "\n");
		for (j = 0; j < G->n; j++){
			int f = 0;
			for (k = 0; k < G->Nv[i]; k++){
				if (G->E[i][k] == j){
					f = 1;
				}
			}
			fprintf(fp, "%3d ", f);
		}
		
	}
	
	fclose(fp);

	return ;
}

void displayGraph(struct Graph *G){
	idx_ i, j;
	fprintf(stdout, "Graph(%d, %d):\n", G->n, G->m);
	for (i = 0; i < G->n; i++){
		fprintf(stdout, " - %d (col = %d) {Nv = %d} :", i, G->C[i], G->Nv[i]);
		for (j = 0; j < G->Nv[i]; j++){
			fprintf(stdout, " %d", G->E[i][j]);
		}
		fprintf(stdout, "\n");
	}
}

void freeGraph(struct Graph *G){
	for (idx_ i = 0; i < G->n; i++){
		free(G->E[i]);
	}
	free(G->C);
	free(G->E);
	free(G->Nv);
	free(G);
}

struct Graph * ErdosRenyi(idx_ n, float p){
	struct Graph *G;
	bool **bitarray;

	G = newGraph(n);

	if (p == 0){
		for (idx_ i = 0; i < n; i++){
			G->E[i] = calloc(0, sizeof(idx_));
			if (G->E[i] == NULL){
				fprintf(stderr, "ERROR in graph.c/ErdosRenyi() : Unable to callocate G->E[%d] of size 0\n", i);
				exit(ALLOC_ERR);
			}
		}
		return G;
	}

	bitarray = calloc(n, sizeof(bool *));
	if (bitarray == NULL){
		fprintf(stderr, "ERROR in graph.c/ErdosRenyi() : Unable to callocate bitarray of size %d\n", n);
		exit(ALLOC_ERR);
	}
	for (idx_ i = 0; i < n; i++){
		bitarray[i] = calloc(n, sizeof(bool));
		if (bitarray[i] == NULL){
			fprintf(stderr, "ERROR in graph.c/ErdosRenyi() : Unable to callocate bitarray[%d] of size %d\n", i, n);
			exit(ALLOC_ERR);
		}
	}

	for (idx_ i = 0; i < n; i++){
		for (idx_ j = i+1; j < n; j++){
			if (drand48() <= p){
				bitarray[i][j] = true;
				bitarray[j][i] = true;
				G->Nv[i] ++;
				G->Nv[j] ++;
			} else {
				bitarray[i][j] = false;
				bitarray[j][i] = false;
			}
		}
		G->E[i] = calloc(G->Nv[i], sizeof(idx_));
		if (G->E[i] == NULL){
			fprintf(stderr, "ERROR in graph.c/ErdosRenyi() : Unable to callocate G->E[%d] of size %d\n", i, G->Nv[i]);
			exit(ALLOC_ERR);
		}
		G->m += G->Nv[i];
		idx_ k = 0;
		for (idx_ j = 0; j < n; j++){
			if (bitarray[i][j]){
				G->E[i][k++] = j;
			}
		}
	}

	free(bitarray);

	return G;
}

// Assegna una colorazione random al grafo `G'
// prendendo i colori in [0,c)
void assignRandomColoring(struct Graph *G, int c){
	for (idx_ i = 0; i < G->n; i++){
		G->C[i] = lrand48()%c;
	}

	return ;
}