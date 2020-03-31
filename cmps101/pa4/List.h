//MARK MORENO
//1742618
//pa4
//maalmore
#ifndef List_h
#define List_h

#include <stdio.h>


typedef struct ListObj* List;


int isEmpty(List Q);
List newList(void);
void freeList(List*pL);

int length(List L);
int index(List L);
int front(List L);
int back(List L);
int get(List L);
void clear(List L);
void moveFront(List L);
void moveBack(List L);
void movePrev(List L);
void moveNext(List L);
void prepend(List L, int data);
void append(List L, int data);
void insertBefore(List L, int data);
void insertAfter(List L, int data);
void deleteFront(List L);
void deleteBack(List L);
void delete(List L);
List copyList(List L);
void set(List L, int x);
int equals(List A, List B);
void printList(FILE* out,List L);
List concatList(List A, List B);

#endif /* List_h */