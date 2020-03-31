//  BigInteger.c
//  longInts
//
//  Created by Mark Moreno on 11/6/19.
//  Copyright © 2019 Mark Moreno. All rights reserved.
//
//MARK MORENO
//1742618
//pa1
//maalmore
#include "List.h"
#include "BigInteger.h"
#include<stdio.h>
#include<stdlib.h>

typedef struct BigIntegerObj* BigInteger;
typedef struct BigIntegerObj{
    int signValue;
    int pValue;
    long tenth;
    List L;
} BigIntegerObj;

BigInteger newBigInteger(){
    BigInteger BI;
    BI= malloc(sizeof(BigIntegerObj));
    BI->L=malloc(sizeof(BI->L));
    BI->L=newList();
    BI->signValue=0;
    BI->pValue=0;
    BI->tenth=0;
    return BI;
}
void override(BigInteger A, BigInteger B){
    makeZero(A);
    A->pValue=B->pValue;
    A->signValue=B->signValue;
    A->tenth=B->tenth;
    moveFront(B->L);
    for(int i=0;i<length(B->L);i++){
        append(A->L, get(B->L));
        moveNext(B->L);
    }
    //printBigInt(A);
}
void normalize(BigInteger N){
    
    long base=length(N->L)*N->pValue;
    int p=1;
    int n =10;
    int n_=1;
    while((n-base)<0){
        n_=n_*10;
        n=n*10;
        p=p+1;
    }
    BigInteger H = equalize(N, p, n);
    //printBigInt(H);
    override(N, H);
    freeBigInteger(&H);
    
    
}
BigInteger equalize(BigInteger N, int p, long n){
    long n_=1;
    //printf("p %d")
    BigInteger H = newBigInteger();
    
    //printf("n:%d p:%d\n",n_,p);
    long multiple=0;
    int count=1;
    int x10=10;
    long prev=0;
    int iterate=1;
    long temp=0;
    moveBack(N->L);
    for(int i=0;i<length(N->L)*N->pValue;i++){
        for(int k=1;k<count;k++){
            x10=x10*10;
        }//1234
        if(count<=N->pValue){
            multiple=(((get(N->L)%x10)-prev)/(x10/10));
            
            prev=(get(N->L)%x10);
            count++;
            //printf("1[%ld - %d - %d]\n",multiple,count,x10);//
            x10=10;
        }else{
            movePrev(N->L);
            count=1;
            x10=10;
            prev=0;
            multiple=((get(N->L)%x10)-prev);
            prev=(get(N->L)%x10);
            count++;
            //printf("2[%ld - %d]\n",multiple,count);//
        }
        
        if(iterate==p){
            temp=temp+(multiple*n_);
            prepend(H->L, temp);
            temp=0;
            n_=1;
            iterate=1;
        }else{
            temp=temp+multiple*n_;
            n_=n_*10;
            iterate++;
        }
        //printf("temp %ld\n",temp);//
    }
    if(temp!=0)prepend(H->L, temp);
    H->tenth=n;
    H->pValue=p;
    H->signValue=N->signValue;
    return H;
}
void freeBigInteger(BigInteger* pN){
    BigInteger I = *pN;
    if( pN!=NULL && *pN!=NULL ){
        List temp = I->L;
        freeList(&temp);
        temp=NULL;
        free(*pN);
        *pN = NULL;
    }
    
}

int sign(BigInteger N){
    if(N->signValue==-1)return -1;
    else if(N->signValue==1)return 1;
    else return 0;
}
int compare(BigInteger A, BigInteger B){
    if(A->pValue>B->pValue)return 1;
    else if(A->pValue<B->pValue)return -1;
    else if(sign(A)==1&&sign(B)<1)return 1;
    else if(sign(B)==1&&sign(A)<1)return -1;
    else if(length(A->L)>length(B->L))return 1;
    else if(length(A->L)<length(B->L))return -1;
    else if(length(A->L)==length(B->L)){
        moveFront(A->L);
        moveFront(B->L);
        long getA=0;
        long getB=0;
        for(int i =0;i<length(A->L);i++){
            getA=get(A->L);
            getB=get(B->L);
            if(getA==getB){
                moveNext(A->L);
                if(A!=B)
                    moveNext(B->L);
            }else break;
        }
        
        //printf("%ld>%ld\n",getA,getB);
        if(getA>getB&&A->signValue!=-1)return 1;
        else if(getA<getB&&A->signValue!=-1)return -1;
        else if(getA<getB&&A->signValue==-1)return 1;
        else if(getA>getB&&A->signValue==-1)return -1;
        else return 0;
    }
    else return 0;
}

int equals(BigInteger A, BigInteger B){
    int eq=1;
    if(A->pValue!=B->pValue)return 0;
    else if(A->signValue!=B->signValue)return 0;
    else if(length(A->L)!=length(B->L))return 0;
    else{
        for(int i=0;i<length(A->L);i++){
            if(get(A->L)!=get(B->L)){
                eq=0;
                break;
            }
        }
        if(eq==1)return 1;
        else return 0;
    }
}
void makeZero(BigInteger N){
    clear(N->L);
    N->pValue=0;
    N->signValue=0;
    N->tenth=0;
}

void negate(BigInteger N){
    if(N->signValue==-1)N->signValue=1;
    else if(N->signValue==1)N->signValue=-1;
}

BigInteger stringToBigInteger(char* s){
    BigInteger BI=newBigInteger();
    int len =0;
    while(s[len]!='\0'&&s[len]!='\n'){
        len++;
    }
    //printf("length=%d\n",len);
    int p=1;
    long n =10;
    while((n-len)<=0){
        n=n*10;
        p=p+1;
        
    }
    BI->tenth=n;
    BI->pValue=p;
    //printf("%ld,%d\n",n,p);
    long count = len-1;
    //if(strncmp(s[len-1]),"\n")len--;
    while(count>=0){
        long tenth = 1;
        long temp=0;
        for(int i = 0;i<p;i++){
            
            if(count>=0){
                char tempChar = s[count];
                int sInt=atoi(&tempChar);
                //printf("s=%c s[%d]=%d\n",s[count],count,sInt);
                temp=temp+(sInt*tenth);
                count--;
            }
            tenth = tenth*10;
            //count--;
        }
        prepend(BI->L, temp);
    }
    while(front(BI->L)==0){
        //printf("%ld\n",front(N->L));
        if(front(BI->L)==0)deleteFront(BI->L);
    }
    char beg = s[0];
    if(beg=='-'){
        BI->signValue=-1;
    }else if(length(BI->L)==0){
        BI->signValue=0;
    }else BI->signValue=1;
    //printf("r=%d\n p=%d\n",n,p);
    normalize(BI);
    return BI;
}
BigInteger copy(BigInteger N){
    BigInteger A = newBigInteger();
    A->pValue=N->pValue;
    A->signValue=N->signValue;
    A->tenth=N->tenth;
    A->L=copyList(N->L);
    
    return A;
}

void add(BigInteger S, BigInteger A, BigInteger B){
    BigInteger N=sum(A,B);
    makeZero(S);
    moveFront(N->L);
    for(int i=0;i<length(N->L);i++){
        append(S->L, get(N->L));
        moveNext(N->L);
    }
    S->signValue=N->signValue;
    S->pValue=N->pValue;
    S->tenth=N->tenth;
    //printBigInt(S);
    freeBigInteger(&N);
}
void printBigInt(BigInteger N){
    printf("bigIntData{\n");
    printf("    tenth: %ld\n",N->tenth);
    printf("    sign: %d\n",N->signValue);
    printf("    P: %d\n",N->pValue);
    printf("    Num: ");
    printList(stdout, N->L);
    printf("}\n");
}

BigInteger sum(BigInteger A, BigInteger B){
    BigInteger N = newBigInteger();
    if(A->pValue!=0)N->pValue=A->pValue;
    else N->pValue=B->pValue;
    N->tenth=A->tenth;
    N->signValue=A->signValue;
    if(A->pValue>B->pValue){
        B=equalize(B, A->pValue, A->tenth);
    }else if(B->pValue>A->pValue){
        A=equalize(A, B->pValue, B->tenth);
    }
    if(length(A->L)==0&&length(B->L)!=0){
        freeBigInteger(&N);
        N=copy(B);
        return N;
    }
    else if(length(A->L)!=0&&length(B->L)==0){
        freeBigInteger(&N);
        N=copy(A);
        return N;
    }
    else if(A->signValue==1&&B->signValue==-1){
        freeBigInteger(&N);
        negate(B);
        N=diff(A,B);
        negate(B);
        return N;
    }else if(A->signValue==-1&&B->signValue==1){
        freeBigInteger(&N);
        negate(A);
        //printBigInt(A);
        //printBigInt(B);
        //printf("In here");
        N=diff(B,A);
        negate(A);
        return N;
    }else if(A->signValue==-1&&B->signValue==-1)N->signValue=-1;
    else N->signValue=1;
    moveBack(A->L);
    moveBack(B->L);
    long lengthA=length(A->L);
    long lengthB=length(B->L);
    long longest;
    if(lengthA>lengthB)longest=lengthA;
    else longest = lengthB;
    long tempInt;
    long carry=0;
    for(int i =0;i<longest;i++){
        if((i<lengthA)&&(i<lengthB)){
            tempInt=get(A->L)+get(B->L)+carry;
            if(tempInt>A->tenth-1){
                carry=1;
                tempInt=tempInt-A->tenth;
            }else carry=0;
            prepend(N->L, tempInt);
            movePrev(A->L);
            if(A!=B)
                movePrev(B->L);
        }else if(i<lengthA&&i>=lengthB){
            tempInt=get(A->L)+carry;
            if(tempInt>A->tenth-1){
                carry=1;
                tempInt=tempInt-A->tenth;
            }
            else carry=0;
            prepend(N->L, tempInt);
            movePrev(A->L);
        }else if(i>=lengthA&&i<lengthB){
            tempInt=get(B->L)+carry;
            if(tempInt>A->tenth-1){
                carry=1;
                tempInt=tempInt-A->tenth;
            }else carry=0;
            prepend(N->L, tempInt);
            movePrev(B->L);
        }
    }
    //N->tenth=A->tenth;
    //N->pValue=A->pValue;
    if(carry>0)prepend(N->L, carry);//maybe
    //printBigInt(N);
    //normalize(N);
    return N;
}

void subtract(BigInteger D, BigInteger A, BigInteger B){
    BigInteger N=diff(A,B);
    makeZero(D);
    moveFront(N->L);
    for(int i=0;i<length(N->L);i++){
        append(D->L, get(N->L));
        moveNext(N->L);
    }
    D->signValue=N->signValue;
    D->pValue=N->pValue;
    D->tenth=N->tenth;
    //printBigInt(D);
    freeBigInteger(&N);
}

BigInteger diff(BigInteger A, BigInteger B){
    BigInteger N=newBigInteger();
    N->pValue=A->pValue;
    N->tenth=A->tenth;
    List tempA = NULL;
    List tempB = NULL;
    //printBigInt(A);
    //printBigInt(B);
    if(A->pValue>B->pValue){
        B=equalize(B, A->pValue, A->tenth);
    }else if(B->pValue>A->pValue){
        A=equalize(A, B->pValue, B->tenth);
    }
    //printf("\ncomp%d\n",compare(A, B));
    if(compare(A,B)==1){
        if(A->signValue==1&&B->signValue==-1){
            negate(B);
            freeBigInteger(&N);
            N=sum(A, B);
            negate(B);
            N->signValue=1;
            return N;
        }else if(A->signValue==1&&B->signValue==0){
            freeBigInteger(&N);
            N=copy(A);
            N->signValue=1;
            return N;
        }else if(A->signValue==0&&B->signValue==-1){
            freeBigInteger(&N);
            N=copy(B);
            N->signValue=-1;
            return N;
        }else if(A->signValue==-1&&B->signValue==-1){
            tempA=B->L;
            tempB=A->L;
            N->signValue=1;
            return N;
        }else{
            tempA=A->L;
            tempB=B->L;
            N->signValue=1;
        }
    }else if(compare(A,B)==-1){
        if(A->signValue==-1&&B->signValue==1){
            //printf("inheeeere\n");
            negate(A);
            //printBigInt(A);
            //printBigInt(B);
            freeBigInteger(&N);
            N=sum(A, B);
            N->signValue=-1;
            negate(A);
            return N;
        }else if(A->signValue==0&&B->signValue==1){
            freeBigInteger(&N);
            N=copy(A);
            negate(N);
            return N;
        }else if(A->signValue==-1&&B->signValue==0){
            freeBigInteger(&N);
            N=copy(A);
            return N;
        }else if(A->signValue==-1&&B->signValue==-1){
            tempA=A->L;
            tempB=B->L;
            N->signValue=-1;
        }else{
            tempB=A->L;
            tempA=B->L;
            N->signValue=-1;
        }
    }else{
        //printf("in here\n");
        N=newBigInteger();
        return N;
    }
    moveBack(tempA);
    moveBack(tempB);
    long lengthA=length(tempA);
    long lengthB=length(tempB);
    long longest;
    if(lengthA>lengthB)longest=lengthA;
    else longest = lengthB;
    long tempInt;
    long carry=0;
    for(int i =0;i<longest;i++){
        
        if((i<lengthA)&&(i<lengthB)){
            tempInt=get(tempA)-get(tempB)+carry;
            if(get(tempA)<get(tempB)){
                carry=-1;
                tempInt=tempInt+A->tenth;
                //printf("%d\n",A->tenth);
            }else carry=0;
            prepend(N->L, tempInt);
            movePrev(tempA);
            if(A!=B)
                movePrev(tempB);
        }else if(i<lengthA&&i>=lengthB){
            tempInt=get(tempA)+carry;
            carry=0;
            prepend(N->L, tempInt);
            movePrev(tempA);
        }else if(i>=lengthA&&i<lengthB){
            tempInt=get(tempB)+carry;
            carry=0;
            N->signValue=-1;//takeout
            prepend(N->L, tempInt);
            movePrev(tempB);
        }
    }
    while(front(N->L)==0){
        //printf("%ld\n",front(N->L));
        if(front(N->L)==0)deleteFront(N->L);
    }
    normalize(N);
    return N;
}

void multiply(BigInteger P, BigInteger A, BigInteger B){
    BigInteger N=prod(A,B);
    makeZero(P);
    moveFront(N->L);
    for(int i=0;i<length(N->L);i++){
        append(P->L, get(N->L));
        moveNext(N->L);
    }
    P->signValue=N->signValue;
    P->pValue=N->pValue;
    P->tenth=N->tenth;
    //printBigInt(P);
    freeBigInteger(&N);
}
BigInteger prod(BigInteger A, BigInteger B){
    BigInteger N = newBigInteger();
    if(A->pValue<9){
        A=equalize(A, 9, 1000000000);
    }
    if(A==B){
        B=copy(A);
    }
    
    if(A->pValue>B->pValue){
        B=equalize(B, A->pValue, A->tenth);
    }else if(B->pValue>A->pValue){
        A=equalize(A, B->pValue, B->tenth);
    }
    N->pValue=A->pValue;
    N->signValue=1;
    N->tenth=A->tenth;
    BigInteger tempN = newBigInteger();
    tempN->pValue=A->pValue;
    tempN->signValue=1;
    tempN->tenth=A->tenth;
    BigInteger temper=newBigInteger();
    temper->pValue=A->pValue;
    temper->signValue=1;
    temper->tenth=A->tenth;
    //printf("b");
    //printBigInt(B);
    //printBigInt(A);
    long n;
    long multiple=0;
    //int count=1;
    //int x10=10;
    //long prev=0;
    moveBack(B->L);
    for(int i=0;i<length(B->L);i++){
        clear(temper->L);
        /*for(int k=1;k<count;k++){
         x10=x10*10;
         *///1234
        multiple=get(B->L);
        moveBack(A->L);
        //printf("B[%ld]\n",multiple);//
        //clearr(temper->L);
        long multiple2=0;
        //int count2=1;
        
        //long prev2=0;
        //printBigInt(A);
        //printBigInt(B);
        for(int j=0;j<(length(A->L));j++){
            clear(tempN->L);
            //1234
            multiple2=get(A->L);
            n=multiple*multiple2;
            long x10=A->tenth;
            long prev=0;
            //printf("n=%ld\n",n);
            while(x10>0){
                
                long temp=0;
                //printf("x10 %ld %ld %ld\n",x10,n-(x10/A->tenth),(x10/A->tenth));
                temp=n%x10;
                long tmp=temp-prev;
                prepend(tempN->L, tmp/(x10/A->tenth));
                prev=temp;
                x10=x10*A->tenth;
                //printf("temp=%ld tmp=%ld prev=%ld A->t=%ld\n",temp,tmp,prev,A->tenth);
            }
            //printf("**[%ld]\n",n);
            //printf("*[%ld %ld]\n",(n/A->tenth) ,n%A->tenth);//
            //if(n/A->tenth!=0)append(tempN->L, n/A->tenth);
            //append(tempN->L, n%A->tenth);
            long zeros=(length(A->L)-index(A->L))+(length(B->L)-index(B->L)-2);
            for(int h=0;h<zeros;h++){
                append(tempN->L, 0);
                //printf("0");
            }//printf("\n");
            //printf("A[%ld]\n",multiple2);//
            //printf("tempN ");
            //printBigInt(tempN);
            add(temper, temper, tempN);
            movePrev(A->L);
            //printf("temper\n");//
            //printBigInt(temper);
        }
        //printf("----------\n");//
        //printf("%ld x %ld\n",multiple,multiple2);
        //printBigInt(temper);
        //printf("before N\n");//
        //printBigInt(N);
        add(N,N,temper);
        movePrev(B->L);
        //printf("N\n");//
        //printBigInt(N);
    }
    //printBigInt(N);
    normalize(N);
    if(A->signValue==-1&&B->signValue==1)N->signValue=-1;
    else if(A->signValue==1&&B->signValue==-1)N->signValue=-1;
    else if(A->signValue==-1&&B->signValue==-1)N->signValue=1;
    else N->signValue=1;
    freeBigInteger(&A);
    if(A==B)freeBigInteger(&B);
    freeBigInteger(&tempN);
    freeBigInteger(&temper);
    return N;
}

void printBigInteger(FILE* out, BigInteger N){
    if(length(N->L)==0)fprintf(out,"0");
    else{
        while(front(N->L)==0){
            //printf("%ld\n",front(N->L));
            if(front(N->L)==0)deleteFront(N->L);
        }
        if(N->signValue==-1)fprintf(out,"-");
        moveFront(N->L);
        for(int i=0;i<length(N->L);i++){
            long ten=(N->tenth/10);
            for(int j=0;j<N->pValue;j++){
                if(((get(N->L)-ten)<0)&&index(N->L)!=0){
                    if(get(N->L)!=0)
                        fprintf(out,"0");
                    
                    ten=ten/10;
                }
            }
            if(get(N->L)==0&&N->pValue>1)fprintf(out,"0");
            fprintf(out,"%ld",get(N->L));
            moveNext(N->L);
        }
        
    }
    fprintf(out,"\n\n");
    //printList(stdout, N->L);
}
