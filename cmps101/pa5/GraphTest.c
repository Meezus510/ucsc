//MARK MORENO
//1742618
//pa5
//maalmore

#include<stdio.h>
#include<stdlib.h>
#include"List.h"
#include"Graph.h"
#include "time.h"


int main(int argc, char* argv[]){
    int i, n=20;
    List S = newList();
    Graph G = newGraph(n);
    Graph T=NULL, C=NULL;
    
    for(i=1; i<=n; i++) append(S, i);
    srand(time(0));
    int randI=20;
    for(int i=1;i<randI+1;i++){
        int randJ=rand()%5;
        for(int j=0;j<randJ;j++){
            int random=rand()%randI;
            //printf("%d\n",random);
            addArc(G, i, random+1);
        }
    }
    
    printGraph(stdout, G);
    
    DFS(G, S);
    fprintf(stdout, "\n");
    fprintf(stdout, "x:  d  f  p\n");
    for(i=1; i<=n; i++){
        fprintf(stdout, "%d: %2d %2d %2d\n", i, getDiscover(G, i), getFinish(G, i), getParent(G, i));
    }
    fprintf(stdout, "\n");
    printList(stdout, S);
    fprintf(stdout, "\n");
    
    T = transpose(G);
    C = copyGraph(G);
    fprintf(stdout, "\n");
    printGraph(stdout, C);
    fprintf(stdout, "\n");
    printGraph(stdout, T);
    fprintf(stdout, "\n");
    
    DFS(T, S);
    fprintf(stdout, "\n");
    fprintf(stdout, "x:  d  f  p\n");
    for(i=1; i<=n; i++){
        fprintf(stdout, "%d: %2d %2d %2d\n", i, getDiscover(T, i), getFinish(T, i), getParent(T, i));
    }
    fprintf(stdout, "\n");
    printList(stdout, S);
    fprintf(stdout, "\n");
    
    freeList(&S);
    freeGraph(&G);
    freeGraph(&T);
    freeGraph(&C);
    return(0);
}