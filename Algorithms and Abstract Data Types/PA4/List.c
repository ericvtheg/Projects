//-----------------------------------------------------------------------------
// List.c
// Implementation file for Queue ADT
//-----------------------------------------------------------------------------
//Eric Ventor
//1492157
//PA4
//CS101 
//11/17/2017

#include <stdio.h>
#include <stdlib.h>
#include "List.h"

// structs --------------------------------------------------------------------

// private NodeObj type
typedef struct NodeObj{
   struct NodeObj* prev;
   struct NodeObj* next;
   struct NodeObj* cursor;
   int data;
   int index;
} NodeObj;

// private Node type 
typedef NodeObj * Node;

// private QueueObj type
typedef struct ListObj{
   Node head;
   Node tail;
   Node cursor;
   int index;
   int length;
} ListObj;

// Constructors-Destructors ---------------------------------------------------

//newList()
//Returns reference to new empty List object
List newList(void){
   List Eric = malloc(sizeof(ListObj));
   Eric->head = NULL;
   Eric->tail = NULL;
   Eric->cursor = NULL;
   Eric->index = -1;
   Eric->length = 0;
   return Eric;
}
//newNode()
//returns Node after allocating memory and setting points to NULL
Node newNode(int data){
   Node N = malloc(sizeof(NodeObj));
   N->data = data;
   N->prev = NULL;
   N->next = NULL;
   return(N);
}

//freeList()
//Frees all heap memory associated with List *L & sets *pQ to Null
void freeList(List* pL){//what exactly is being parsed in?
   if(*pL != NULL && pL !=  NULL){
      while(length(*pL)!=0){ //maybe change parathenses
		  deleteBack(*pL); //does this work?
	  }
   free(*pL);
   *pL = NULL;
   }
}


// Access functions -----------------------------------------------------------
void moveTo(List L, int m){
   if(m < 0 || m > L->length-1){ //should it be length-1
      printf("ERROR: moveTo() index out of bounds\n");
	  L->cursor = NULL;
	  L->index = -1;
   }else{
      L->cursor = L->head;
	  L->index = 0;
	  while(index(L)<m){ 
         moveNext(L);
	  }
   }
}

int length(List L){
   if(L == NULL){
	   fprintf(stderr, "ERROR: list does not exist in length()\n");
	   exit(1);
   }
   return L->length; 
}

int index(List L){
	if(L == NULL){
		fprintf(stderr, "ERROR: list does not exist");
		exit(1);
	}else if(L->index == -1){
		//printf("Index is currently null");
		return -1;
	}
	return L->index;
}

// Returns front element. Pre: length()>0
int front(List L){
	if(L == NULL){
	   fprintf(stderr, "ERROR: list does not exist");
	   exit(1);
	}else if(L->head == NULL){
	   fprintf(stderr, "ERROR: index head does not exist");
	   exit(1);
	}
	return L->head->data;
}

// Returns back element. Pre: length()>0
int back(List L){
	if(L == NULL){
	   fprintf(stderr, "ERROR: list does not exist in back ()");
	   exit(1);
	}else if(L->length == 0){
       fprintf(stderr, "ERROR: list's length is 0 in back()");  
	   exit(1);
	}else if(L->tail == NULL){
		fprintf(stderr, "ERROR: L-> tail = NULL in back()");
	}
	return L->tail->data;
}

// Returns cursor element. Pre: length()>0, index()>=0
int get(List L){
	if(L == NULL){
	   fprintf(stderr, "ERROR: list does not exist");
	   exit(1);
	}else if(L->length == 0 || L->index == -1){
	   fprintf(stderr, "ERROR: get() unable to find cursor or index \n");
	   exit(1);
	}else if(L->cursor == NULL){
	   fprintf(stderr, "ERROR: L-> cursor = NULL in get()");
	   exit(1);
	}
	return L->cursor->data;
}

// Returns true if and only if this List and L are the same
 // integer sequence. The states of the cursors in the two Lists
 // are not used in determining equality.
int equals(List A, List B){ 
   if(A == NULL || B == NULL){
	   if(A == NULL && B == NULL){
		   printf("Both lists are null but equal");
		   return 1;
	   }
	   fprintf(stderr, "ERROR: One list is NULL");
	   exit(1);
   }
   int t;
   Node tempA;
   Node tempB;
   tempA = A->head;
   tempB = B->head;
   if(A->length != B->length){
	   return 0;
   }

   for(int i = 0; i<(A->length); i++){
      t = (tempA->data == tempB->data);
	  tempA = tempA->next;
	  tempB = tempB->next;
      	  
   }
   free(tempA);
   tempA = NULL;
   free(tempB);
   tempB = NULL;
   return t;
}

// Manipulation procedures ----------------------------------------------------

// Resets this List to its original empty state.
void clear(List L){
   if(L == NULL){ 
      fprintf(stderr, "ERROR: list is NULL in clear()");
	  exit(1);
   }
	freeList(&L);
	L = newList();
}
// If List is non-empty, places the cursor under the front element,
// otherwise does nothing.
void moveFront(List L){
   moveTo(L, 0);
}

// If List is non-empty, places the cursor under the back element,
// otherwise does nothing.
void moveBack(List L){
   moveTo(L, L->length-1);
}

// If cursor is defined and not at front, moves cursor one step toward
// front of this List, if cursor is defined and at front, cursor becomes
// undefined, if cursor is undefined does nothing.
void movePrev(List L){
   if(L == NULL){
      fprintf(stderr, "ERROR: List equals null in movePrev()");
	  exit(1);
   }
   if(L->cursor == NULL){
      L->index = -1; 
      printf("ERROR: L->cursor = NULL");
	  return;
   }
   L->cursor=L->cursor->prev;
   L->index--;
   

}

// If cursor is defined and not at back, moves cursor one step toward
// back of this List, if cursor is defined and at back, cursor becomes
// undefined, if cursor is undefined does nothing.
void moveNext(List L){
   if(L->index == -1){
	  L->cursor = NULL;
      printf("ERROR: index equals NULL in moveNext()");
   }else if(L->cursor->next == NULL){ 
	   L->cursor=NULL;
	   L->index = -1;
   }else{
      L->cursor = L->cursor->next; 
      L->index++;
   }   
}

//Insert new element into this List. If List is non-empty,
//insertion takes place before front element.
void prepend(List L, int data){
   if(L == NULL){ 
	   fprintf(stderr, "ERROR: list equals null in prepend()");
	   exit(1);
   }
   Node new = newNode(data); 
   //new->data = data;
   if(L->length == 0){
     L->head = new;
	  L->tail = new;
	  L->length++;
	  return;
   }
   L->head->prev = new; 
   new->next = L->head;
   L->head = new; 
   L->length++;
   if(L->index != -1){
	   L->index++;
   }
   return;
	   
}

//Insert new element into this List. If List is non-empty,
//insertion takes place after back element.
void append(List L, int data){
   if(L == NULL){
      fprintf(stderr, "ERROR: list equals null in append()");
	  exit(1);
   }
   Node new = newNode(data); //used to be pointer
   if(L->length ==0){
      L->head = new;
	  L->tail = new;
	  L->length++;
	  return;
   }
   L->tail->next = new;
   new->prev = L->tail;
   L->tail = new;
   L->length++;
}

//Insert new element before cursor.Pre: length()>0, index()>=0
void insertBefore(List L, int data){
   if(L->index == -1){
	   fprintf(stderr, "ERROR: index is undefined in insertBefore()");
	   exit(1);
   }
   if(L->length == 0){//make sure this doesn't cause errors if errorous length =0
      append(L, data);
	  return;
   }
   Node new = newNode(data); //used to be pointer
   new->data = data;
   //if cursor is on head
   if(L->cursor->prev == NULL){
	   prepend(L, data); //does this work
	   L->index++; //should this be here?
	   return;
   }
   L->cursor->prev->next = new;
   new->next = L->cursor;
   new->prev = L->cursor->prev;
   L->cursor->prev = new;
   L->length++;
   L->index++;
   new = NULL;
}
// Inserts new element after cursor.
// Pre: length()>0, index()>=0
void insertAfter(List L, int data){
   if(L->index == -1){
	   fprintf(stderr, "ERROR: index is undefined in insertBefore()");
	   exit(1);
   }
   if(L->length == 0){//make sure this doesn't cause errors if somewhere errorous length =0
      append(L, data);
	  return;
   }
   Node new = newNode(data); //used to be pointer
   new->data = data;
   //if cursor is on tail
   if(L->cursor->next == NULL){
	   append(L, data); //does this work
	   return;
   }
   L->cursor->next->prev = new;
   new->prev = L->cursor;
   new->next = L->cursor->next;
   L->cursor->next = new;
   L->length++;
}

// Deletes the front element. Pre: length()>0
void deleteFront(List L){
   if(L == NULL || L->length==0){ 
      return; 
   }else if(L->head == NULL){
      fprintf(stderr, "ERROR: head is null in deleteFront()");
	   exit(1);
   }else if(L->cursor == L-> head){
     L->index = -1;
	  L->cursor = NULL;
   }
   Node temp = L->head; //used to be pointer
   L->head->next->prev=NULL;
   L->head->next=L->head;
   L->length--;
   //freeNode(&temp);
   temp = NULL;
   if(L->index != -1){
	   L->index--;
   }
}

// Deletes the back element. Pre: length()>0
void deleteBack(List L){
   if(L == NULL || L->length == 0){ 
      return; 
   }else if(L->tail == NULL){
	   fprintf(stderr,"ERROR: tail is null in deleteBack()");
	   exit(1);
   }else if(L->cursor == L->tail){
	   L->index= -1;
	   L->cursor = NULL;
   }
   Node temp = L->tail;
   if(length(L) > 1){
      L->tail = L->tail->prev;
	  freeNode(&temp);
   }else{
      temp = NULL;
   }
   L->length--;
}

// Deletes cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
void delete(List L){
   if(L == NULL){
	   return;
   }else if(L->index == -1 || L->cursor == NULL){
	   fprintf(stderr, "ERROR: index is undefined in delete()");
	   exit(1);
   }else if(L->cursor == L->head){
	  deleteFront(L);
	  return;
   }else if(L->cursor == L->tail){
      deleteBack(L);
	  return;
   }
   Node temp = L->cursor; //used to be pointer
   L->cursor->next->prev = L->cursor->prev;
   L->cursor->prev->next = L->cursor->next;
   L->cursor->prev = NULL; //should i declare these as null?
   L->cursor->next = NULL;
   
   temp = NULL;
   
   L->length--;
   L->index=-1;
}

// Other operations -----------------------------------------------------------

//printsList to specificed output file
void printList(FILE* out, List L){
   int t;
   if(L == NULL || out == NULL){
      fprintf(stderr, "ERROR: list or out file equals NULL in printlist()");
      exit(1);
   }
   Node temp = L->head; //used to be pointer
   
   while(temp !=NULL){
      t = temp->data;
	  fprintf(out, "%d ", t);
	  temp = temp->next;
   }
   temp = NULL;
}

// Returns a new List representing the same integer sequence as this
// List. The cursor in the new list is undefined, regardless of the
// state of the cursor in this List. This List is unchanged.
List copyList(List L){
   if(L == NULL){
      fprintf(stderr, "ERROR: no list to copy in copyList()\n");
	   exit(1);
   }
   List copy = newList();
   Node temp = L->head; 
   
   while(temp != NULL){
	   append(copy, temp->data);
	   temp = temp->next;
   }
   temp = NULL;
   return copy;
}

void freeNode(Node *pN){
   if(pN != NULL && *pN != NULL){
      free(*pN);
	  *pN = NULL;
   }
}



