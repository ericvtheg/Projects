//Eric Ventor 
//Eventor
//1492157
//PA5
//12/5/2017
//Prof. Tantalo
//Graph Header Implementation

//link to dfs psuedo code:
//https://classes.soe.ucsc.edu/cmps101/Fall17/Notes/11-14-17.pdf

#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "List.h" 

#define WHITE 0  //undiscovered
#define GRAY  1  //inbetween
#define BLACK 2  //discovered

typedef struct GraphObj{
   int source; //source vertex
   int order;  //number of vertices
   int size;   //number of edges
   int time;
   int * parent;
   int * color;
   int * discover;
   int * finish;
   List * adj; //adjacent vertices
} GraphObj;

/* Constructors-Destructors */

/* newGraph() 
Graph Constructor */
Graph newGraph(int n){
	Graph G = malloc(sizeof(GraphObj));
	G->parent = malloc((n+1)*sizeof(int));
	G->color = malloc((n+1)*sizeof(int));
	G->discover = malloc((n+1)*sizeof(int));
	G->finish = malloc((n+1)*sizeof(int));
	G->adj = malloc((n+1)*sizeof(List));
	G->source = NIL;
	G->order = n;
	G->size = 0;
	G->time = 0; //should this be equal to 1?!!!!!
	
	for(int i = 1; i <= n; i ++){ 
		G->adj[i] = newList();
		G->parent[i] = NIL;
		G->color[i] = WHITE;
		G->discover[i] = UNDEF;	
		G->finish[i] = UNDEF;		
	}
	return G;
}  

/* freeGraph()
frees all memory associated with Graph using a pointer */
void freeGraph(Graph* pG){
	if(*pG == NULL){
		return;
	}
	//free each list
	for(int i = 1; i<= getOrder(*pG); i++){
		freeList(&((*pG)->adj[i])); 
	}
	
	//free memory being pointed to
	free((*pG)->parent);
	free((*pG)->color);
	free((*pG)->discover);
	free((*pG)->finish);
	free((*pG)->adj);

	//set pointers to null
	(*pG)->parent = NULL;
	(*pG)->color = NULL;
	(*pG)->finish = NULL;
	(*pG)->discover = NULL;
	(*pG)->adj = NULL; 

	free((*pG));
	(*pG) = NULL;

	return;
}

/*** helper functions***/

/* deQueue()
gets value of entry and deletes it from Graph */
int deQueue(List L){ //based on C Queue Handouts
	if( L == NULL){
		fprintf(stderr, "deQueue() ERROR: NULL List reference\n");
		exit(1);
	}
	
	moveTo(L,0); 
	int x = get(L);
	delete(L);

	return x;
}

/* sort()
uses insertion sort to append edges/arcs in Graph */
void sort(List L, int v){
	if(length(L) == 0){
		append(L, v);
		return;
	}
	
	for(int i = 0; i <=length(L)-1; i++){
		moveTo(L, i);
		if(get(L) > v){
			//printf("%d if statement hit v= %d \n",i, v );
			insertBefore(L, v);
			return;
		}
	}
	append(L, v);
	return;
}

/* Access functions */

/* getOrder()
returns number of vertices in graph */
int getOrder(Graph G){
	return G->order;
}
/* getSize()
returns number of edges of graph */
int getSize(Graph G){
	return G->size;
}

/* getParent()
returns Parent of entry
Pre: 1<=u<=n=getOrder(G) */
int getParent(Graph G, int u){
	if(1 > u || u > getOrder(G)){
		fprintf(stderr, "getParent() ERROR: u is out of bounds\n");
		exit(1);
	}
	return G->parent[u];
}

/* getDiscover()
returns Discover time of entry 
Pre: 1<=u<=n=getOrder(G) */
int getDiscover(Graph G, int u){
	if(1 > u || u > getOrder(G)){
		fprintf(stderr, "getDiscover() ERROR: u is out of bounds\n");
		exit(1);
	}
	return G->discover[u];
}

 /* getFinish()
returns Finish time of entry 
Pre: 1<=u<=n=getOrder(G) */
int getFinish(Graph G, int u){
	if(1 > u || u > getOrder(G)){
		fprintf(stderr, "getFinish() ERROR: u is out of bounds\n");
		exit(1);
	}
	return G->finish[u];
}

/* Manipulation procedures */

/* addArc()
uses insertionSort to add arcs to graph 
Pre: 1<=u<=n, 1<=v<=n */
void addArc(Graph G, int u, int v){	
	if(1 > u || u > getOrder(G)){
		fprintf(stderr, "addEdge() ERROR: u is out of bounds\n"); //clarify what u is
		exit(1);
	}else if(1 > v || v > getOrder(G)){
		fprintf(stderr, "addEdge() ERROR: v is out of bounds\n");
	   	exit(1);
	}
	sort(G->adj[u], v);

	G->size++;
}

/* addEdge()
uses insertionSort to add edges to graph 
Pre: 1<=u<=n, 1<=v<=n */
void addEdge(Graph G, int u, int v){
	if(1 > u || u > getOrder(G)){
		fprintf(stderr, "addEdge() ERROR: u is out of bounds\n"); //clarify what u is
		exit(1);
	}else if(1 > v || v > getOrder(G)){
		fprintf(stderr, "addEdge() ERROR: v is out of bounds\n");
	   	exit(1);
	}
	sort(G->adj[u], v);
	sort(G->adj[v], u);

	G->size++;
}
/* visit()
Sub-function of DFS goes through 
'discovering' each vertex */
void visit(Graph G, List S, int i){
	G->color[i] = GRAY; //discovered
	G->discover[i] = (++(G->time));
	
	for(moveFront(G->adj[i]); index(G->adj[i])>=0; moveNext((G->adj[i]))){
		int y = get(G->adj[i]);
		if(G->color[y] == WHITE){
			G->parent[y] = i;
			visit(G, S, y);
		}
	}
	G->color[i] = BLACK; //finished
	G->finish[i] = (++(G->time));
	prepend(S, i);
}

/* DFS()
uses DFS algorithim to traverse graph 
Pre: length(S)==getOrder(G) */
void DFS(Graph G, List S){
	if(length(S)!=getOrder(G)){
		fprintf(stderr, "DFS() ERROR: length of list does not equal # of vertices\n");
		exit(1);
	}
	for(int i = 1; i <= getOrder(G); i++){
		G->color[i] = WHITE;
		G->parent[i]= NIL;
		G->discover[i] = UNDEF;
		G->finish[i] = UNDEF;
	}
	
	List cop = copyList(S);
	clear(S);
	
	G->time = 0;
	moveFront(cop);
	
	for(int i = 1; index(cop) >= 0; moveTo(cop, i++)){ //chnage this loop
		int y = get(cop);
		if(G->color[y] == WHITE){
			visit(G, S, y);
			moveNext(cop);
		}
	}
	freeList(&cop);
}		

/* Other Functions */

/* transpose()
returns new graph that is the transposed of G */
Graph transpose(Graph G){
	Graph trans = newGraph(getOrder(G));
	for(int i =1; i <= getSize(G); i++){ //indice
		//fprintf(stderr, "hit %d", i);
		for(int j = 1; j <= getOrder(G);j++){ //go to each list
			moveFront(G->adj[j]);
			while(0 <= index(G->adj[j])){ //goes inside each list and compares each entry to indice
				int y = get(G->adj[j]);
				if(i == y){
					//append(trans->adj[i], j);
					addArc(trans, y, j);
				}
				moveNext(G->adj[j]);
			}
		}
	}
	trans->size = G->size;
	return trans;
}

/* copyGraph()
returns new graph that is the same as G except for 
values aren't initialized i.e. discover, finish, etc. */
Graph copyGraph(Graph G){
	Graph copy = newGraph(getOrder(G));
	for(int i =1; i <= getSize(G); i++){ //indice
		//fprintf(stderr, "hit %d", i);
		for(int j = 1; j <= getOrder(G);j++){ //go through each list
			moveFront(G->adj[j]);
			while(0 <= index(G->adj[j])){ //goes inside each list
				int y = get(G->adj[j]);
				if(i == y){
					//append(copy->adj[j], i);
					addArc(copy, j, y);
				}
				moveNext(G->adj[j]);
			}
		}
	}
	copy->size = G->size;
	return copy;
}

/* printGraph()
prints graph */
void printGraph(FILE* out, Graph G){
	for(int i = 1; i <= getOrder(G); i++){
		fprintf(out, "%d: ",i);
		printList(out, G->adj[i]);
		fprintf(out, "\n"); 
	}
}

