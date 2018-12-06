//------------------------------------------------------------------------------
// List.h
// Header file for 
//------------------------------------------------------------------------------
//Eric Ventor 
//Eventor - 1492157
//PA4
//11/17/2017
//Prof. Tantalo
//Graph Header File

#include "List.h"
#define NIL 0
#define INF -1

/*In addition to the above prototypes Graph.h will define the type Graph as well as #define constant
macros INF and NIL that represent infinity and an undefined vertex label, respectively. For the purpose of
implementing BFS, any negative int value is an adequate choice for INF, and any non-positive int can
stand in for NIL, since all valid vertex labels will be positive integers. INF and NIL should of course be
different integers.
*/

typedef struct GraphObj* Graph;

/*** Constructors-Destructors ***/
Graph newGraph(int n); //returns a Graph pointing to a newly created GraphObj representing a graph having n vertices and no edges 

void freeGraph(Graph* pG); //frees all dynamic memory associated with the Graph *pG, then sets the handle *pG to NULL

/*** Access functions ***/
int getOrder(Graph G); //returns order

int getSize(Graph G); //returns size

int getSource(Graph G); //returns Source

//precondition 1 <= u <= getOrder(G)
int getParent(Graph G, int u); //return the parent of vertex u in the BreadthFirst tree created by BFS(), or NIL if BFS() has not yet been called

//precondition 1 <= u <= getOrder(G)
int getDist(Graph G, int u); //returns the distance from the most recent BFS source to vertex u, or INF if BFS() has not yet been called


//pre: getSource(G)!=NIL, so BFS() mustbe called before getPath().
//precondition 1 <= u <= getOrder(G)
void getPath(List L, Graph G, int u); //appends to the List L the vertices of a shortest path in G from source to u, or appends to L thevalue NIL if no such path exists

/*** Manipulation procedures ***/
void makeNull(Graph G);  //deletes all edges of G, restoring it to its original(no edge) state

// precondition that their two int arguments must lie in the range 1 to getOrder(G)
void addEdge(Graph G, int u, int v);

// precondition that their two int arguments must lie in the range 1 to getOrder(G)
void addArc(Graph G, int u, int v);

void BFS(Graph G, int s); //runs the BFS algorithm on the Graph G with source s, setting the color, distance, parent, and source fields of G accordingly

/*** Other operations ***/
void printGraph(FILE* out, Graph G); //prints the adjacency list representation of G to the file pointed to by out
