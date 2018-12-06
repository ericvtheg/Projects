//-----------------------------------------------------------------------------
//FindComponents.c
//Reads in, Performs Graph operations, Finds SCC, Prints out
//-----------------------------------------------------------------------------
//Eric Ventor 
//Eventor
//1492157
//PA5
//12/5/2017
//Prof. Tantalo

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Graph.h"
#include "List.h" 

int main(int argc, char * argv[]){
	FILE* in;
	FILE* out;
	bool flag = true;
	bool stop = true;
	int v = 0, g = 0, n = 0, strong = 0, temp = 0;
	List S = newList();
	List SCC = newList();
	
	//open in file for reading
	in = fopen(argv[1], "r");  
	//open out file for writing
	out = fopen(argv[2], "w");

	if(argc != 3){
		fprintf(stderr, "ERROR: incorrect number of args");
		exit(1);
	}
	if(in == NULL){
		fprintf(stderr, "ERROR: input file does not exist");
		exit(1);
	}
	
	Graph G;

	//Read the input file.
	//Assemble a graph object G using newGraph() and addArc()
	while(stop){
		if(flag){
			fscanf(in, "%d", &n);
			G = newGraph(n);
			flag = false;
		}
		fscanf(in, "%d", &v);
		fscanf(in, "%d", &g);
		if(v == 0 && g == 0){
			stop = false;
		}else{
			addArc(G, v, g);
		}
	}
	//Print the adjacency list representation of G to the output file
	fprintf(out, "Adjacency list representation of G:\n");
	printGraph(out, G);
	
	for(int i =1; i <= getOrder(G); i++){
		append(S, i);
	}
	
	//printList(out, S);
	fprintf(out, "\n");
	
	//Run DFS on G
	DFS(G, S);
	
	//fprintf(stdout, "x:  d  f  p\n");
	
	//Get transpose of G
	//fprintf(stdout, "\n");
	Graph T = transpose(G);
	
	//processing the vertices in the second call by decreasing finish times from the first call
	DFS(T, S);
	
	//fprintf(stdout, "x:  d  f  p\n");
	
	//printGraph(stdout, T);
	
	//finding strongly connected components
	for(moveFront(S); index(S)>=0; moveNext(S)){
		if(getParent(T, get(S)) == NIL){
			strong++;
		}
	}
	
	fprintf(out, "G contains %d strongly connected components:\n", strong);
	
	moveBack(S);
	int i = 1;
	while(index(S)>= 0){
		
		temp = get(S);
		prepend(SCC, temp);
		
		if(getParent(T, temp) == 0){
			fprintf(out, "Component %d: ", i);
			printList(out, SCC);
			fprintf(out, "\n");
			clear(SCC);
			i++;
		}
		movePrev(S);
	}

		
	freeGraph(&G);
	freeGraph(&T);
	freeList(&S);
	freeList(&SCC);
	fclose(in);
	fclose(out);
	return(0);
}