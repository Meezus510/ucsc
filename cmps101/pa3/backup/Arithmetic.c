//  main.c
//  longInts
//
//  Created by Mark Moreno on 11/6/19.
//  Copyright © 2019 Mark Moreno. All rights reserved.
//
//MARK MORENO
//1742618
//pa3
#include <stdio.h>
#include "BigInteger.h"

int main(int argc, const char * argv[]) {
    if (argc!=3){
     printf("Incorrect number of arguments: EXIT");
     exit(1);
     }
    FILE *f = fopen(argv[1], "r");
    //FILE *f = fopen("/Users/markmoreno/Desktop/test.txt", "r");
    
    //int ch, lines = 0;
    char *contents[4];
    size_t len = 0;
    for(int i = 0; i < 4; i++)
    {
        contents[i] = NULL;
        len = 0;
        getline(&contents[i], &len, f);
    }
    //FILE* fp = fopen ("/Users/markmoreno/Desktop/empty.txt","w");
    FILE* fp = fopen (argv[2],"w");
    /*for(int i=0;i<4;i++){
        printf("%s\n", contents[i]);
    }*/
    
    char*num1=contents[1];
    char*num2=contents[3];
    //printf("%s", num1);
    BigInteger A=stringToBigInteger(num1);
    //printBigInt(A);
    printBigInteger(fp, A);
    //printBigInteger(stdout, A);
    BigInteger B=stringToBigInteger(num2);
    //printBigInt(B);
    printBigInteger(fp, B);
    //printBigInteger(stdout, B);
    BigInteger C=sum(A, B);
    normalize(C);
    //printBigInt(B);
    //BigInteger G=equalize(B, 2, 100);
    //printBigInt(G);
    
    
    printBigInteger(fp, C);
    BigInteger D=diff(A, B);
    printBigInteger(fp, D);
    BigInteger E=diff(A,A);
    printBigInteger(fp, E);
    BigInteger Fa=newBigInteger();
    BigInteger Fb=newBigInteger();
    //printf("loop\n");
    for(int i=0;i<3;i++){
        printf("loop\n");
        add(Fa, Fa, A);
        //printBigInt(Fa);
    }
    normalize(Fa);
    printf("out of loop\n");
    for(int i =0;i<2;i++){
        add(Fb, Fb, B);
        //printBigInt(Fb);
    }
    normalize(Fb);
    subtract(Fa, Fa, Fb);
    printBigInteger(fp, Fa);
    
    //printBigInt(A);
    //printBigInt(B);
    //printf("mult: \n");
    
    BigInteger Alma=prod(A, B);//
    printBigInteger(fp, Alma);//
    //printBigInt(G);//
    //BigInteger Alma =prod2(A,B);
    //printBigInteger(fp, Alma);
    //printBigInt(Alma);
    //printBigInt(G);
    
    BigInteger H=prod(A, A);
    printBigInteger(fp, H);
    //printBigInt(H);
    BigInteger I=prod(B, B);
    printBigInteger(fp, I);
    //printBigInt(I);
    //printf("Begin: \n");
    BigInteger J=copy(A);
    //printBigInt(J);
    BigInteger K=copy(B);
    //printBigInt(K);
    for(int i=0;i<3;i++){
        multiply(J, J, A);
    }
    //printBigInt(J);
    BigInteger M=copy(J);
    for(int i=0;i<8;i++){
        add(M, M, J);
    }
    //printf("M\n");
    //printBigInt(M);
    normalize(M);
    for(int i=0;i<4;i++){
        multiply(K, K, B);
        //printBigInt(K);
    }
    //printBigInt(K);
    BigInteger N=copy(K);
    for(int i=0;i<15;i++){
        add(N, N, K);
    }
    //printBigInt(N);
    normalize(K);
    BigInteger L=sum(M, N);
    normalize(L);
    printBigInteger(fp, L);
    
    //printBigInt(L);
    
    printf("\n");
    //FILE* fp = fopen ("/Users/markmoreno/Desktop/empty.txt","w");
    //FILE* fp = fopen (argv[2],"w");
    fclose(f);
    fclose (fp);
    return 0;
}