//MARK MORENO
//1742618
//pa5
//maalmore
#ifndef Graph_h
#define Graph_h

#include <stdio.h>
#include<stdlib.h>
#include "List.h"
extern int NIL;
extern int UNDEF;
/*** Constructors-Destructors ***/
typedef struct GraphObj *Graph;
Graph newGraph(int n);
void freeGraph(Graph* pG);
/*** Access functions ***/
int getOrder(Graph G);
int getSize(Graph G);
int getSource(Graph G);
int getParent(Graph G, int u);
/*** Manipulation procedures ***/
void makeNull(Graph G);
void addEdge(Graph G, int u, int v);
void addArc(Graph G, int u, int v);
void BFS(Graph G, int s);
/*** Other operations ***/
void printGraph(FILE* out, Graph G);
int getFinish(Graph G, int u);
int getDiscover(Graph G, int u);
void DFS(Graph G, List S);
Graph transpose(Graph G);
Graph copyGraph(Graph G);
void printConnect(FILE* out, Graph G);
void DFS_Visit(Graph G, int u, List S, List L);

#endif /* Graph_h */