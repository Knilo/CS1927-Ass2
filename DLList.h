// DLList.h - Interface to doubly-linked list ADT
// Written by John Shepherd, March 2013
// Last modified, August 2014

#ifndef DLLIST_H
#define DLLIST_H

#include <stdio.h>
#include "DLList.h"

// External view of DLList
// Implementation given in DLList.c
// Implements a DLList of strings (i.e. items are strings)

typedef struct DLListRep *DLList;

// create a new empty DLList
DLList newDLList();

// free up all space associated with list
void freeDLList(DLList);

// create an DLList by reading items from a file
// assume that the file is open for reading
//DLList getDLList(FILE *);

// display list on output, one item per line
void showDLList(DLList);

//change pagerank
DLList alterpagerank (DLList, double);

//alter degree
DLList alterDegree (DLList, int);

//get pagerank
double getpagerank (DLList);

//get degree
int getDegree (DLList L);

// check sanity of a DLList (for testing)
int validDLList(DLList);

// return item at current position
char *DLListCurrent(DLList);

// move current position (+ve forward, -ve backward)
// return 1 if reach end of list during move
int DLListMove(DLList, int);

// move to specified position in list
// i'th node, assuming first node has i==1
int DLListMoveTo(DLList, int);

// insert an item before current item
// new item becomes current item
void DLListBefore(DLList, char *, double, int);

// insert an item after current item
// new item becomes current item
void DLListAfter(DLList, char *, double, int);

// delete current item
// new item becomes item following current
// if current was last, current becomes new last
// if current was only item, current becomes null
void DLListDelete(DLList);

// return number of elements in a list
int DLListLength(DLList);

// is the list empty?
int DLListIsEmpty(DLList);

//print to pagerankList.txt
void printToFileP (DLList);

//function to print TfiDf list to screen for searchTfIdf
void printToScreenTfIdf (DLList);

//compares lists and returns common nodes
DLList cmpLists(DLList a, DLList b);

//function to order list by pagerank
DLList orderByPagerank (DLList L);

DLList orderByDegreeAndDeleteDuplicates (DLList L);

//function to check if next exists
int isNext(DLList L);

#endif
