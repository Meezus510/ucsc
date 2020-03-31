//  Matrix.c
//  Matrix
//
//  Created by Mark Moreno on 10/16/19.
//  Copyright © 2019 Mark Moreno. All rights reserved.
//MARK MORENO
//1742618
//pa2
//maalmore

#include "Matrix.h"
#include "List.h"
#include <stdio.h>
#include<stdlib.h>

typedef struct EntryObj{
    int J;
    double value;
} EntryObj;

// private Node type
typedef EntryObj* Entry;

// private QueueObj type
typedef struct MatrixObj{
    //List *rowList[]; //list of lists
    int NZ;
    int Size;
    List *rowList;
} MatrixObj;

Entry newEntry(int j, double v){
    Entry E = malloc(sizeof(EntryObj));
    E->J = j;
    E->value = v;;
    return(E);
}
void freeEntry(Entry* pE){
    if( pE!=NULL && *pE!=NULL ){
        free(*pE);
        *pE = NULL;
    }
}

Matrix newMatrix(int n){
    Matrix M=malloc(sizeof(MatrixObj));
    M->rowList=malloc((n+1)*sizeof(*M->rowList));
    M->Size=n;
    M->NZ=0;
    for(int i =0; i <= n; i++){
        
        M->rowList[i]=NULL;
        M->rowList[i]=newList();
    }
    return M;
    
}

void freeMatrix(Matrix* pM){
    
    Matrix temp = *pM;
    if(pM!=NULL&&*pM!=NULL){
        for(int i =0;i<=temp->Size;i++){
            List L=temp->rowList[i];
            if(length(L)!=0){
                moveFront(L);
                while(get(L)!=NULL){
                    Entry tempE = get(L);
                    freeEntry(&tempE);
                    tempE=NULL;
                    moveNext(L);
                    deleteFront(L);
                }
            }
            
            free(temp->rowList[i]);
            temp->rowList[i]=NULL;
        }
        free(temp->rowList);
        temp->rowList=NULL;
        free(*pM);
        *pM=NULL;
        
    }
}

int size(Matrix M){
    return M->Size;
}

int NNZ(Matrix M){
    return M->NZ;
}

void makeZero(Matrix M){
    if(M!=NULL){
        for(int i =0;i<=M->Size;i++){
            if(!isEmpty(M->rowList[i])){
                List L=M->rowList[i];
                moveFront(L);
                while(get(L)!=NULL){
                    Entry tempE = get(L);
                    freeEntry(&tempE);
                    moveNext(L);
                }
                clear(L);
                L=NULL;
            }
        }
        M->NZ=0;
    }
}

void changeEntry(Matrix M, int i, int j, double x){
    List A=M->rowList[i];
    
    //printf("length: %d",length(A));
    if(length(A)==0){
        Entry E = newEntry(j, x);
        if(E->value!=0){
            append(A, E);
            M->NZ++;
        }
        else freeEntry(&E);
    }
    else if(!isEmpty(A)){
        Entry E = newEntry(j, x);
        int found = -1;
        moveFront(A);
        while(get(A)!=NULL){    //O(n)
            Entry temp = get(A);
            //printf("tempj %d\n", temp->J);
            if(j<temp->J){
                //printf("E-j:%d x:%.02f",E->J,E->value);
                if(E->value!=0){
                    insertBefore(A, E);
                    M->NZ++;
                }
                else freeEntry(&E);
                found =1;
                break;
            }
            else if(j==temp->J){
                if(x==0){
                    delete(A);
                    freeEntry(&temp);
                    M->NZ--;
                }
                else temp->value=x;
                freeEntry(&E);
                found=1;
                break;
            }
            moveNext(A);
        }
        if(found<0){
            if(E->value!=0){
                append(A, E);
                M->NZ++;
            }
            else freeEntry(&E);
        }
    }
}

Matrix copy(Matrix A){
    Matrix B = newMatrix(A->Size);
    B->NZ=A->NZ;
    B->Size=A->Size;
    for(int i =0; i<=A->Size;i++){  //O(n)
        if(!isEmpty(A->rowList[i])){
            List tempL = A->rowList[i];
            moveFront(tempL);
            while(get(tempL)!=NULL){    //O(a)
                Entry tempE = get(tempL);
                Entry E = newEntry(tempE->J, tempE->value);
                append(B->rowList[i], E);
                moveNext(A->rowList[i]);
            }
        }
    }
    return B;
}

Matrix transpose(Matrix A){
    Matrix B = newMatrix(A->Size);
    B->NZ=0;
    B->Size=A->Size;
    for(int i=0;i<=A->Size; i ++){  //O(n)
        if(!isEmpty(A->rowList[i])){
            List tempL=A->rowList[i];
            moveFront(tempL);
            while(get(tempL)!=NULL){    //O(a)
                Entry tempE = get(tempL);
                Entry E = newEntry(i, tempE->value);
                if(E->value!=0){
                    append(B->rowList[tempE->J], E);
                    B->NZ++;
                }
                else freeEntry(&E);
                moveNext(tempL);
            }
        }
    }
    
    return B;
}

Matrix scalarMult(double x, Matrix A){
    Matrix B = newMatrix(A->Size);
    B->NZ=0;
    B->Size=A->Size;
    for(int i =0;i<=A->Size;i++){   //O(n)
        if(!isEmpty(A->rowList[i])){
            List L = A->rowList[i];
            moveFront(L);
            while(get(L)!=NULL){    //O(a)
                Entry E = get(L);
                Entry tempE = newEntry(E->J, ((E->value)*x));
                if(E->value!=0){
                    append(B->rowList[i], tempE);
                    B->NZ++;
                }
                else freeEntry(&E);
                moveNext(L);
            }
        }
    }
    return B;
}
Matrix sum(Matrix A, Matrix B){
    if(A->Size!=B->Size){
        return NULL;
    }else{
        Matrix C = newMatrix(A->Size);
        C->NZ=0;
        C->Size=A->Size;
        for(int i = 1; i<=A->Size;i++){     //O(n)
            List AtempL = A->rowList[i];
            List BtempL = B->rowList[i];
            if(!isEmpty(A->rowList[i])&&!isEmpty(B->rowList[i])){
                //printf("%d in here1\n",i);
                
                moveFront(AtempL);
                moveFront(BtempL);
                
                while(get(AtempL)!=NULL||get(BtempL)!=NULL){    //O(a+b)
                    //printf("While\n");
                    
                    Entry AtempE = get(AtempL);
                    Entry BtempE = get(BtempL);
                    //printf("B v: %f\n",BtempE->value);
                    //printf("(I: %d %d, %.02f)\n ",i,BtempE->J,BtempE->value);
                    if(AtempE!=NULL&&BtempE==NULL){
                        Entry E = newEntry(AtempE->J, AtempE->value);
                        
                        if(E->value!=0){
                            append(C->rowList[i], E);
                            C->NZ++;
                        }
                        else freeEntry(&E);
                        moveNext(AtempL);
                    }else if(AtempE==NULL&&BtempE!=NULL){
                        Entry E = newEntry(BtempE->J, BtempE->value);
                        
                        if(E->value!=0){
                            append(C->rowList[i], E);
                            C->NZ++;
                        }
                        else freeEntry(&E);
                        moveNext(BtempL);
                    }else if((AtempE->J)==(BtempE->J)){
                        //printf("%d %d %d IF=\n",i,AtempE->J,BtempE->J);
                        double d = (AtempE->value)+(BtempE->value);
                        Entry E = newEntry(AtempE->J, d);
                        
                        if(E->value!=0){
                            append(C->rowList[i], E);
                            C->NZ++;
                        }
                        else freeEntry(&E);
                        if(AtempL!=BtempL){
                            moveNext(AtempL);
                            moveNext(BtempL);
                        }else if(AtempL==BtempL){
                            moveNext(AtempL);
                        }
                    }
                    else if((AtempE->J)!=(BtempE->J)){
                        
                        if(AtempE->J<BtempE->J){
                            //printf("IF<\n");
                            Entry E = newEntry(AtempE->J, AtempE->value);
                            
                            if(E->value!=0){
                                append(C->rowList[i], E);
                                C->NZ++;
                            }
                            else freeEntry(&E);
                            
                            moveNext(AtempL);
                        }else{
                            //printf("IF>\n");
                            Entry E = newEntry(BtempE->J, BtempE->value);
                            
                            if(E->value!=0){
                                append(C->rowList[i], E);
                                C->NZ++;
                            }
                            else freeEntry(&E);
                            
                            moveNext(BtempL);
                        }
                    }
                }
            }
            else if(!isEmpty(A->rowList[i])&&isEmpty(B->rowList[i])){
                //printf("%d in here2\n",i);
                
                List AtempL = A->rowList[i];
                moveFront(AtempL);
                while(get(AtempL)!=NULL){
                    Entry tempE = get(AtempL);
                    Entry E = newEntry(tempE->J, tempE->value);
                    append(C->rowList[i], E);
                    C->NZ++;
                    moveNext(AtempL);
                }
            }else if(!isEmpty(B->rowList[i])&&isEmpty(A->rowList[i])){
                //printf("%d in here3\n",i);
                
                
                List BtempL = B->rowList[i];
                moveFront(BtempL);
                while(get(BtempL)!=NULL){
                    Entry tempE = get(BtempL);
                    Entry E = newEntry(tempE->J, tempE->value);
                    append(C->rowList[i], E);
                    C->NZ++;
                    moveNext(BtempL);
                }
            }
        }
        return C;
    }
}


Matrix diff(Matrix A, Matrix B){
    if(A->Size!=B->Size){
        return NULL;
    }else{
        Matrix C = newMatrix(A->Size);
        C->Size=A->Size;
        C->NZ=0;
        for(int i = 0; i<=A->Size;i++){     //O(n)
            if(!isEmpty(A->rowList[i])&&!isEmpty(B->rowList[i])){
                
                List AtempL = A->rowList[i];
                List BtempL = B->rowList[i];
                moveFront(AtempL);
                moveFront(BtempL);
                while(get(AtempL)!=NULL||get(BtempL)!=NULL){    //O(a+b)
                    //printf("while %d",i);
                    Entry AtempE = get(AtempL);
                    Entry BtempE = get(BtempL);
                    if(AtempE!=NULL&&BtempE==NULL){
                        Entry E = newEntry(AtempE->J, (AtempE->value));
                        
                        if(E->value!=0){
                            append(C->rowList[i], E);
                            C->NZ++;
                        }
                        else freeEntry(&E);
                        moveNext(AtempL);
                    }else if(AtempE==NULL&&BtempE!=NULL){
                        Entry E = newEntry(BtempE->J, 0-(BtempE->value));
                        
                        if(E->value!=0){
                            append(C->rowList[i], E);
                            C->NZ++;
                        }
                        else freeEntry(&E);
                        moveNext(BtempL);
                    }else if((AtempE->J)==(BtempE->J)){
                        double d = (AtempE->value)-(BtempE->value);
                        Entry E = newEntry(AtempE->J, d);
                        
                        if(E->value!=0){
                            append(C->rowList[i], E);
                            C->NZ++;
                        }
                        else freeEntry(&E);
                        
                        if(AtempL!=BtempL){
                            moveNext(AtempL);
                            moveNext(BtempL);
                        }else if(AtempL==BtempL){
                            moveNext(AtempL);
                        }
                    }
                    else if((AtempE->J)!=(BtempE->J)){
                        if(AtempE->J<BtempE->J){
                            Entry E = newEntry(AtempE->J, AtempE->value);
                            
                            if(E->value!=0){
                                append(C->rowList[i], E);
                                C->NZ++;
                            }
                            else freeEntry(&E);
                            
                            moveNext(AtempL);
                        }else{
                            Entry E = newEntry(BtempE->J, 0-(BtempE->value));
                            
                            if(E->value!=0){
                                append(C->rowList[i], E);
                                C->NZ++;
                            }
                            else freeEntry(&E);
                            
                            moveNext(BtempL);
                        }
                    }
                }
            }else if(!isEmpty(A->rowList[i])&&isEmpty(B->rowList[i])){
                
                
                List AtempL = A->rowList[i];
                moveFront(AtempL);
                while(get(AtempL)!=NULL){
                    Entry tempE = get(AtempL);
                    Entry E = newEntry(tempE->J, tempE->value);
                    append(C->rowList[i], E);
                    C->NZ++;
                    moveNext(AtempL);
                }
            }else if(!isEmpty(B->rowList[i])&&isEmpty(A->rowList[i])){
                
                
                List BtempL = B->rowList[i];
                moveFront(BtempL);
                while(get(BtempL)!=NULL){
                    Entry tempE = get(BtempL);
                    Entry E = newEntry(tempE->J, -(tempE->value));
                    append(C->rowList[i], E);
                    C->NZ++;
                    moveNext(BtempL);
                }
            }
        }
        return C;
    }
    
}
double vectorDot(List P, List Q){
    double dot = 0;
    moveFront(P);
    moveFront(Q);
    while(get(P)!=NULL&&get(Q)!=NULL){    //O(a+b)
        Entry AtempE = get(P);
        Entry BtempE = get(Q);
        if((AtempE->J)==(BtempE->J)){
            double d = (AtempE->value)*(BtempE->value);
            dot = dot+d;
            moveNext(P);
            moveNext(Q);
        }
        else if((AtempE->J)!=(BtempE->J)){
            if(AtempE->J<BtempE->J||get(Q)==NULL){
                moveNext(P);
            }else{
                moveNext(Q);
            }
        }
    }
    
    return dot;
}
Matrix product(Matrix A, Matrix B){  //fix
    if(A->Size!=B->Size){
        return NULL;
    }else{
        Matrix C = newMatrix(A->Size);
        C->NZ=0;
        C->Size=A->Size;
        Matrix M = transpose(B);
        for(int i=0;i<=A->Size;i++){    //O(n)
            if(!isEmpty(A->rowList[i])){
                
                for(int j= 0;j<=M->Size;j++){   //O(n)
                    if(!isEmpty(M->rowList[j])){
                        double d = vectorDot(A->rowList[i], M->rowList[j]);
                        Entry E = newEntry(j, d);
                        
                        if(E->value!=0){
                            append(C->rowList[i], E);
                            C->NZ++;
                        }
                        else freeEntry(&E);
                        
                    }
                }
            }
        }
        freeMatrix(&M);
        return C;
    }
}
int equals(Matrix A, Matrix B){
    int eq = 0;
    
    if( A==NULL || B==NULL ){
        printf("Queue Error: calling equals() on NULL Queue reference\n");
        return(-1);
    }
    
    eq = ( A->NZ == B->NZ && A->Size==B->Size);
    //printf("%d %d\n",A->NZ,B->NZ);
    for(int i = 0; i<=A->Size&&eq;i++){
        if((A->rowList[i]!=NULL&&!isEmpty(A->rowList[i]))
           ||(B->rowList[i]!=NULL&&!isEmpty(B->rowList[i]))){
            eq=(length(A->rowList[i])==length(B->rowList[i]));
            moveFront(A->rowList[i]);
            moveFront(B->rowList[i]);
            while(get(A->rowList[i])!=NULL&&get(B->rowList[i])!=NULL&&eq){
                eq=(get(A->rowList[i])==get(B->rowList[i]));
                if(A->rowList[i]!=B->rowList[i]){
                    moveNext(A->rowList[i]);
                    moveNext(B->rowList[i]);
                }else if(A->rowList[i]==B->rowList[i]){
                    moveNext(A->rowList[i]);
                }
            }
            eq=(length(A->rowList[i])==length(B->rowList[i]));
            
        }
        
    }
    return eq;
}

void printMatrix(FILE* out, Matrix M){
    if(M!=NULL){
        //printf("Max: %d\n",M->Max);
        for(int i = 0; i <=M->Size; i++){
            if(M->rowList[i]!=NULL&&!isEmpty(M->rowList[i])){
                List A=M->rowList[i];
                if(!isEmpty(A)){
                    fprintf(out, "%d: ",i);
                    moveFront(A);
                    while(get(A)!=NULL){
                        Entry temp = get(A);
                        fprintf(out, "(%d, %.01f) ",temp->J,temp->value);
                        moveNext(A);
                    }
                    fprintf(out, "\n");
                    
                }
            }
        }
        fprintf(out, "\n");
    }
    else printf("Empty Matrix\n");
}
