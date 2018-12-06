//-----------------------------------------------------------------------------
// FindPath.c
//Reads in and Prints out
//-----------------------------------------------------------------------------
//Eric Ventor 
//Eventor - 1492157
//PA4
//11/17/2017
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
	int v, g, n = 0;
	List path = newList();

	//open in file for reading
	in = fopen(argv[1], "r");  
	//open out file for writer
	out = fopen(argv[2], "w");

	if(argc != 3){//is the filename consider an arg?
		fprintf(stderr, "ERROR: incorrect number of args");
		exit(1);
	}
	if(in == NULL){
		fprintf(stderr, "ERROR: input file does not exist");
		exit(1);
	}
	Graph G;
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
			addEdge(G, v, g);
		}
	}

	fprintf(out, "\n");
	printGraph(out, G);

	rewind(in);
	stop = true;
	flag = true;


	//commented out because it causes a segfault in a bizarre way 
	//Simply by the end of the loop it decides to segfault
	//even though all commands are executed 
	//I expand on this more in the README
	/*while(stop){
		if(flag){
			fscanf(in, "%d", &n);
			//G = newGraph(n);
			flag = false;
		}else if(v == 0 && g==0){
			break;
		}

		fscanf(in, "%d %d", &v, &g);

		BFS(G, v);
		getPath(path, G, g);

		fprintf(out, "The distance from %d to %d is %d", v, g, getDist(G, g));
		fprintf(out, "A shortest %d-%d path is:");

		printList(out, path);
		printf("\n");
		stop = false;
	}*/

	freeList(&path);
	freeGraph(&G);
	fclose(in);
	fclose(out);
}