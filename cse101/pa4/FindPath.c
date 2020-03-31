//MARK MORENO
//1742618
//pa4
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
        //printf( "%d %d\n", v, e);
        addEdge(G, v, e);
    }
    //printGraph(stdout, G);
    printGraph(fp, G);
    v=e=-1;
    while(v!=0&&e!=0){
        fscanf(f, "%d %d", &v, &e);
        //printf( "%d %d\n", v, e);
        if(v!=0&&e!=0)printPath(fp, G, v, e);
        //if(v!=0&&e!=0)printPath(stdout, G, v, e);
    }

    freeGraph(&G);
    
    fclose(f);
    fclose(fp);
    
    return 0;
}