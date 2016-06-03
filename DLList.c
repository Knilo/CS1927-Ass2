// DLList.c - Implementation of doubly-linked list ADT
// Written by John Shepherd, March 2013
// Modified by John Shepherd, August 2014, August 2015

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "DLList.h"

// data structures representing DLList

typedef struct DLListNode {
	char   *urlname;  // urlname of this list item (string)
	double    pagerank; //pagerank value
	int degree;
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

typedef struct DLListRep {
	int  nitems;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *curr;  // current node in list
	DLListNode *last;  // last node in list
} DLListRep;

// create a new DLListNode (private function)
static DLListNode *newDLListNode(char *it, double pr, int d)
{
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->urlname = strdup(it);
	new->pagerank = pr;
	new->degree = d;
	new->prev = new->next = NULL;
	return new;
}

//alter pagerank
DLList alterpagerank (DLList L, double pr) {
	assert(L != NULL);
	assert(L->curr != NULL);
	L->curr->pagerank = pr;
	return L;
}

//alter degree
DLList alterDegree (DLList L, int d) {
	assert(L != NULL);
	assert(L->curr != NULL);
	L->curr->degree = d;
	return L;

}
//get pagerank
double getpagerank (DLList L) {
	assert(L != NULL);
	assert(L->curr != NULL);
	return L->curr->pagerank;
}

//get degree
int getDegree (DLList L) {
	assert(L != NULL);
	assert(L->curr != NULL);
	return L->curr->degree;
}


// create a new empty DLList
DLList newDLList()
{
	struct DLListRep *L;

	L = malloc(sizeof (struct DLListRep));
	assert (L != NULL);
	L->nitems = 0;
	L->first = NULL;
	L->last = NULL;
	L->curr = NULL;
	return L;
}

// free up all space associated with list
void freeDLList(DLList L)
{
	assert(L != NULL);
	DLListNode *curr, *prev;
	curr = L->first;
	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(L);
}

// trim off \n from strings (private function)
// this is needed for getDLList() because of fgets()
// alternatively, we could use the evil gets() function
/*static char *trim(char *s)
{
	int end = strlen(s)-1;
	if (s[end] == '\n') s[end] = '\0';
	return s;
}*/

// create an DLList by reading items from a file
// assume that the file is open for reading
// assume one item per line, line < 999 chars
/*DLList getDLList(FILE *in)
{
	DLList L;
	DLListNode *new;
	char line[1000];

	L = newDLList();
	while (fgets(line,1000,in) != NULL) {
		char *urlname = strdup(trim(line));
		//new = newDLListNode(urlname, pr);
		if (L->last == NULL) {
			L->first = L->last = new;
		}
		else {
			L->last->next = new;
			new->prev = L->last;
			L->last = new;
		}
		L->nitems++;
	}
	L->curr = L->first;
	return L;
}*/

// display items from a DLList, one per line
void showDLList(DLList L)
{
	assert(L != NULL);
	DLListNode *curr;
	for (curr = L->first; curr != NULL; curr = curr->next)
		printf("%s, %d, %0.7f\n", curr->urlname, curr->degree, curr->pagerank);
}

// check sanity of a DLList (for testing)
int validDLList(DLList L)
{
	if (L == NULL) {
		fprintf(stderr, "DLList is null\n");
		return 0;
	}
	if (L->first == NULL) {
		// list is empty; curr and last should be null
		if (L->last != NULL || L->curr != NULL) {
			fprintf(stderr, "Non-null pointers in empty list\n");
			return 0;
		}
	}
	else {
		// list is not empty; curr and last should be non-null
		if (L->last == NULL || L->curr == NULL) {
			fprintf(stderr, "Null pointers in non-empty list\n");
			return 0;
		}
	}
	int count;
	DLListNode *curr;
	// check scanning forward through list
	count = 0;
	for (curr = L->first; curr != NULL; curr = curr->next) {
		if (curr->prev != NULL && curr->prev->next != curr) {
			fprintf(stderr, "Invalid forward link into node (%s)\n", curr->urlname);
			return 0;
		}
		if (curr->next != NULL && curr->next->prev != curr) {
			fprintf(stderr, "Invalid backward link into node (%s)\n", curr->urlname);
			return 0;
		}
		count++;
	}
	if (count != L->nitems) {
		fprintf(stderr, "Forward count mismatch; counted=%d, nitems=%d\n",
		        count, L->nitems);
		return 0;
	}
	// check scanning backward through list
	count = 0;
	for (curr = L->last; curr != NULL; curr = curr->prev) {
		count++;
	}
	if (count != L->nitems) {
		fprintf(stderr, "Backward count mismatch; counted=%d, nitems=%d\n",
		        count, L->nitems);
		return 0;
	}
	// nothing went wrong => must be ok
	return 1;
}

// return item at current position
char *DLListCurrent(DLList L)
{
	assert(L != NULL); assert(L->curr != NULL);
	return L->curr->urlname;
}

// move current position (+ve forward, -ve backward)
// return 1 if reach end of list during move
// if current is currently null, return 1
int DLListMove(DLList L, int n)
{
	assert(L != NULL);
	if (L->curr == NULL)
		return 1;
	else if (n > 0) {
		while (n > 0 && L->curr->next != NULL) {
			L->curr = L->curr->next;
			n--;
		}
	}
	else if (n < 0) {
		while (n < 0 && L->curr->prev != NULL) {
			L->curr = L->curr->prev;
			n++;
		}
	}
	return (L->curr == L->first || L->curr == L->last);
}

// move to specified position in list
// i'th node, assuming first node has i==1
int DLListMoveTo(DLList L, int i)
{
	assert(L != NULL); assert(i > 0);
	L->curr = L->first;
	return DLListMove(L, i - 1);
}

// insert an item before current item
// new item becomes current item
void DLListBefore(DLList L, char *it, double pr, int d)
{
	assert(L != NULL);
	if (L->curr == NULL) {
		DLListNode *new = newDLListNode(it, pr, d);
		L->first = new;
		L->last = new;
		L->curr = new;
	} else {
		DLListNode *new = newDLListNode(it, pr, d);
		DLListNode *temp = L->curr->prev;
		new->next = L->curr;
		L->curr->prev = new;

		if (L->curr == L->first) {
			L->first = new;
		} else {
			temp->next = new;
		}
		new->prev = temp;
		L->curr = new;
	}
	L->nitems ++;

}

// insert an item after current item
// new item becomes current item
void DLListAfter(DLList L, char *it, double pr, int d)
{
	assert(L != NULL);
	if (L->nitems == 0) {
		DLListNode *new = newDLListNode(it, pr, d);
		L->first = new;
		L->last = new;
		L->curr = new;
	} else {
		DLListNode *temp = L->curr->next;
		DLListNode *new = newDLListNode(it, pr, d);
		new->prev = L->curr;
		L->curr->next = new;

		if (L->curr == L->last) {
			L->last = new;
		} else {
			temp->prev = new;
		}
		new->next = temp;
		L->curr = new;
	}
	L->nitems ++;
}
// delete current item
// new item becomes item following current
// if current was last, current becomes new last
// if current was only item, current becomes null
void DLListDelete(DLList L)
{
	assert(L != NULL);
	if (L->curr == L->last) {
		L->last = L->curr->prev;
		free(L->curr);
		L->curr = L->last;
		L->last = NULL;
	} else if (L->curr == L->first) {
		L->first = L->curr->next;
		free(L->curr);
		L->curr = L->first;
		L->first = NULL;
	} else {
		L->curr->prev->next = L->curr->next;
		L->curr->next->prev = L->curr->prev;
		DLListNode *tempNode = L->curr->next;
		free(L->curr);
		L->curr = tempNode;
	}
	L->nitems--;
}

// return number of elements in a list
int DLListLength(DLList L)
{
	return (L->nitems);
}

// is the list empty?
int DLListIsEmpty(DLList L)
{
	return (L->nitems == 0);
}

void printToFileP (DLList L) {
	assert(L != NULL);
	FILE * outputFile = fopen("pagerankList.txt", "w");
	DLListNode *curr;
	for (curr = L->first; curr != NULL; curr = curr->next)
		fprintf(outputFile, "%s, %d, %0.7f\n", curr->urlname, curr->degree, curr->pagerank);
}

void printToScreenTfIdf (DLList L) {
	assert(L != NULL);
	DLListNode *curr;
	int i= 0;
	for (curr = L->first; curr != NULL && i <= 10; curr = curr->next, i++)
		printf("%s %0.7f\n", curr->urlname, curr->pagerank);
}

DLList cmpLists(DLList a, DLList b) {
	if (DLListIsEmpty(b)) {
		return a;
	}
	DLList new = newDLList();
	DLListNode *temp = a->first;
	DLListNode *temp2 = b->first;
	while (temp != NULL) {
		temp2 = b->first;
		while (temp2 != NULL) {
			if (strcmp(temp->urlname, temp2->urlname) == 0) {
				DLListAfter(new, temp->urlname, 0, 0);
				break;
			}
			temp2 = temp2->next;
		}
		temp = temp->next;
	}
	freeDLList(a);
	freeDLList(b);

	return new;
}

DLList orderByPagerank (DLList L) {
	DLList new = newDLList();
	//puts("hi");
	int i = 0;
	int j = 0;
	for (i = 0; i < DLListLength(L); i++) {
		DLListMoveTo(L, i + 1);
		//puts("");

		if (DLListLength(new) == 0) {
			//puts("yo");
			DLListAfter(new, DLListCurrent(L), getpagerank(L), getDegree(L));
		}
		else if (DLListLength(new) == 1) {
			//puts("yo yo");
			if (getpagerank(L) <= getpagerank(new)) {
				DLListAfter(new, DLListCurrent(L), getpagerank(L), getDegree(L));
			} else {
				DLListBefore(new, DLListCurrent(L), getpagerank(L), getDegree(L));
			}
		}
		else if (DLListLength(new) > 1) {
			//puts("no yo");
			DLListMoveTo(new, 1);
			j = 0;
			while ((getpagerank(L) <= getpagerank(new)) && j <= DLListLength(new)) {
				//puts("checking");
				DLListMove(new, 1);
				j++;
			}
			if (j >= DLListLength(new)) {
				//puts("poop");
				DLListAfter(new, DLListCurrent(L), getpagerank(L), getDegree(L));
			} else {
				//puts("wee");
				DLListBefore(new, DLListCurrent(L), getpagerank(L), getDegree(L));
			}

		}
		
		//printToScreenTfIdf(new);
	}



	freeDLList(L);
//printToScreenTfIdf(new);
	return new;
}

int isNext(DLList L){
	assert(L != NULL);
	return (L->curr->next != NULL);
}