//MARK MORENO
//1742618
//pa2
//maalmore
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include"List.h"
#include "Matrix.h"
int main(int argc, char* argv[]){
    if (argc!=3){
     printf("Incorrect number of arguments: EXIT");
     exit(1);
     }
    FILE *f = fopen(argv[1], "r");
    //FILE *f = fopen("/Users/markmoreno/Desktop/in4", "r");
    
    int n = 0, nzA = 0, nzB = 0;
    fscanf(f, "%d %d %d", &n, &nzA, &nzB);
    printf("%d %d %d\n", n, nzA, nzB);
    
    int row, col;
    double dec;
    
    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);
    Matrix C = NULL;
    Matrix D = NULL;
    Matrix E = NULL;
    Matrix F = NULL;
    Matrix G = NULL;
    Matrix H = NULL;
    Matrix I = NULL;
    Matrix J = NULL;
    for(int i=0;i<nzA;i++){
        fscanf(f, "%d %d %lf", &row, &col, &dec);
        changeEntry(A, row, col, dec);
    }
    
    for(int i=0;i<nzB;i++){
        printf("i: %d\n",i);
        fscanf(f, "%d %d %lf", &row, &col, &dec);
        changeEntry(B, row, col, dec);
    }
    FILE* fp = fopen (argv[2],"w");
    //FILE* fp = fopen ("/Users/markmoreno/Desktop/empty.txt","w");
    
    fprintf(fp, "A has %d non-zero entries:\n",nzA);
    printMatrix(fp, A);
    printMatrix(stdout, A);

    fprintf(fp, "B has %d non-zero entries:\n",nzB);
    printMatrix(fp, B);
    printMatrix(stdout, B);
    
    fprintf(fp, "(1.5)*A =\n");
    C=scalarMult(1.5, A);
    printMatrix(fp, C);
    printMatrix(stdout, C);
    
    fprintf(fp, "A+B =\n");
    D=sum(A, B);
    printMatrix(fp, D);
    printMatrix(stdout, D);
    
    fprintf(fp, "A+A =\n");
    E=sum(A, A);
    printMatrix(fp, E);
    printMatrix(stdout, E);
    
    fprintf(fp, "B-A =\n");
    F=diff(B, A);
    printMatrix(fp, F);
    printMatrix(stdout, F);
    
    fprintf(fp, "A-A =\n");
    G=diff(A, A);
    printMatrix(fp, G);
    printMatrix(stdout, G);
    
    fprintf(fp, "Transpose(A) =\n");
    H=transpose(A);
    printMatrix(fp, H);
    printMatrix(stdout, H);
    
    fprintf(fp, "A*B =\n");
    I=product(A, B);
    printMatrix(fp, I);
    printMatrix(stdout, I);
    
    fprintf(fp, "B*B =\n");
    J=product(B, B);
    printMatrix(fp, J);
    printMatrix(stdout, J);
    
    
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);
    freeMatrix(&E);
    freeMatrix(&F);
    freeMatrix(&G);
    freeMatrix(&H);
    freeMatrix(&I);
    freeMatrix(&J);
    
    fclose(f);
    fclose(fp);
    
    return 0;
}