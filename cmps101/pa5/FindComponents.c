//MARK MORENO
//1742618
//pa5
//maalmore
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include "Graph.h"
int main(int argc, char* argv[]){
    if (argc!=3){
        printf("Incorrect number of arguments: EXIT");
        exit(1);
    }
    FILE *f = fopen(argv[1], "r");
    //FILE *f = fopen("/Users/markmoreno/Desktop/test.txt", "r");
    FILE* fp = fopen (argv[2],"w");
    //FILE* fp = fopen ("/Users/markmoreno/Desktop/empty.txt","w");
    int n = 0;
    
    
    fscanf(f, "%d", &n);
    Graph G=newGraph(n);
    int v, e;
    v=e=-1;
    while(v!=0&&e!=0){
        fscanf(f, "%d %d", &v, &e);
        addArc(G, v, e);
    }
    printGraph(stdout, G);
    printGraph(fp, G);
    v=e=-1;
    Graph T=transpose(G);
    printGraph(stdout, T);
    List L = newList();
    DFS(G, L);
    printList(stdout, L);
    
    printConnect(stdout, G);
    printList(stdout, L);
    DFS(T,L);
    printConnect(stdout, T);
    printConnect(fp, T);
    
    
    freeGraph(&G);
    freeGraph(&T);
    freeList(&L);
    
    fclose(f);
    fclose(fp);
    
    return 0;
}
