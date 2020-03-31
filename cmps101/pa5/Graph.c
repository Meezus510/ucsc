//MARK MORENO
//1742618
//pa5
//maalmore
#include "Graph.h"
#include "List.h"
int NIL=0;
int UNDEF=-1;
typedef struct GraphObj{
    int order;
    int count;
    List*connected;
    List*row;
    int *color;
    int *parent;
    int *discover;
    int *finish;
    int size;
    int time;
    
}GraphObj;
/*** Constructors-Destructors ***/
Graph newGraph(int n){
    Graph G=malloc(sizeof(GraphObj));
    G->connected=malloc((n+1)*sizeof(*G->connected));
    G->row=malloc((n+1)*sizeof(*G->row));
    G->color=malloc((n+1)*sizeof(*G->color));
    G->parent=malloc((n+1)*sizeof(*G->parent));
    G->discover=malloc((n+1)*sizeof(*G->discover));
    G->finish=malloc((n+1)*sizeof(*G->finish));
    G->order=n;;
    G->size=0;
    G->time=0;
    G->count=0;
    for(int i =0; i <= n; i++){
        G->color[i]=-1;
        G->parent[i]=0;
        G->discover[i]=-1;
        G->finish[i]=-1;
        G->row[i]=NULL;
        G->row[i]=newList();
        G->connected[i]=NULL;
        G->connected[i]=newList();
    }
    return G;
    
}
void freeGraph(Graph* pG){
    Graph temp = *pG;
    if(pG!=NULL&&*pG!=NULL){
        for(int i =0;i<=temp->order;i++){
            List L=temp->row[i];
            List A=temp->connected[i];
            freeList(&L);
            freeList(&A);
            
            free(L);
            free(A);
            L=NULL;
            A=NULL;
        }
        free(temp->connected);
        temp->connected=NULL;
        free(temp->row);
        temp->row=NULL;
        free(temp->color);
        temp->color=NULL;
        free(temp->discover);
        temp->discover=NULL;
        free(temp->finish);
        temp->finish=NULL;
        free(temp->parent);
        temp->parent=NULL;
        temp->row=NULL;
        free(*pG);
        *pG=NULL;
        
    }
    
}
/*** Access functions ***/
int getOrder(Graph G){
    return G->order;
}
int getSize(Graph G){
    return G->size;
}
int getCount(Graph G){
    return G->count;
}
int getParent(Graph G, int u){
    return G->parent[u];
}
/*** Manipulation procedures ***/
void makeNull(Graph G){
    if(G!=NULL){
        for(int i =0;i<=G->order;i++){
            G->color[i]=-1;
            G->discover[i]=-1;
            G->finish[i]=-1;
            G->parent[i]=0;
            if(!isEmpty(G->row[i])){
                List L=G->row[i];
                moveFront(L);
                clear(L);
                L=NULL;
            }
        }
        G->size=0;
    }
}
void insertAdj(Graph G, int u, int v){
    if(!isEmpty(G->row[u])){
        moveFront(G->row[u]);
        int found=-1;
        for(int i=0;i<length(G->row[u]);i++){
            if(v==get(G->row[u])){
                found=1;
                break;
            }//do nothing
            else if(v<get(G->row[u])){
                insertBefore(G->row[u], v);
                found=1;
                break;
            }
            moveNext(G->row[u]);
        }
        if(found!=1)append(G->row[u], v);
    }else append(G->row[u], v);
}
void addEdge(Graph G, int u, int v){
    insertAdj(G, u, v);
    insertAdj(G, v, u);
    G->size++;
}
void addArc(Graph G, int u, int v){
    insertAdj(G, u, v);
    G->size++;
}

/*** Other operations ***/
void printGraph(FILE* out, Graph G){
    fprintf(out, "Adjacency list representation of G:\n");
    for(int i=1;i<=G->order;i++){
        fprintf(out,"%d: ",i);
        printList(out, G->row[i]);
    }
    fprintf(out,"\n");
    
}
int getDiscover(Graph G, int u){
    
    return G->discover[u];
}
int getFinish(Graph G, int u){
    return G->finish[u];
}
void DFS_Visit(Graph G, int u, List S, List L){
    G->time++;
    G->discover[u]=G->time;
    G->color[u]=1;
    if(length(G->row[u])!=0){
        moveFront(G->row[u]);
        for(int i=1;i<=length(G->row[u]);i++){
            int v=get(G->row[u]);
            if(G->color[v]==0){
                G->parent[v]=u;
                DFS_Visit(G, v, S, L);
            }
            moveNext(G->row[u]);
        }
    }
    G->color[u]=2;
    prepend(S, u);
    prepend(L, u);
    G->time++;
    G->finish[u]=G->time;
    
}
void DFS(Graph G, List S){
    for(int i=1;i<=G->order;i++){
        G->color[i]=0;
        G->parent[i]=0;
        G->discover[i]=-1;
        G->finish[i]=-1;
    }
    G->time=0;
    G->count=1;
    if(length(S)==0){
        for(int i=1;i<=getOrder(G);i++){
            if(G->color[i]==0){
                DFS_Visit(G, i, S, G->connected[getCount(G)]);
                G->count ++;
            }
        }
    }else{
        moveFront(S);
        for(int i=1;i<=getOrder(G);i++){
            if(G->color[get(S)]==0){
                DFS_Visit(G, get(S), S, G->connected[getCount(G)]);
                G->count++;
            }
            moveNext(S);
        }
        List temp = newList();
        moveFront(S);
        for(int i=1;i<=getOrder(G);i++){
            append(temp, get(S));
            moveNext(S);
        }
        clear(S);
        moveFront(temp);
        for(int i=1;i<=getOrder(G);i++){
            append(S, get(temp));
            moveNext(temp);
        }
        //printList(stdout, temp);
        freeList(&temp);
        //for(int i=0;i<=getOrder(G);i++);
    }
}
Graph transpose(Graph G){
    Graph T=newGraph(getOrder(G));
    for(int i=1;i<=getOrder(T);i++){
        if(length(G->row[i])!=0){
            moveFront(G->row[i]);
            for(int j=0;j<length(G->row[i]);j++){
                addArc(T, get(G->row[i]), i);
                moveNext(G->row[i]);
            }
        }
    }
    return T;
}
Graph copyGraph(Graph G){
    Graph C=newGraph(getOrder(G));
    for(int i=1;i<=getOrder(C);i++){
        if(length(G->row[i])!=0){
            moveFront(G->row[i]);
            for(int j=0;j<length(G->row[i]);j++){
                addArc(C, i, get(G->row[i]));
                moveNext(G->row[i]);
            }
        }
    }
    return C;
    
}
void printConnect(FILE* out, Graph G){
    fprintf(out,"G contains %d strongly connected components:\n",getCount(G)-1);
    for(int i=1;i<=getCount(G);i++){
        if(length(G->connected[i])!=0){
            fprintf(out,"Component %d: ",i);
            printList(out, G->connected[i]);
        }
    }
    fprintf(out,"\n\n");
}

