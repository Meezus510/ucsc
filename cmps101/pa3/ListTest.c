//MARK MORENO
//1742618
//pa1
//maalmore
#include<stdio.h>
#include<stdlib.h>
#include"List.h"

int main(int argc, char* argv[]){
    
    List A = newList();
    List B = newList();
    List C = NULL;
    long i;
    
    for(i=100000000; i<=10000000000000; i=i+123456789056){
        append(A,i);
        prepend(B,i);
    }
    
    printList(stdout,A);
    printf("\n");
    printList(stdout,B);
    printf("\n");
    
    for(moveFront(A); index(A)>=0; moveNext(A)){
        printf("%ld ", get(A));
    }
    printf("\n");
    for(moveBack(B); index(B)>=0; movePrev(B)){
        printf("%ld ", get(B));
    }
    printf("\n");
    
    C = copyList(A);
    
    
    
    moveFront(A);
    for(i=0; i<5; i++) moveNext(A); // at index 5
    insertBefore(A, -112345678);            // at index 6
    for(i=0; i<9; i++) moveNext(A); // at index 15
    insertAfter(A, -234567890);
    for(i=0; i<5; i++) movePrev(A); // at index 10
    delete(A);
    printList(stdout,A);
    printf("\n");
    printf("%d\n", length(A));
    clear(A);
    printf("%d\n", length(A));
    
    freeList(&A);
    freeList(&B);
    freeList(&C);
    
    return(0);
}