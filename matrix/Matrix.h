#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <sstream>
#include <iomanip>

using namespace std;

#define alphabets "abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define numbers "-.0123456789"
#define PI 3.14159265359

#ifndef MATRIX_H
#define MATRIX_H

class Matrix
{
	bool notMatrix;
	unsigned int rows;
	unsigned int columns;
	double num;
	string name;
	double** twoDArray;

public:
	//Constructors and Destructors:
	Matrix();
	Matrix(double);
	Matrix(unsigned int, unsigned int);
	Matrix(const Matrix&);
	Matrix(string, string);
	Matrix(string name, string matrixString, vector<Matrix> & storedMatrices, vector<string>& systemCommands);
	~Matrix();
	void copyMatrix(const Matrix*);

	//Operators:
	Matrix& operator=(Matrix);
	Matrix& operator=(double);
	Matrix operator+(Matrix&);
	Matrix operator-(Matrix&);
	Matrix operator+(double);
	Matrix operator-(double);
	Matrix operator*(Matrix&);
	Matrix operator/(Matrix&);
	Matrix operator*(double);
	Matrix operator/(double);
	Matrix& operator-();

	//Setters and Getters:
	void setName(string);
	void setNum(double);
	void setSize(unsigned int, unsigned int);
	string getName();
	int getRows();
	int getColumns();
	double getNum();
	double getElement(unsigned int, unsigned int);


	//Operations:
	static Matrix* add(Matrix&, Matrix&);   //add made by '+' operator
	static Matrix* pseudoAdd(Matrix&, Matrix&);   //add made by '.+' operator

	static Matrix* subtract(Matrix&, Matrix&);             // -
	static Matrix* pseudoSubtract(Matrix&, Matrix&);       // .-

	static Matrix* product(Matrix&, Matrix&);              // *
	static Matrix* pseudoProduct(Matrix&, Matrix&);        // .*

	static Matrix* divide(Matrix&, Matrix&);               //  /
	static Matrix* pseudoDiv(Matrix&, Matrix&);            // ./

	static Matrix* powerMatrix(Matrix &, Matrix&);

	void addMatrixToMatrix(Matrix & x, unsigned int r, unsigned int c);

	Matrix* sqrtMatrix();
	Matrix* negative();
	Matrix* inverse();
	Matrix* transpose();
	Matrix* adjoint();
	Matrix* cofactor();

	double determinant(bool minor = false, unsigned posRow = 0, unsigned posCol = 0);

	void eye(unsigned int = 0, unsigned int = 0);
	void ones(unsigned int = 0, unsigned int = 0);
	void zeros(unsigned int = 0, unsigned int = 0);
	void random(unsigned int = 0, unsigned int = 0);

	Matrix* sinMatrix();
	Matrix* cosMatrix();
	Matrix* tanMatrix();
	Matrix* asinMatrix();
	Matrix* acosMatrix();
	Matrix* atanMatrix();

	Matrix* subMatrix(string);
	//Matrix* concatenate(Matrix&, Matrix&);

	void printMatrix(bool = true, unsigned int = 0, const vector<string>& = vector<string>());
	static Matrix multiOpHandling(string, vector<Matrix>&, vector<string>&, bool &);


	bool is_square();
	bool is_scalar();


private:
	void setElement(unsigned int, unsigned int, double);

};

#endif
