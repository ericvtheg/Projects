//-----------------------------------------------------------------------------
// Graph.c
// Implementation file for 
//-----------------------------------------------------------------------------
//Eric Ventor 
//Eventor - 1492157
//PA4
//11/17/2017
//Prof. Tantalo
//Graph Header File


typedef struct VertexObj{
   struct VertexObj* prev;
   struct VertexObj* next;
   struct VertexObj* cursor;
   int data;
} VertexObj;

// private Node type 
typedef VertexObj * Node;

/*** Constructors-Destructors ***/
//returns a Graph pointing to a newly created GraphObj representing a graph having n vertices and no edges
Graph newGraph(int n){
	
}  

//frees all dynamic memory associated with the Graph *pG, then sets the handle *pG to NULL
void freeGraph(Graph* pG){
	
}

/*** Access functions ***/

int getOrder(Graph G){
	
}

int getSize(Graph G){
	
}

int getSource(Graph G){
	
}

//precondition 1 <= u <= getOrder(G)
//return the parent of vertex u in the BreadthFirst tree created by BFS(), or NIL if BFS() has not yet been called
int getParent(Graph G, int u){
	
} 

//precondition 1 <= u <= getOrder(G)
//returns the distance from the most recent BFS source to vertex u, or INF if BFS() has not yet been called
int getDist(Graph G, int u){
	
}

//pre: getSource(G)!=NIL, so BFS() mustbe called before getPath().
//precondition 1 <= u <= getOrder(G)
//appends to the List L the vertices of a shortest path in G from source to u, or appends to L thevalue NIL if no such path exists
void getPath(List L, Graph G, int u){
	
}

/*** Manipulation procedures ***/

//deletes all edges of G, restoring it to its original(no edge) state
void makeNull(Graph G){
	
}

// precondition that their two int arguments must lie in the range 1 to getOrder(G)
void addEdge(Graph G, int u, int v){
	
}

// precondition that their two int arguments must lie in the range 1 to getOrder(G)
void addArc(Graph G, int u, int v){
	
}
//runs the BFS algorithm on the Graph G with source s, setting the color, distance, parent, and source fields of G accordingly
void BFS(Graph G, int s){
	
}

/*** Other operations ***/
//prints the adjacency list representation of G to the file pointed to by out
void printGraph(FILE* out, Graph G){
	
}