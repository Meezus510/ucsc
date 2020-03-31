//  Created by Mark Moreno on 10/16/19.
//  Copyright © 2019 Mark Moreno. All rights reserved.
//MARK MORENO
//1742618
//pa2
//maalmore

#ifndef Matrix_h
#define Matrix_h

#include <stdio.h>
typedef struct MatrixObj* Matrix;
// newMatrix()
Matrix newMatrix(int n);
// freeMatrix()
 void freeMatrix(Matrix* pM);
// Access functions
// size()
// Return the size of square Matrix M.
int size(Matrix M);
// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M);
// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise. int equals(Matrix A, Matrix B);
// Manipulation procedures
// makeZero()
// Re-sets M to the zero Matrix.
void makeZero(Matrix M);
// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x);
// Matrix Arithmetic operations
// copy()
// Returns a reference
Matrix copy(Matrix A);
// transpose()
// Returns a reference
// of A.
Matrix transpose(Matrix A);
// scalarMult()
// Returns a reference to a new
Matrix scalarMult(double x, Matrix A);
// sum()
// Returns a reference to a new
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B);
// diff()
// Returns a reference to a new
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B);
// product()
// Returns a reference to a new Matrix
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B);
/*to a new
Matrix
Matrix
object having the same entries as A.
object representing the transpose
object representing xA.
object representing A+B.
object representing A-B.
object representing AB
to a new
Matrix
Matrix
// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows // are not printed. Each non-zero is represented as one line consisting // of the row number, followed by a colon, a space, then a space separated // list of pairs "(col, val)" giving the column numbers and non-zero values // in that row. The double val will be rounded to 1 decimal point.*/
void printMatrix(FILE* out, Matrix M);
int equals(Matrix A, Matrix B);

#endif /* Matrix_h */