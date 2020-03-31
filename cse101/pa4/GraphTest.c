//MARK MORENO
//1742618
//pa4
//maalmore

#include <stdio.h>
#include<stdlib.h>
#include "Graph.h"
#include "time.h"

int main(int argc, const char * argv[]) {
    srand(time(0));
    int randI=20;
    int randJ=rand()%5;
    Graph G= newGraph(randI);
    for(int i=1;i<randI+1;i++){
        for(int j=0;j<randJ;j++){
            int random=rand()%randI;
            //printf("%d\n",random);
            addArc(G, i, random);
        }
    }
    
    //Graph G= newGraph(15);
    addEdge(G, 1, 7);
    addEdge(G, 2, 3);
    addEdge(G, 2, 6);
    addEdge(G, 3, 7);
    addEdge(G, 4, 10);
    addEdge(G, 5, 9);
    addEdge(G, 5, 10);
    addEdge(G, 6, 7);
    addEdge(G, 6, 11);
    addEdge(G, 7, 12);
    addEdge(G, 8, 13);
    addEdge(G, 9, 13);
    addEdge(G, 9, 14);
    addEdge(G, 10, 15);
    addEdge(G, 14, 15);
    printGraph(stdout, G);
    printPath(stdout, G, 1, 12);
    printPath(stdout, G, 1, 15);
    printPath(stdout, G, 4, 8);
    printPath(stdout, G, 9, 6);
    printPath(stdout, G, 15, 8);
    
    
    return 0;
}