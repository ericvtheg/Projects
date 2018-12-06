//------------------------------------------------------------------------------
// List.h
// Header file for Queue ADT
//------------------------------------------------------------------------------
//Eric Ventor
//1492157
//CS101 
//10/18/2017

//#define _LIST_H_INCLUDE_
//#ifndef _LIST_H_INCLUDE_
#include<stdio.h>
#include<stdlib.h>


// Exported type --------------------------------------------------------------
typedef struct NodeObj* Node;
typedef struct ListObj* List;

// Constructors-Destructors ---------------------------------------------------

//newList()
//Returns reference to new empty List object
List newList(void);

//freeQueue()
//Frees all heap memory associated with List *L & sets *pQ to Null
void freeList(List* pL);


// Access functions -----------------------------------------------------------

void moveTo(List L, int m);

int length(List L);

int index(List L);

int front(List L);

int back(List L);

int get(List L);

int equals(List A, List B);

// Manipulation procedures ----------------------------------------------------

void clear(List L);

void moveFront(List L);

void moveBack(List L);

void movePrev(List L);

void moveNext(List L);

void prepend(List L, int data);

void append(List L, int data);

void insertBefore(List L, int data);

void insertAfter(List L, int data);


void deleteFront(List L);

void deleteBack(List L);

void delete(List L);

// Other operations -----------------------------------------------------------

void printList(FILE* out, List L);

List copyList(List L);

void freeNode(Node *pN);

//#endif
