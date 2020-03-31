//
//  BigInteger.h
//  longInts
//
//  Created by Mark Moreno on 11/6/19.
//  Copyright © 2019 Mark Moreno. All rights reserved.
//
//MARK MORENO
//1742618
//pa3
//maalmore

#ifndef BigInteger_h
#define BigInteger_h
#include<stdio.h>
#include<stdlib.h>

// Exported type -------------------------------------------------------------
// BigInteger reference type
typedef struct BigIntegerObj* BigInteger;


// Constructors-Destructors ---------------------------------------------------
// newBigInteger()
// Returns a reference to a new BigInteger object in the zero state.
BigInteger newBigInteger(void);
    
// freeBigInteger()
// Frees heap memory associated with *pN, sets *pN to NULL.
void freeBigInteger(BigInteger* pN);
// Access functions -----------------------------------------------------------
// sign()
// Returns -1 if N is negative, 1 if N is positive, and 0 if N is in the zero // state.
int sign(BigInteger N);
// compare()
// Returns -1 if A<B, 1 if A>B, and 0 if A=B.
int compare(BigInteger A, BigInteger B);
// equals()
// Return true (1) if A and B are equal, false (0) otherwise.
int equals(BigInteger A, BigInteger B);
// Manipulation procedures ----------------------------------------------------
// makeZero()
// Re-sets N to the zero state.
void makeZero(BigInteger N);
// negate()
// Reverses the sign of N: positive <--> negative. Does nothing if N is in the // zero state.
void negate(BigInteger N);
// BigInteger Arithmetic operations -----------------------------------------------
// stringToBigInteger()
// Returns a reference to a new BigInteger // represented in base 10 by the string s. // Pre: s is a non-empty string containing // and an optional sign {+, -} prefix.
BigInteger stringToBigInteger(char* s);
// copy()
// Returns a reference to a new BigInteger
BigInteger copy(BigInteger N);
//object representing the decimal integer only base ten digits {0,1,2,3,4,5,6,7,8,9}
//object in the same state as N.
// add()
// Places the sum of A and B in the existing BigInteger S, overwriting its // current state: S = A + B
void add(BigInteger S, BigInteger A, BigInteger B);
// sum()
// Returns a reference to a new BigInteger object representing A + B.
BigInteger sum(BigInteger A, BigInteger B);
// subtract()
// Places the difference of A and B in the existing BigInteger D, overwriting //itscurrentstate: D=A-B
void subtract(BigInteger D, BigInteger A, BigInteger B);
// diff()
// Returns a reference to a new BigInteger object representing A - B.
BigInteger diff(BigInteger A, BigInteger B);
// multiply()
// Places the product of A and B in the existing BigInteger P, overwriting // its current state: P = A*B
void multiply(BigInteger P, BigInteger A, BigInteger B);
// prod()
// Returns a reference to a new BigInteger object representing A*B
BigInteger prod(BigInteger A, BigInteger B);
// Other operations -----------------------------------------------------------
// printBigInteger()
// Prints a base 10 string representation of N to filestream out.
void printBigInteger(FILE* out, BigInteger N);
void printBigInt(BigInteger N);
void normalize(BigInteger N);
BigInteger equalize(BigInteger N, int p, long n);
BigInteger prod2(BigInteger A, BigInteger B);
#endif /* BigInteger_h */