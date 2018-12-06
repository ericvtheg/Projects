//Eric Ventor
//1492157
//PA2
//CS101 
//10/18/2017
/* Lex checks for preconditions, 
* counts lines, 
* copies words into null terminated char array,
* uses insertion sorts along with List methods to sort,
* and finally prints in alpha order to output file
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "List.h"

#define MAX_LEN 160

int main(int argc, char * argv[]){
   FILE* in;
   FILE* out;
   int j;
   int lineCount = 0;
   char line[MAX_LEN];
   
   
   //open in file for reading
   in = fopen(argv[1], "r");  
   out = fopen(argv[2], "w");
   
   //preconditions
   if(argc != 3){ //update error message to read input file name
      fprintf(stderr, "ERROR: incorrect number of args"); 
	  exit(1);
   }
   if(in == NULL){ //update error message
	   fprintf(stderr, "ERROR: input file is NULL");
	   exit(1);
   }
   if(out == NULL){
      fprintf(stderr, "ERROR: incorrect number of args");
   }
   
   //count lines
   while(fgets(line, MAX_LEN, in) != NULL) {
      lineCount++;
   }
   //rewinds file pointer
   rewind(in);
   
   char words[lineCount][MAX_LEN];
   int c = 0;

   //uses line as a buffer and copies into words[c] til the EOF
   while(fgets(line, MAX_LEN, in) != NULL){
      strcpy(words[c], line );
		c++;
 	}
	
   //Declare and initialize list
   List Z = newList();
   
   prepend(Z, 0);
   //insertion sort
   for(int i = 1; j < lineCount; i++){
      char* key = words[i];
      j = i-1;
      moveTo(Z, j);
      while((j > -1) && strcmp(key, words[get(Z)]) < 0){
         movePrev(Z);
		 j--;
	  }
	  if(index(Z) == -1){
         prepend(Z, i);
	  }else{
	     insertAfter(Z, i);
	  }
   }
   //Prints from head of list til NULL by fetching List values with get()
   for(moveFront(Z); index(Z) != -1; moveNext(Z)){
		 fprintf(out, "%s", words[get(Z)]);
   }
   
   //freeList memory
   freeList(&Z);
   
   //close and free file streams
   fclose(in);
   fclose(out);
   
   
}