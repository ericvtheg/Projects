//Eric Ventor 
//Eventor - 1492157
//PA5
//12/5/2017
//Prof. Tantalo
//Graph Header File

#include <stdio.h>
#include <stdlib.h>

#include "List.h"

#define NIL 0
#define INF -1
#define UNDEF -2

/* Constructors-Destructors */

typedef struct GraphObj* Graph;

Graph newGraph(int n);

void freeGraph(Graph* pG);

/* Access functions */

int getOrder(Graph G);

int getSize(Graph G);

int getParent(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */

int getDiscover(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */

int getFinish(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */

/* Manipulation procedures */

void addArc(Graph G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */

void addEdge(Graph G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */

void DFS(Graph G, List S); /* Pre: length(S)==getOrder(G) */

/* Other Functions */

Graph transpose(Graph G);

Graph copyGraph(Graph G);

void printGraph(FILE* out , Graph G);
