//MARK MORENO
//1742618
//pa4
//maalmore
#include<stdio.h>
#include<stdlib.h>
#include "List.h"

// private struct, not exported
typedef struct NodeObj{
    int data;
    struct NodeObj* next;
    struct NodeObj* prev;
} NodeObj;

// private Node type
typedef NodeObj* Node;

// private QueueObj type
typedef struct ListObj{
    Node front;
    Node back;
    Node cursor;
    int length;
    int indx;
} ListObj;


// Constructors-Destructors ---------------------------------------------------


Node newNode(int data){
    Node N = malloc(sizeof(NodeObj));
    N->data = data;
    N->next = NULL;
    return(N);
}

void freeNode(Node* pN){
    if( pN!=NULL && *pN!=NULL ){
        free(*pN);
        *pN = NULL;
    }
}
List newList(void){
    List L;
    L=malloc(sizeof(ListObj));
    L->front=L->back=L->cursor=NULL;
    L->length=0;
    return L;
}
void freeList(List*pL){
    if(pL!=NULL && *pL!=NULL){
        while( !isEmpty(*pL)){
            deleteFront(*pL);
        }
        free(*pL);
        *pL = NULL;
    }
}

int length(List L){
    
    if(L==NULL){printf("Queue Error: calling getLength() on NULL Queue reference\n");
        return(-1);
    }else
        return (L->length);
}
int index(List L){ //check
    if (L->cursor==NULL){
        return -1;
    }else{
        return L->indx;
    }
}
int front(List L){
    if( L==NULL ){
        printf("Queue Error: calling getFront() on NULL Queue reference\n");
        return(-1);
    }
    if( isEmpty(L) ){
        printf("Queue Error: calling getFront() on an empty Queue\n");
        return(-1);
    }else
        return(L->front->data);
}
int back(List L){
    if( L==NULL ){
        printf("Queue Error: calling getFront() on NULL Queue reference: ");
        return(-1);
    }
    if( isEmpty(L) ){
        printf("Queue Error: calling getFront() on an empty Queue\n");
        return(-1);
    }
    else
        return(L->back->data);
}
int get(List L){
    if( L->cursor==NULL ){
        return(-1);
    }else
        return(L->cursor->data);
}

void clear(List L){
    Node temp;
    while( !isEmpty(L)){
        temp=L->front->next;
        deleteFront(L);
        L->front=temp;
    }
    temp=NULL;
    L->front=L->back=L->cursor=NULL;
    
}
void moveFront(List L){
    if( L==NULL ){
        printf("Error: NULL List reference\n");
    }
    if( isEmpty(L) ){
        printf("Error: empty List\n");
    }
    else{
        L->cursor=L->front;
        L->indx=0;
    }
}
void moveBack(List L){
    if( L==NULL ){
        printf("Error: NULL List reference\n");
    }
    if( isEmpty(L) ){
        printf("Error: empty List\n");
    }else{
        L->cursor=L->back;
        L->indx=L->length-1;
    }
}
void movePrev(List L){
    if( L==NULL ){
        printf("Error: NULL List reference\n");
    }
    if( isEmpty(L) ){
        printf("Error: empty List\n");
    }else{
        L->cursor=L->cursor->prev;
        L->indx--;
    }
}
void moveNext(List L){
    if( L==NULL ){
        printf("Error: NULL List reference\n");
    }
    if( isEmpty(L) ){
        printf("Error: empty List\n");
    }else{
        L->cursor=L->cursor->next;
        L->indx++;
    }
    
}
void prepend(List L, int data){
    //Node N = newNode(data);
    
    if( L==NULL ){
        printf("Error: calling prepend() on NULL Queue reference\n");
    }
    else if( isEmpty(L) ) {
        Node N = newNode(data);
        
        L->front = L->back = N;
        L->front->prev=NULL;
        L->length++;
    }else{
        Node N = newNode(data);
        
        /*Node temp = L->front;
         L->front=N;
         L->front->next=temp;
         L->front->prev=NULL;
         Node tempPrev;
         temp->prev=L->front;
         while(temp->next!=NULL){
         tempPrev=temp;
         temp=temp->next;
         temp->prev=tempPrev;
         }
         L->back=temp;
         */
        Node temp=L->front;
        L->front=N;
        L->front->next=temp;
        L->front->prev=NULL;
        temp->prev=L->front;
        
        if(L->cursor!=NULL)
            L->indx++;
        L->length++;
        temp=NULL;
        
    }
    
}
void append(List L, int data){
    //Node N = newNode(data);
    
    if( L==NULL ){
        printf("Queue Error: calling Enqueue() on NULL Queue reference\n");
    }
    else if( isEmpty(L) ) {
        Node N = newNode(data);
        
        L->front = L->back = N;
        L->front->prev=NULL;
        L->length++;
    }else{
        Node N = newNode(data);
        
        Node temp = L->back;
        L->back->next = N;
        L->back = N;
        L->back->prev=temp;
        L->length++;
        temp=NULL;
    }
}
void insertBefore(List L, int data){
    if(L->cursor==NULL){
    }    else if(L->cursor==L->front){
        prepend(L, data);
    }else{
        Node N = newNode(data);
        Node tempPrev=L->cursor->prev;
        Node temp = L->cursor;
        tempPrev->next=NULL;
        temp->prev=NULL;
        N->prev=tempPrev;
        N->next=temp;
        tempPrev->next=N;
        temp->prev=N;
        L->length++;
        L->indx++;
        tempPrev=temp=NULL;
    }
}
void insertAfter(List L, int data){
    if(L->cursor==NULL){
        
    }
    else if(L->cursor==L->back){
        append(L, data);
    }else{
        Node N = newNode(data);
        Node tempPrev=L->cursor;
        Node temp = L->cursor->next;
        tempPrev->next=NULL;
        temp->prev=NULL;
        N->prev=tempPrev;
        N->next=temp;
        tempPrev->next=N;
        temp->prev=N;
        L->length++;
        tempPrev=temp=NULL;
    }
}
void deleteFront(List L){
    Node N = NULL;
    
    if( L==NULL ){
        //printf("Queue Error: calling Dequeue() on NULL Queue reference\n");
    }
    if( isEmpty(L) ){
        //printf("Queue Error: calling Dequeue on an empty Queue\n");
    }else{
        N = L->front;
        if( length(L)>1 ) {
            L->front = L->front->next;
        }else{
            L->front = L->back = NULL;
        }
        L->length--;
        L->indx--;
        freeNode(&N);
        N=NULL;
        
    }
}
void deleteBack(List L){
    Node N = NULL;
    if( L==NULL ){
        //printf("Queue Error: calling Dequeue() on NULL Queue reference\n");
    }
    else if( isEmpty(L) ){
        //printf("Queue Error: calling Dequeue on an empty Queue\n");
    }else{
        N = L->back;
        if( length(L)>1 ) {
            L->back = L->back->prev;
        }else{
            L->front = L->back = NULL;
        }
        L->length--;
        if(L->cursor==N)L->cursor=NULL;
        freeNode(&N);
        N=NULL;
        //if(L->indx!=-1)L->indx=-1;
        
    }
}

void delete(List L){
    //L->indx=-1;
    if(L->cursor==NULL){
        
    }
    else if(L->cursor==L->front){
        deleteFront(L);
    }
    else if(L->cursor==L->back){
        deleteBack(L);
        
    }
    else{
        Node tempPrev=L->cursor->prev;
        Node tempNext=L->cursor->next;
        freeNode(&L->cursor);
        tempPrev->next=tempNext;
        tempNext->prev=tempPrev;
        L->cursor=NULL;
        //L->indx=-1;
        L->length--;
        tempPrev=tempNext=NULL;
    }
    //L->cursor=NULL;
    
}
List copyList(List L){ //add
    List New = newList();
    Node temp = L->front;
    for(int i=0;i<L->length;i++){
        append(New, temp->data);
        temp=temp->next;
    }
    temp=NULL;
    return New;
    
}



// getLength()
// Returns the length of Q.
int getLength(List L){
    if( L==NULL ){
        printf("Queue Error: calling getLength() on NULL Queue reference\n");
        return -1;
    }else
        return(L->length);
}
int isEmpty(List Q){
    if( Q==NULL ){
        printf("Queue Error: calling isEmpty() on NULL Queue reference\n");
        exit(1);
    }
    return(Q->length==0);
}
void printList(FILE* out,List L){
    Node N = NULL;
    
    if( L==NULL ){
        printf("Queue Error: calling printQueue() on NULL Queue reference\n");
    }
    else{
        for(N = L->front; N != NULL; N = N->next){
            fprintf(out,"%d ", N->data);
        }
        fprintf(out,"\n");
    }
}
List concatList(List A, List B){ //add
    // Returns a new List which is the concatenation of
    // A and B. The cursor in the new List is undefined,
    // regardless of the states of the cursors in A and B.
    // The states of A and B are unchanged.
    return A;
}
int equals(List A, List B){
    //int eq = 0;
    if( A==NULL || B==NULL ){
        printf("Queue Error: calling equals() on NULL Queue reference\n");
        return(-1);
    }
    if(A->length!=B->length)return 0;
    moveFront(A);
    moveFront(B);
    
    for(int i=0;i<A->length;i++){
        if(get(A)!=get(B))return (0);
        moveNext(A);
        if(A!=B)
            moveNext(B);
    }
    return 1;
}
void set(List L, int x){
    
    L->cursor->data=x;
}
