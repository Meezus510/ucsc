//MARK MORENO
//1742618
//pa4
//maalmore
#include "Graph.h"
#include "List.h"
int NIL=-1;
int INF=-1;
typedef struct GraphObj{
    int order;
    int source;
    List*row;
    int *color;
    int *parent;
    int *distance;
    int *discover;
    int size;
    
}GraphObj;
/*** Constructors-Destructors ***/
Graph newGraph(int n){
    Graph G=malloc(sizeof(GraphObj));
    G->row=malloc((n+1)*sizeof(*G->row));
    G->color=malloc((n+1)*sizeof(*G->color));
    G->parent=malloc((n+1)*sizeof(*G->parent));
    G->distance=malloc((n+1)*sizeof(*G->distance));
    G->discover=malloc((n+1)*sizeof(*G->discover));
    G->order=n;;
    G->size=0;
    G->source=-1;
    for(int i =0; i <= n; i++){
        G->color[i]=-1;
        G->parent[i]=-1;
        G->distance[i]=-1;
        G->row[i]=NULL;
        G->row[i]=newList();
    }
    return G;
    
}
void freeGraph(Graph* pG){
    Graph temp = *pG;
    if(pG!=NULL&&*pG!=NULL){
        for(int i =0;i<=temp->order;i++){
            List L=temp->row[i];
            freeList(&L);
            free(L);
            L=NULL;
        }
        free(temp->row);
        temp->row=NULL;
        free(temp->color);
        temp->color=NULL;
        free(temp->discover);
        temp->discover=NULL;
        free(temp->distance);
        temp->distance=NULL;
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
int getSource(Graph G){
    return G->source;
}
int getParent(Graph G, int u){
    return G->parent[u];
}
int getDist(Graph G, int u){
    return G->distance[u];
}
void getPath(List L, Graph G, int u){
    //printf("[%d %d %d]\n",G->distance[u],G->source,G->parent[u]);
    if(u!=getSource(G)&&getParent(G, u)!=-1){
        int p=u;
        while(getParent(G, p)!=getSource(G)){
            p=getParent(G, p);
            prepend(L, p);
        }
        prepend(L, getSource(G));
    }
    if(getDist(G, u)!=-1||!isEmpty(L))
        append(L, u);
    else append(L, NIL);
}
/*** Manipulation procedures ***/
void makeNull(Graph G){
    if(G!=NULL){
        for(int i =0;i<=G->order;i++){
            G->color[i]=-1;
            G->distance[i]=-1;
            G->parent[i]=-1;
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
void BFS(Graph G, int s){
    List queue=newList();
    int dist=0;
    for(int i=1;i<=G->order;i++){
        G->color[i]=0;
        G->parent[i]=-1;
        G->distance[i]=-1;
    }
    G->source=s;
    G->color[s]=1;
    G->parent[s]=-1;
    G->distance[s]=0;
    append(queue, s);
    while(!isEmpty(queue)){
        
        moveFront(queue);
        
        int u=get(queue);
        //printf("%d ",u);
        deleteFront(queue);
        moveFront(G->row[u]);
        for(int i=1;i<=length(G->row[u]);i++){
            dist=1;
            int v=get(G->row[u]);
            if(G->color[v]==0){
                G->color[v]=1;
                G->parent[v]=u;
                int p=v;
                while(getParent(G, p)!=s){
                    p=getParent(G, p);
                    dist++;
                }
                G->distance[v]=dist;
                append(queue, v);
                
            }
            moveNext(G->row[u]);
        }
        G->color[u]=2;
    }
    freeList(&queue);
}
/*** Other operations ***/
void printGraph(FILE* out, Graph G){
    for(int i=1;i<=G->order;i++){
        fprintf(out,"%d: ",i);
        printList(out, G->row[i]);
    }
    fprintf(out,"\n");
    
}
void printPath(FILE* out, Graph G, int s, int u){
    BFS(G, s);
    if(getDist(G, u)==-1){
        fprintf(out,"The distance from %d to %d is infinity\n",s,u);
        fprintf(out,"No %d-%d path exists\n\n",s,u);
    }else{
        List Queue=newList();
        getPath(Queue, G, u);
        fprintf(out,"The distance from %d to %d is %d\n",s,u,getDist(G, u));
        fprintf(out,"A shortest %d-%d path is: ",s,u);
        printList(out, Queue);
        fprintf(out,"\n");
        freeList(&Queue);
        
    }
}