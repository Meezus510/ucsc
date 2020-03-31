//  main.c
//  longInts
//
//  Created by Mark Moreno on 11/6/19.
//  Copyright © 2019 Mark Moreno. All rights reserved.
//

#include <stdio.h>
#include "BigInteger.h"

int main(int argc, const char * argv[]) {
    
    BigInteger BI = stringToBigInteger("123456789111");
    printBigInteger(stdout, BI);
    
    BigInteger BI2 = stringToBigInteger("9876543211");
    printBigInteger(stdout, BI2);
    printf("compare BI BI2 %d\n",compare(BI, BI2));
    
    BigInteger BI3 = stringToBigInteger("1233456789111");
    printBigInteger(stdout, BI3);
    printf("equals BI BI3 %d\n",equals(BI, BI3));
    
    BigInteger BI4=copy(BI2);
    printBigInteger(stdout, BI4);
    printf("compare BI4 BI2 %d\n",compare(BI4, BI2));
    
    BigInteger BI5=sum(BI,BI3);
    printBigInteger(stdout, BI);
    printf("BI3: ");
    printBigInteger(stdout, BI3);
    printf("BI5: ");
    printBigInteger(stdout, BI5);
    
    BigInteger BI6=diff(BI5,BI3);
    printf("BI6: ");
    printBigInteger(stdout, BI6);
    printf("BI5: ");
    printBigInteger(stdout, BI5);
    BigInteger BI7=diff(BI6,BI5);
    printf("BI6: ");
    printBigInteger(stdout, BI6);
    printf("BI5: ");
    printBigInteger(stdout, BI5);
    printf("BI7: ");
    printBigInteger(stdout, BI7);
    freeBigInteger(&BI3);
    BigInteger BI9=copy(BI6);
    BigInteger BI8=diff(BI6,BI9);
    printf("BI6: ");
    printBigInteger(stdout, BI6);
    printf("BI9: ");
    printBigInteger(stdout, BI9);
    printf("BI8: ");
    printBigInteger(stdout, BI8);
    printBigInt(BI6);
    subtract(BI6, BI7, BI5);
    printf("BI6: ");
    printBigInteger(stdout, BI6);
    printBigInt(BI6);
    add(BI6, BI6, BI6);
    printBigInteger(stdout, BI6);
    BigInteger N2=stringToBigInteger("123456");
    BigInteger N3=stringToBigInteger("567891");
    BigInteger N1=prod(N2, N3);
    printBigInteger(stdout, N1);
    BigInteger N4=stringToBigInteger("123456789101112131415");
    BigInteger N5=stringToBigInteger("123456789101112000000");
    BigInteger N6=diff(N4, N5);
    printBigInt(N6);
    normalize(N6);
    printBigInt(N6);
    printBigInteger(stdout, N6);
    
    return 0;
}