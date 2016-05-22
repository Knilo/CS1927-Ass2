// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"
#include "Queue.h"

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int    nV;    // #vertices
	int    nE;    // #edges
	int  **edges; // matrix of weights (0 == no edge)
} GraphRep;

// check validity of Vertex
int validV(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
Edge mkEdge(Graph g, Vertex v, Vertex w, int weight)
{
	assert(g != NULL && validV(g, v) && validV(g, w));
	Edge new = {v, w, weight}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge(Graph g, Vertex v, Vertex w, int wt)
{
	assert(g != NULL && validV(g, v) && validV(g, w));
	if (g->edges[v][w] == 0) {
		g->edges[v][w] = wt;
		g->edges[w][v] = wt;
		g->nE++;
	}
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g, v) && validV(g, w));
	if (g->edges[v][w] != 0) {
		g->edges[v][w] = 0;
		g->edges[w][v] = 0;
		g->nE--;
	}
}

// create an empty graph
Graph newGraph(int nV)
{
	assert(nV > 0);
	struct GraphRep *g;
	g = malloc(sizeof (struct GraphRep));
	assert (g != NULL);
	g->nV = nV;
	g->nE = 0;
	/*
	g->edges = malloc(sizeof(x));
	for (int i = 0; i < g->nV; i++) {
		for (int j = 0; j < g->nV; j++) {

			g->edges[i][j] = 0;
		}
	}
	*/
	int** arr;
	arr = (int**) malloc(nV * sizeof(int*));
	for (int i = 0; i < nV; i++) {
		arr[i] = (int*) malloc(nV * sizeof(int));
	}
	g->edges = arr;
	return g;
}

// free memory associated with graph
void dropGraph(Graph g)
{
	assert(g != NULL);
}

// display graph, using names for vertices
void showGraph(Graph g, char **names)
{
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf("\n");
	}
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath(Graph g, Vertex src, Vertex dest, int max, int *path)
{
	assert(g != NULL && validV(g, src) && validV(g, dest));
	return -1;  // dummy ... always claims there is no path
}
