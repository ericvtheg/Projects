//-----------------------------------------------------------------------------
// Graph.c
// Implementation file for 
//-----------------------------------------------------------------------------
//Eric Ventor 
//Eventor - 1492157
//PA4
//11/17/2017
//Prof. Tantalo
//Graph Header Implementation

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
   int * parent;
   int * color;
   int * distance;
   List * adj; 
} GraphObj;

/*** Constructors-Destructors ***/
//returns a Graph pointing to a newly created GraphObj representing a graph having n vertices and no edges
Graph newGraph(int n){
	Graph G = malloc(sizeof(GraphObj));
	G->parent = malloc((n+1)*sizeof(int));
	G->color = malloc((n+1)*sizeof(int));
	G->distance = malloc((n+1)*sizeof(int));
	G->adj = malloc((n+1)*sizeof(List));
	G->source = NIL;
	G->order = n;
	G->size = 0;
	for(int i = 1; i <= n; i ++){ //should equal sign be there...yes
		G->adj[i] = newList();
		G->parent[i] = NIL;
		G->color[i] = WHITE;
		G->distance[i] = INF;
	}
	return G;
}  

//frees all dynamic memory associated with the Graph *pG, then sets the handle *pG to NULL
void freeGraph(Graph* pG){
	if(*pG == NULL){
		return;
	}
	//free each list
	for(int i = 1; i<= getOrder(*pG); i++){
		freeList(&((*pG)->adj[i])); //do i need an * in the parathensis
	}

	free((*pG)->parent);
	free((*pG)->color);
	free((*pG)->distance);
	free((*pG)->adj);

	(*pG)->parent = NULL;
	(*pG)->color = NULL;
	(*pG)->distance = NULL;
	//(*pG)->adj = NULL; 

	free((*pG));
	(*pG) = NULL;

	return;
}

/*** Access functions ***/

int getOrder(Graph G){
	return G->order;
}

int getSize(Graph G){
	return G->size;
}

int getSource(Graph G){
	return G->source;
}

//precondition 1 <= u <= getOrder(G)
//return the parent of vertex u in the BreadthFirst tree created by BFS(), or NIL if BFS() has not yet been called
int getParent(Graph G, int u){
	//check if BFS has been called
	if(1 > u || u > getOrder(G)){
		fprintf(stderr, "getParent() ERROR: u is out of bounds\n");
		exit(1);
	}
	return G->parent[u];

} 

//precondition 1 <= u <= getOrder(G)
//returns the distance from the most recent BFS source to vertex u, or INF if BFS() has not yet been called
int getDist(Graph G, int u){
	if(1 > u || u > getOrder(G)){
		fprintf(stderr, "getDist() ERROR: u is out of bounds\n");
		exit(1);
	}
	if(getSource(G) == NIL){
		return INF;
	}
	return G->distance[u];
}

//pre: getSource(G)!=NIL, so BFS() mustbe called before getPath().
//precondition 1 <= u <= getOrder(G)
//appends to the List L the vertices of a shortest path in G from source to u, or appends to L thevalue NIL if no such path exists
void getPath(List L, Graph G, int u){
	if(1 > u || u > getOrder(G)){
		fprintf(stderr, "getPath() ERROR: u is out of bounds\n");
		exit(1);
	}
	if(getSource(G) == NIL){
		fprintf(stderr, "getPath() ERROR: BFS must be called before\n");
		exit(1);
	}
	if(u == G->source){
		append(L, u);
	}else if(G->adj[u] == NIL){
		append(L, u);
	}else{
		getPath(L, G, (G->adj[u]));
		append(L, u);
	}
}

/*** Manipulation procedures ***/

//deletes all edges of G, restoring it to its original(no edge) state
void makeNull(Graph G){ //am i supposed to leave the source vertice?
	for(int i = 1; i <= getOrder(G); i++){
		clear(G->adj[i]);
	}
	G->size = 0;
}

// precondition that their two int arguments must lie in the range 1 to getOrder(G)
void addEdge(Graph G, int u, int v){
	if(1 > u || u > getOrder(G)){
		fprintf(stderr, "addEdge() ERROR: u is out of bounds\n"); //clarify what u is
		exit(1);
	}
	else if(1 > v || v > getOrder(G)){
		fprintf(stderr, "addEdge() ERROR: v is out of bounds\n");
	   	exit(1);
	}

	sort(G->adj[u], v);
	sort(G->adj[v], u);

	G->size++;

}

// precondition that their two int arguments must lie in the range 1 to getOrder(G)
//not necessary for pa4
void addArc(Graph G, int u, int v){	
	if(1 > u || u > getOrder(G)){
		fprintf(stderr, "addEdge() ERROR: u is out of bounds\n"); //clarify what u is
		exit(1);
	}
	else if(1 > v || v > getOrder(G)){
		fprintf(stderr, "addEdge() ERROR: v is out of bounds\n");
	   	exit(1);
	}

	sort(G->adj[u], v);

	G->size++;
}

//runs the BFS algorithm on the Graph G with source s, setting the color, distance, parent, and source fields of G accordingly
void BFS(Graph G, int s){
	for(int i = 1; i <= getOrder(G); i++){
		G->color[i] = WHITE;
		G->distance[i] = INF;
		G->adj[i] = NIL;
	}
	G->color[s] = GRAY;
	G->distance[s] = 0;
	G->adj[s] = NIL;
	G->source = s;
	List Q = newList();
	append(Q, s);
	while(0 < length(Q)){
		int x = deQueue(Q);
		List tmp = G->adj[x];
		for(int j = 0; j <= length(tmp); j++){
			moveTo(Q, j);
			int y = get(tmp);
			if(G->color[y] == WHITE){
				G->color[y] = GRAY; //!!! do I need to add j by 1 since it starts at 0
				G->distance[y] = G->distance[x]+1;
				G->adj[y] = x;
				append(Q, y); 
			}
		}
		G->color[x] = BLACK;
	}
	//Don't forget to free Q before function returns from piazza post keyword "implement Queue"
	freeList(&Q);
}

/*** Other operations ***/
//prints the adjacency list representation of G to the file pointed to by out
void printGraph(FILE* out, Graph G){
	
	//printf("%d", getOrder(G));
	for(int i = 1; i <= getOrder(G); i++){
		//printf("HITTT before first print\n");
		fprintf(out, "%d: ",i);
		//printf("HITTT second first print\n");
		printList(out, G->adj[i]);
		//printf("HITTT third first print\n");
		fprintf(out, " \n"); //this new line may not be necessary
		//getting VERY weird segfault error right when loop finishes here
	}
}

/*** helper functions***/
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