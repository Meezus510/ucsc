//MARK MORENO
//1742618
//pa2
//maalmore
//ListTest
//-----------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include"List.h"
#include <time.h>

int main(int argc, char* argv[]){
    srand(time(0));
    List A = newList();
    List B = newList();
    List C = newList();
    
    int n[20];
    double d[20];
    for(int i=0;i<20;i++){
        n[i] = rand() % 100;
        d[i] = (rand() % 100)/9.69;
        append(A, &n[i]);
        prepend(B, &d[i]);
    }
    char a1[]="this";
    char a2[]="is";
    char a3[]="a";
    char a4[]="test";
    append(C, &a1);
    append(C, &a2);
    append(C, &a3);
    append(C, &a4);
   
    // works with char
    for(moveFront(C); index(C)>=0; moveNext(C)){
        printf("%s ", get(C));
    }
    printf("\n");
    for(moveBack(C); index(C)>=0; movePrev(C)){
        printf("%s ", get(C));
    }
    printf("\n");
    
    
    
    int i, u=-1, v=-2, equal=0;
    
    // initialize Lists A and B

    for(moveFront(A); index(A)>=0; moveNext(A)){
        printf("%d ", *(int*)get(A));
    }
    printf("\n");
    for(moveFront(B); index(B)>=0; moveNext(B)){
        printf("%.01f ", *(double*)get(B));
    }
    printf("\n");
    
    for(moveBack(A); index(A)>=0; movePrev(A)){
        printf("%d ", *(int*)get(A));
    }
    printf("\n");
    for(moveBack(B); index(B)>=0; movePrev(B)){
        printf("%.01f ", *(double*)get(B));
    }
    printf("\n");
    
    moveFront(A);
    while( index(A)>=0 ){
        append(C, get(A));
        moveNext(A);
    }
    // print the copy in forward direction
    
    
    // check shallow equality of A and C by comparing pointers
    equal = (length(A)==length(C));
    moveFront(A);
    moveFront(C);
    while( index(A)>=0 && equal){
        equal = ( get(A)==get(C) );
        moveNext(A);
        moveNext(C);
    }
    printf("A equals C is %s\n", (equal?"true":"false") );
    
    moveFront(A);
    for(i=0; i<5; i++) moveNext(A); // at index 5
    insertBefore(A, &u);            // now at index 6
    for(i=0; i<9; i++) moveNext(A); // at index 15
    insertAfter(A, &v);             // doesn't change index
    for(i=0; i<5; i++) movePrev(A); // at index 10
    delete(A);                      // index is now undefined
    
    // print A in forward and backward direction
    for(moveFront(A); index(A)>=0; moveNext(A)){
        printf("%d ", *(int*)get(A));
    }
    printf("\n");
    for(moveBack(A); index(A)>=0; movePrev(A)){
        printf("%d ", *(int*)get(A));
    }
    printf("\n");
    
    // check length of A, before and after clear()
    printf("%d\n", length(A));
    clear(A);
    printf("%d\n", length(A));
    
    freeList(&A);
    freeList(&B);
    freeList(&C);
    
    return(0);
}