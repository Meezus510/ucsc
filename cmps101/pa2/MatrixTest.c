//MARK MORENO
//1742618
//pa2
//maalmore
//MatrixTest
//-----------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include"List.h"
#include "Matrix.h"

int main(int argc, char* argv[]){
    srand((time(0)));
    
    Matrix A=newMatrix(5);
    Matrix B=newMatrix(5);
    Matrix C=NULL;
    Matrix D=NULL;
    Matrix E=NULL;
    Matrix F=NULL;
    Matrix G=NULL;
    Matrix H=NULL;
    double arr1[25];
    double arr2[25];
    
    for(int i=0;i<25;i++){
        arr1[i]=rand()%10;
        arr2[i]=rand()%10;
        
    }
    int count=0;
    for(int i = 1; i<=5;i++){
        for(int j = 1;j<=5;j++){
            changeEntry(A, i, j, (arr1[count]));
            changeEntry(B, i, j, (arr2[count]));
            count++;
        }
    }
    printf("Matrix A - NZ: %d\n",NNZ(A));
    printMatrix(stdout, A);
    
    printf("Matrix A - NZ: %d\n",NNZ(B));
    printMatrix(stdout, B);
    
    C = sum(A, B);
    printf("Matrix C - sum(A,B) - NZ: %d\n",NNZ(C));
    printMatrix(stdout, C);
    
    D = diff(A, B);
    printf("Matrix D - diff(A,B) - NZ: %d\n",NNZ(D));
    printMatrix(stdout, D);
    
    makeZero(C);
    printf("Matrix C - makeZero - NZ: %d\n",NNZ(C));
    printMatrix(stdout, C);
    
    E = scalarMult(.72, D);
    printf("Matrix E - scalarMult(.7,D) - NZ: %d\n",NNZ(E));
    printMatrix(stdout, E);
    
    F = product(A, E);
    printf("Matrix F - product(A,E) - NZ: %d\n",NNZ(F));
    printMatrix(stdout, F);
    
    G = copy(F);
    printf("Matrix G - copy(F) - NZ: %d\n",NNZ(G));
    printMatrix(stdout, G);
    
    H = transpose(F);
    printf("Matrix F - transpose(F) - NZ: %d\n",NNZ(H));
    printMatrix(stdout, H);
    
    printf("Calling Equals:\n");
    if(equals(F, G)){
        printf("F == G\n");
    }else printf("F != G\n");
    if(equals(A, B)){
        printf("A == B\n");
    }else printf("A != B\n");
    
    return(0);
}