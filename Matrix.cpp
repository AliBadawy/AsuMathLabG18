#include<stdio.h>

#include "Matrix.h"
#include <iomanip> 

/************START CONSTRUCTORS*******************/
double** twoDArray = NULL;
#pragma warning(disable:4996)

//Default Constructor:
Matrix::Matrix()
	:twoDArray(NULL) {}

//Scalar Constructor:
Matrix::Matrix(double value)
	: twoDArray(NULL),
	notMatrix(true),
	rows(0),
	columns(0),
	num(value) {}

//Size Constructor:
Matrix::Matrix(unsigned int rows, unsigned int cols)
	:twoDArray(NULL),
	notMatrix(false),
	num(0)
{
	this->setSize(rows, cols);
}

//Copy Constructor:
Matrix::Matrix(const Matrix& A)
	:twoDArray(NULL),
	num(0)
{
	copyMatrix(&A);
}

//String Constructor
Matrix::Matrix(string name, string matrixString)
	:twoDArray(NULL),
	notMatrix(false),
	rows(0),
	columns(0),
	num(0)
{
	if (name.find_first_not_of(alphabets) == 0) throw("Variable name is not valid.");

	this->name = name;

	if (matrixString.find("[") != matrixString.npos)matrixString.erase(0, matrixString.find("[") + 1);

	while (matrixString.find("]") != matrixString.npos)
		matrixString.erase(matrixString.begin() + matrixString.find("]"), matrixString.end());

	vector<string> arrayOfRows(0);
	if (matrixString.find(";") == matrixString.npos) {
		arrayOfRows.push_back(matrixString);
		this->rows = 1;
	}
	else {
		while (1) {

			if (matrixString.find(";") != matrixString.npos && matrixString.find(";") != matrixString.length() - 1) {
				arrayOfRows.push_back(matrixString.substr(0, matrixString.find(";")));
				matrixString.erase(0, matrixString.find(";") + 1);
				this->rows++;
			}
			else {
				arrayOfRows.push_back(matrixString);
				this->rows++;
				break;
			}
		}
	}
	unsigned int colSafe = 0;
	vector<double> inputElements(0);
	for (size_t i = 0; i<this->rows; i++) {
		this->columns = 0;
		char* rowBuffer = new char[arrayOfRows[i].length() + 1];
		strcpy(rowBuffer, arrayOfRows[i].c_str());
		char* e;     //check for atof errors (two floating point, ..etc)
		char* token = strtok(rowBuffer, " ,");
		while (token) {
			double element = 0;
			this->columns++;
			element = strtod(token, &e);
			if (*e != '\0' && *e != ';') {   // if there's error it'll be passed to 'e' so it will not be null
				throw("Invalid numbers");
			}
			inputElements.push_back(element);
			token = strtok(NULL, " ,");
		}
		colSafe += this->columns;
		delete rowBuffer;
	}

	arrayOfRows.clear();

	if (this->columns != colSafe / (double)this->rows)throw("Size Error."); // handle this error (if no of columns is not constant)

	this->setSize(this->rows, this->columns);
	int k = 0;
	for (size_t i = 0; i<this->rows; i++)
		for (size_t j = 0; j<this->columns; j++) {
			this->twoDArray[i][j] = inputElements[k];
			k++;
		}
	inputElements.clear();
}
/*********************END CONSTRUCTORS*******************/

/*********************DESTRUCTOR***********************/
Matrix::~Matrix() {
	if (this->twoDArray) {
		for (size_t i = 0; i<this->rows; i++)
			delete[] this->twoDArray[i];
		this->twoDArray = NULL;
	}
	this->rows = 0, this->columns = 0, this->num = 0, this->name = "";
	this->notMatrix = false;
}
/*****************END DESTRUCTOR***********************/
void Matrix::copyMatrix(const Matrix* A) {
	if (this->name == "")
		this->setName(A->name);
	if (this->twoDArray) {
		for (size_t i = 0; i<this->rows; i++)
			delete[](this->twoDArray[i]);
		this->twoDArray = NULL;
	}
	if (A->notMatrix) {
		this->rows = 0;
		this->columns = 0;
		this->num = A->num;
		this->notMatrix = true;
	}
	else {
		this->notMatrix = false;
		this->setSize(A->rows, A->columns);
		for (size_t i = 0; i < this->rows; i++)
			for (size_t j = 0; j<this->columns; j++)
				this->twoDArray[i][j] = A->twoDArray[i][j];
	}
}

/***********OPERATORS*****************/
//Assignment Operators:
Matrix& Matrix::operator=(Matrix& A) {
	this->copyMatrix(&A);
	return *this;
}

Matrix& Matrix::operator=(double value) {
	this->setNum(value);
	return *this;
}

//Mathematical Operators:
Matrix& Matrix::operator+(Matrix& A) {
	Matrix* result = add(*this, A);
	return *result;
}
Matrix& Matrix::operator-(Matrix& A) {
	Matrix* result = subtract(*this, A);
	return *result;
}
Matrix& Matrix::operator+(double value) {
	Matrix *temp = new Matrix, *result;
	temp->setNum(value);
	result = add(*this, *temp);
	return *result;
}
Matrix& Matrix::operator-(double value) {
	Matrix *temp = new Matrix, *result;
	temp->setNum(value);
	result = subtract(*this, *temp);
	return *result;
}
Matrix& Matrix::operator*(Matrix& A) {
	Matrix* result = product(*this, A);
	return *result;
}

Matrix& Matrix::operator*(double value) {
	Matrix  *temp = new Matrix, *result;
	temp->setNum(value);
	result = product(*this, *temp);
	return *result;
}

Matrix& Matrix::operator/(Matrix& A) {
	Matrix* result = divide(*this, A);
	return *result;
}

Matrix& Matrix::operator/(double value) {
	Matrix *temp = new Matrix, *result;
	temp->setNum(value);
	result = divide(*this, *temp);
	return *result;
}

//negative unary operator:
Matrix& Matrix::operator-() {
	Matrix* result = this->negative();
	return *result;
}


/*******SETTERS AND GETTERS***********/
//Setters:
void Matrix::setName(string name) {
	this->name = name;
}

void Matrix::setNum(double num) {
	if (this->twoDArray) {
		for (size_t i = 0; i<this->rows; i++)
			delete[] this->twoDArray[i];
		this->twoDArray = NULL;
	}
	this->num = num;
	this->notMatrix = true;
}

void Matrix::setSize(unsigned int rows, unsigned int cols) {
	if (this->twoDArray)
		for (size_t i = 0; i<this->rows; i++)
			delete[] this->twoDArray[i];
	this->rows = rows;
	this->columns = cols;
	this->twoDArray = new double*[rows];
	for (size_t i = 0; i<rows; i++) {
		this->twoDArray[i] = new double[cols];
		for (size_t j = 0; j<cols; j++)
			twoDArray[i][j] = 0.0;
	}
	this->notMatrix = false;
}

void Matrix::setElement(unsigned int i, unsigned int j, double value) {
	this->twoDArray[i][j] = value;
}

//Getters:
string Matrix::getName() {
	return this->name;
}

int Matrix::getRows() {
	return this->rows;
}

int Matrix::getColumns() {
	return this->columns;
}

double Matrix::getNum() {
	return this->num;
}

double Matrix::getElement(unsigned int i, unsigned int j) {
	return this->twoDArray[i][j];
}

bool Matrix::is_square() {
	if (this->rows == this->columns) return true;
	else return false;
}

bool Matrix::is_scalar() {
	return this->notMatrix;
}

/********************OPERATIONS******************************/

Matrix* Matrix::add(Matrix& A, Matrix& B) {
	Matrix* temp = new Matrix;

	if (A.notMatrix&&B.notMatrix) {
		temp->setNum(A.num + B.num);
		return temp;
	}
	else if (!A.notMatrix && !B.notMatrix) {
		if (A.rows != B.rows || A.columns != B.columns)throw("Dimensions Errors (Addition).");
		temp->setSize(A.rows, A.columns);
		for (size_t i = 0; i< temp->rows; i++)
			for (size_t j = 0; j< temp->columns; j++)
				temp->twoDArray[i][j] = A.twoDArray[i][j] + B.twoDArray[i][j];
		return temp;
	}
	else throw("Operation Error: try '.+'");
}

Matrix* Matrix::pseudoAdd(Matrix& A, Matrix& B) {
	if (A.notMatrix && !B.notMatrix) {
		Matrix* temp = new Matrix;
		temp->setSize(B.rows, B.columns);
		for (size_t i = 0; i<temp->rows; i++)
			for (size_t j = 0; j<temp->columns; j++)
				temp->twoDArray[i][j] = A.num + B.twoDArray[i][j];
		return temp;
	}
	else if (!A.notMatrix && B.notMatrix) {
		Matrix* temp = new Matrix;
		temp->setSize(A.rows, A.columns);
		for (size_t i = 0; i<temp->rows; i++)
			for (size_t j = 0; j<temp->columns; j++)
				temp->twoDArray[i][j] = A.twoDArray[i][j] + B.num;
		return temp;
	}
	else throw ("Operator Error: try '+'");
}

Matrix* Matrix::subtract(Matrix& A, Matrix& B) {
	if (A.notMatrix&&B.notMatrix) {
		Matrix* temp = new Matrix;
		temp->setNum(A.num - B.num);
		return temp;
	}
	else if (!A.notMatrix && !B.notMatrix) {
		Matrix* temp = new Matrix;
		if (A.rows != B.rows || A.columns != B.columns)throw("Dimensions Errors (Subtraction).");
		temp->setSize(A.rows, A.columns);
		for (size_t i = 0; i< temp->rows; i++)
			for (size_t j = 0; j< temp->columns; j++)
				temp->twoDArray[i][j] = A.twoDArray[i][j] - B.twoDArray[i][j];
		return temp;
	}
	else throw ("Operator Error: try '.-'");
}

Matrix* Matrix::pseudoSubtract(Matrix& A, Matrix& B) {
	if (A.notMatrix && !B.notMatrix) {
		Matrix* temp = new Matrix;
		temp->setSize(B.rows, B.columns);
		for (size_t i = 0; i<temp->rows; i++)
			for (size_t j = 0; j<temp->columns; j++)
				temp->twoDArray[i][j] = A.num - B.twoDArray[i][j];
		return temp;
	}
	else if (!A.notMatrix && B.notMatrix) {
		Matrix* temp = new Matrix;
		temp->setSize(A.rows, A.columns);
		for (size_t i = 0; i<temp->rows; i++)
			for (size_t j = 0; j<temp->columns; j++)
				temp->twoDArray[i][j] = A.twoDArray[i][j] - B.num;
		return temp;
	}
	else throw ("Operator Error: try '-'");
}

Matrix* Matrix::product(Matrix& A, Matrix& B) {
	if (A.notMatrix && B.notMatrix) {     //scalar * scalar
		Matrix* temp = new Matrix;
		temp->setNum(A.num * B.num);
		return temp;
	}
	else if (!A.notMatrix && !B.notMatrix) {
		if (A.columns != B.rows)throw("Dimensions Error (Multiplication).");
		Matrix* temp = new Matrix;
		temp->setSize(A.rows, B.columns);
		for (size_t i = 0; i<A.rows; i++)
			for (size_t j = 0; j<B.rows; j++)
				for (size_t k = 0; k<A.columns; k++)  //or B.rows
					temp->twoDArray[i][j] += A.twoDArray[i][k] * B.twoDArray[k][j];
		return temp;
	}
	else throw ("Operator Error: try '.*'");
}

Matrix* Matrix::pseudoProduct(Matrix& A, Matrix& B) {
	if (A.notMatrix && !B.notMatrix) {  //scalar .* matrix
		Matrix* temp = new Matrix;
		temp->setSize(B.rows, B.columns);
		for (size_t i = 0; i<(temp->rows); i++)
			for (size_t j = 0; j<(temp->columns); j++)
				temp->twoDArray[i][j] = A.num * B.twoDArray[i][j];
		return temp;
	}
	else if (!A.notMatrix && B.notMatrix) {  //matrix .* scalar
		Matrix* temp = new Matrix;
		temp->setSize(A.rows, A.columns);
		for (size_t i = 0; i<(temp->rows); i++)
			for (size_t j = 0; j<(temp->columns); j++)
				temp->twoDArray[i][j] = A.twoDArray[i][j] * B.num;
		return temp;
	}
	else if (!A.notMatrix && !B.notMatrix) {				//matrix .* matrix
		if (A.rows != B.rows || A.columns != B.columns) throw("Dimensions Error.");
		Matrix* temp = new Matrix;
		temp->setSize(A.rows, A.columns);
		for (size_t i = 0; i<(temp->rows); i++)
			for (size_t j = 0; j<(temp->columns); j++)
				temp->twoDArray[i][j] = A.twoDArray[i][j] * B.twoDArray[i][j];

		return temp;
	}
	else throw("Operator Error: try '*'");
}

Matrix* Matrix::divide(Matrix& A, Matrix& B) {
	if (A.notMatrix && B.notMatrix) {
		Matrix* temp = new Matrix;
		if (B.num == 0.0) throw("nooo");
		temp->setNum(A.num / B.num);
		return temp;
	}
	else if (!A.notMatrix && !B.notMatrix) {
		if (A.columns != B.rows || !B.is_square())throw("Dimensions Error (Division).");
		Matrix* temp = new Matrix;
		temp->setSize(A.rows, B.columns);
		temp = product(A, *(B.inverse()));
		return temp;
	}
	else throw("Operator Error: try './'");
}

Matrix* Matrix::pseudoDiv(Matrix& A, Matrix& B) {
	if (A.notMatrix && !B.notMatrix) {  //scalar ./ matrix
		Matrix* temp = new Matrix;
		temp->setSize(B.rows, B.columns);
		for (size_t i = 0; i< temp->rows; i++)
			for (size_t j = 0; j< temp->columns; j++) {
				if (B.twoDArray[i][j] == 0.0) {
					delete temp;
					throw("nooo");
				}
				temp->twoDArray[i][j] = A.num / B.twoDArray[i][j];
			}
		return temp;
	}
	else if (!A.notMatrix && B.notMatrix) {  //matrix ./ scalar
		if (B.num == 0.0) throw("nooo");
		Matrix* temp = new Matrix;
		temp->setSize(A.rows, A.columns);
		for (size_t i = 0; i<temp->rows; i++)
			for (size_t j = 0; j<(temp->columns); j++)
				temp->twoDArray[i][j] = A.twoDArray[i][j] / B.num;
		return temp;
	}
	else if (!A.notMatrix && !B.notMatrix) {				//matrix ./ matrix
		if (A.rows != B.rows || A.columns != B.columns) throw("Dimensions Error.");
		Matrix* temp = new Matrix;
		temp->setSize(A.rows, A.columns);
		for (size_t i = 0; i<(temp->rows); i++)
			for (size_t j = 0; j<(temp->columns); j++) {
				if (B.twoDArray[i][j] == 0.0) {
					delete temp;
					throw("nooo");
				}
				temp->twoDArray[i][j] = A.twoDArray[i][j] / B.twoDArray[i][j];
			}
		return temp;
	}
	else throw ("Operator Error: try '/'");
}


Matrix* Matrix::negative() {
	Matrix* temp = new Matrix;
	if (this->notMatrix) {
		temp->setNum(0.0 - this->num);
		return temp;
	}
	temp->setSize(this->rows, this->columns);
	for (size_t i = 0; i<temp->rows; i++)
		for (size_t j = 0; j<temp->columns; j++)
			temp->twoDArray[i][j] = (0.0 - this->twoDArray[i][j]);
	return temp;
}

Matrix* Matrix::inverse() {
	if (!this->is_square())throw("Dimensions Error (Inverse).");

	double det = this->determinant();
	if (det == 0)throw("Matrix has no inverse.");
	det = 1.0 / det;
	Matrix* temp = new Matrix;
	temp->setSize(this->rows, this->columns);
	Matrix* temp2 = new Matrix;
	temp2->notMatrix = true;
	temp2->num = det;
	if (temp->rows == 1) {
		delete temp2;      //no need in this case
		temp->twoDArray[0][0] = det;
		return temp;
	}
	else if (temp->rows == 2) {
		for (size_t i = 0; i<temp->rows; i++)
			for (size_t j = 0; j<temp->columns; j++) {
				if (i == j) temp->twoDArray[i][j] = this->twoDArray[!i][!j];
				else temp->twoDArray[i][j] = (0 - this->twoDArray[i][j]);
			}
		temp = pseudoProduct(*temp, *temp2);
		delete temp2;
		return temp;
	}
	else {
		temp = this->adjoint();
		temp = pseudoProduct(*temp, *temp2);
		delete temp2;
		return temp;
	}
}

Matrix* Matrix::transpose() {
	Matrix* temp = new Matrix;
	temp->setSize((this->columns), (this->rows));
	for (size_t i = 0; i<this->columns; i++)
		for (size_t j = 0; j<this->rows; j++)
			temp->twoDArray[i][j] = this->twoDArray[j][i];
	return temp;
}

Matrix* Matrix::adjoint() {
	Matrix* temp = new Matrix;
	temp->setSize(this->rows, this->columns);
	temp = this->cofactor();
	temp = temp->transpose();
	return temp;
}

Matrix* Matrix::cofactor() {     //el plus wel minus hattwaza3 3ala el elements
	Matrix* temp = new Matrix;
	temp->setSize(this->rows, this->columns);
	for (size_t i = 0; i<this->rows; i++)
		for (size_t j = 0; j<this->columns; j++)
			temp->twoDArray[i][j] = pow(-1, i + j) * (this->determinant(true, i, j));
	return temp;
}

double Matrix::determinant(bool minor, unsigned posRow, unsigned posCol) {
	//di halet el determinant el tabee3y elly ana bas 3ayz keyamo
	if (!this->is_square())throw("Dimensions Error (Determinant).");
	if (!minor) {
		if (this->columns == 1)return this->twoDArray[0][0];
		else if (this->columns == 2)
			return ((this->twoDArray[0][0])*(this->twoDArray[1][1]) - (this->twoDArray[1][0])*(this->twoDArray[0][1]));
		else {   //Determinant

			unsigned int n, i, j, k;

			n = this->rows;                //the order of the matrix
			float** a = new float *[n];
			for (size_t i = 0; i<n; i++)
				a[i] = new float[n];

			double det = 1;
			int flag = 0;

			for (i = 0; i<n; i++)
				for (j = 0; j<n; j++)
					a[i][j] = this->twoDArray[i][j];    //input the elements of array
			for (i = 0; i<n; i++)                    //Pivotisation
				for (k = i + 1; k<n; k++)
					if (fabs(a[i][i])<fabs(a[k][i])) {
						flag++;
						for (j = 0; j<n; j++) {
							double temp = a[i][j];
							a[i][j] = a[k][j];
							a[k][j] = temp;
						}
					}


			for (i = 0; i<n - 1; i++)            //loop to perform the gauss elimination
				for (k = i + 1; k<n; k++)
				{
					double t = a[k][i] / a[i][i];
					for (j = 0; j<n; j++)
						a[k][j] = a[k][j] - t*a[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
				}



			for (i = 0; i<n; i++) {
				det = det*a[i][i];
			}
			if (flag % 2 == 0) {
				det = det;
			}
			else {
				det = -det;
			}

			return det;
		}
	}
	//di halet el determinant bta3 el submatrix 3ashan akamel beh el inverse
	else {
		Matrix* temp = new Matrix;
		temp->setSize((this->rows) - 1, (this->columns) - 1);
		for (size_t i = 0; i<temp->rows; i++)
			for (size_t j = 0; j<temp->columns; j++)
			{

				unsigned int x = i<posRow ? i : i + 1;
				unsigned int y = j<posCol ? j : i + 1;
				//cout<<posRow<<"  "<<posRow<<"  "<<x<<"  "<<y<<endl;
				temp->twoDArray[i][j] = this->twoDArray[x][y];
			}

		/* size_t subRow=0;
		for(size_t i=0;i<this->rows;i++){
		size_t subCol=0;
		if(i==posRow)continue;
		for(size_t j=0;j<this->columns;j++){
		if(j==posCol)continue;
		temp->twoDArray[subRow][subCol++]=this->twoDArray[i][j];
		}
		subRow++;
		}*/
		return temp->determinant();
	}
}

Matrix* Matrix::sinMatrix(){
	Matrix *temp = new Matrix;
	temp -> setSize(this->rows,this->columns);
	for(unsigned int i = 0;i<rows;i++)
		for(unsigned int j = 0; j<columns;j++)
			temp->twoDArray[i][j] = sin(this->twoDArray[i][j]);
	return temp;
}

Matrix* Matrix::cosMatrix(){
	Matrix *temp = new Matrix;
	temp -> setSize(this->rows,this->columns);
	for(unsigned int i = 0;i<rows;i++)
		for(unsigned int j = 0; j<columns;j++)
			temp->twoDArray[i][j] = cos(this->twoDArray[i][j]);
	return temp;
}

Matrix* Matrix::tanMatrix(){
	Matrix *temp = new Matrix;
	temp -> setSize(this->rows,this->columns);
	for(unsigned int i = 0;i<rows;i++)
		for(unsigned int j = 0; j<columns;j++){
			if(this->twoDArray[i][j] == PI) throw(/* Handle Here */)			
			temp->twoDArray[i][j] = tan(this->twoDArray[i][j]);
		}	
	return temp;
}

Matrix* Matrix::asinMatrix(){
	Matrix *temp = new Matrix;
	temp -> setSize(this->rows,this->columns);
	for(unsigned int i = 0;i<rows;i++)
		for(unsigned int j = 0; j<columns;j++)
			temp->twoDArray[i][j] = asin(this->twoDArray[i][j]);
	return temp;
}

Matrix* Matrix::acosMatrix(){
	Matrix *temp = new Matrix;
	temp -> setSize(this->rows,this->columns);
	for(unsigned int i = 0;i<rows;i++)
		for(unsigned int j = 0; j<columns;j++)
			temp->twoDArray[i][j] = acos(this->twoDArray[i][j]);
	return temp;
}

Matrix* Matrix::atanMatrix(){
	Matrix *temp = new Matrix;
	temp -> setSize(this->rows,this->columns);
	for(unsigned int i = 0;i<rows;i++)
		for(unsigned int j = 0; j<columns;j++)
			temp->twoDArray[i][j] = atan(this->twoDArray[i][j]);
	return temp;
}

void Matrix::eye(unsigned int rows/*=0*/, unsigned int columns/*=0*/) {
	if (rows == 0 && columns == 0) {
		this->num = 1.0;
		return;
	}
	else if (rows == 0 || columns == 0) throw("Dimension error (eye).");
	this->setSize(rows, columns);
	for (size_t i = 0; i<this->rows; i++) {
		for (size_t j = 0; j<this->columns; j++) {
			this->twoDArray[i][j] = (i == j) ? 1 : 0;
		}
	}
}

/*********************START SUBMATRIX************************/
Matrix* Matrix::subMatrix(string boundaries) {
	if (this->notMatrix)throw("Cannot use this function on a scalar value.");
	if (boundaries.find(",") == boundaries.npos)throw("Invalid format for submatrix");

	Matrix* temp = new Matrix;

	bool rowisrange = false;
	bool colisrange = false;

	string rowPart, colPart;
	unsigned int rows[2], cols[2];  //if range
	unsigned int row, col;          //if one element
	rowPart = boundaries.substr(0, boundaries.find(","));
	colPart = boundaries.substr(boundaries.find(",") + 1);

	if (rowPart.find(":") != rowPart.npos) {
		rowisrange = true;
		if (rowPart == ":") {  // ':' fadya (all rows)
			rows[0] = 1;
			rows[1] = this->rows;
		}
		else if (rowPart.find(":") == 0 && rowPart.find(":") != rowPart.length() - 1) {  // ':N' men el bdaya le N
			string endRange = rowPart.substr(rowPart.find(":") + 1);
			rows[0] = 1;
			char* e;
			rows[1] = strtod(endRange.c_str(), &e);
			if (*e != '\0' || rows[1]>this->rows || rows[1] <= rows[0]) throw("Invalid dimensions.");
		}
		else if (rowPart.find(":") != 0 && rowPart.find(":") == rowPart.length() - 1) {  // 'M:' men M bdaya lel akher
			string startRange = rowPart.substr(0, rowPart.find(":"));
			rows[1] = this->rows;
			char* e;
			rows[0] = strtod(startRange.c_str(), &e);
			if (*e != '\0' || rows[0]<1 || rows[0] >= rows[1]) throw("Invalid dimensions.");
		}
		else if (rowPart.find(":") != 0 && rowPart.find(":") != rowPart.length() - 1) {  //'M:N' men M le N
			string startRange = rowPart.substr(0, rowPart.find(":"));
			string endRange = rowPart.substr(rowPart.find(":") + 1);
			char *e1, *e2;
			rows[0] = strtod(startRange.c_str(), &e1);
			rows[1] = strtod(endRange.c_str(), &e2);
			if (*e1 != '\0' || *e2 != '\0' || rows[0] < 1 || rows[1]>this->rows || rows[0] >= rows[1]) throw("Invalid dimensions.");
		}
	}
	else {
		rowisrange = false;
		char* e;
		row = strtod(rowPart.c_str(), &e);
		if (*e != '\0' || row<1 || row>this->rows) throw("Invalid dimensions.");
	}

	if (colPart.find(":") != colPart.npos) {
		colisrange = true;
		if (colPart == ":") {          // ':' fadya (all columns)
			cols[0] = 1;
			cols[1] = this->columns;
		}
		else if (colPart.find(":") == 0 && colPart.find(":") != colPart.length() - 1) {  // ':N' men el bdaya le N
			string endRange = colPart.substr(colPart.find(":") + 1);
			cols[0] = 1;
			char* e;
			cols[1] = strtod(endRange.c_str(), &e);
			if (*e != '\0' || cols[1]>this->columns || cols[1] <= cols[0]) throw("Invalid dimensions.");
		}
		else if (colPart.find(":") != 0 && colPart.find(":") == colPart.length() - 1) {  // 'M:' men M bdaya lel akher
			string startRange = colPart.substr(0, colPart.find(":"));
			cols[1] = this->columns;
			char* e;
			cols[0] = strtod(startRange.c_str(), &e);
			if (*e != '\0' || cols[0]<1 || cols[0] >= cols[1]) throw("Invalid dimensions.");
		}
		else if (colPart.find(":") != 0 && colPart.find(":") != colPart.length() - 1) {  //'M:N' men M le N
			string startRange = colPart.substr(0, colPart.find(":"));
			string endRange = colPart.substr(colPart.find(":") + 1);
			char *e1, *e2;
			cols[0] = strtod(startRange.c_str(), &e1);
			cols[1] = strtod(endRange.c_str(), &e2);
			if (*e1 != '\0' || *e2 != '\0' || cols[0] < 1 || cols[1]>this->columns || cols[0] >= cols[1]) throw("Invalid dimensions.");
		}
	}
	else {
		colisrange = false;
		char* e;
		col = strtod(colPart.c_str(), &e);
		if (*e != '\0' || col<1 || col>this->columns) throw("Invalid dimensions.");
	}
	if (rowisrange && colisrange) {
		temp->setSize(rows[1] - rows[0] + 1, cols[1] - cols[0] + 1);
		int tempCol = cols[0];
		for (size_t i = 0; i<temp->rows; i++) {
			for (size_t j = 0; j<temp->columns; j++) {
				temp->twoDArray[i][j] = this->twoDArray[rows[0] - 1][tempCol - 1];
				tempCol++;
			}
			rows[0]++;
			tempCol = cols[0];
		}
	}
	else if (rowisrange && !colisrange) {
		temp->setSize(rows[1] - rows[0] + 1, 1);
		for (size_t i = 0; i<temp->rows; i++) {
			for (size_t j = 0; j<temp->columns; j++) {
				temp->twoDArray[i][j] = this->twoDArray[rows[0] - 1][col - 1];
			}
			rows[0]++;
		}
	}
	else if (!rowisrange && colisrange) {
		temp->setSize(1, cols[1] - cols[0] + 1);
		unsigned int tempCol = cols[0];
		for (size_t i = 0; i<temp->rows; i++) {
			for (size_t j = 0; j<temp->columns; j++) {
				temp->twoDArray[i][j] = this->twoDArray[row - 1][tempCol - 1];
				tempCol++;
			}
			tempCol = cols[0];
		}
	}
	else {
		temp->notMatrix = true;
		temp->rows = 0, temp->columns = 0;   //egra2 rotiny 3ashan kol el scalar values yb2o shabah ba3d :D
		double value = this->twoDArray[row - 1][col - 1];
		temp->num = value;
	}
	return temp;
}
/***********************END SUBMATRIX************************/

Matrix* Matrix::concatenateHor(Matrix& A,Matrix& B)
{
    if(A.rows != B.rows)throw("Cannot horizontally concatenate matrices with different heights.");

	int newColumns = A.columns + B.columns;
	int newRows = A.rows;
	Matrix * concMatrix = new Matrix;
	concMatrix->setSize(newRows, newColumns);
	for (size_t i = 0; i < newRows; i++)
	{
		for (size_t j = 0; j < newColumns; j++)
		{
			if (j < A.columns)concMatrix->setElement(i, j, A.twoDArray[i][j]);
			else concMatrix->setElement(i, j, B.twoDArray[i][j-A.columns]);
		}
	}
	return concMatrix;
}

Matrix* Matrix::concatenateVer(Matrix& A, Matrix& B)
{
	if (A.columns != B.columns)throw("Cannot vertically concatenate matrices with different widths.");

	int newColumns = A.columns;
	int newRows = A.rows+B.rows;
	Matrix * concMatrix = new Matrix;
	concMatrix->setSize(newRows, newColumns);
	for (size_t i = 0; i < newRows; i++)
	{
		for (size_t j = 0; j < newColumns; j++)
		{
			if (i < A.rows)concMatrix->setElement(i, j, A.twoDArray[i][j]);
			else concMatrix->setElement(i, j, B.twoDArray[i-A.rows][j]);
		}
	}
	return concMatrix;
}

void Matrix::printMatrix(bool name/*=true*/, unsigned int num_equal/*=0*/, const vector<string>&arrayOfNames) {

	if (name == false) {
		if (this->notMatrix) {
			cout << "     " << this->num << endl;
		}
		else {
			cout << endl;
			unsigned int longestsize = 0;
			//********calculating length of longest element in the displayed matrix*********
			for (size_t i = 0; i<this->rows; i++)
				for (size_t j = 0; j<this->columns; j++) {
					char temp[20];
					sprintf(temp, "%g", this->twoDArray[i][j]);
					if (strlen(temp)>longestsize)longestsize = strlen(temp);
				}
			//***********displaying**************
			for (size_t i = 0; i<this->rows; i++) {
				cout << "     ";
				for (size_t j = 0; j<this->columns; j++) {

					//calculating length of the displayed element
					char temp[20];
					sprintf(temp, "%g", this->twoDArray[i][j]);
					unsigned int outputLen = strlen(temp);
					printf("%g", this->twoDArray[i][j]);
					//cout<<fixed<<std::setprecision(2)<<this->twoDArray[i][j];

					// printing no. of spaces equal to the size of the longest matrix - size of displayed matrix + 3 as a tolerance
					for (size_t k = 0; k<longestsize - outputLen + 3; k++) {
						cout << " ";
					}
				}
				cout << endl;
			}
		}
	}
	else {  //name = true
		cout << "     ";
		for (size_t i = 0; i<num_equal; i++)
			cout << arrayOfNames[i] << " = ";
		if (this->notMatrix) {
			cout << this->num << endl;
		}
		else {
			cout << endl;
			unsigned int longestsize = 0;
			//********calculating length of longest element in the displayed matrix*********
			for (size_t i = 0; i<this->rows; i++)
				for (size_t j = 0; j<this->columns; j++) {
					char temp[20];
					sprintf(temp, "%g", this->twoDArray[i][j]);
					if (strlen(temp)>longestsize)longestsize = strlen(temp);
				}
			//***********displaying**************
			for (size_t i = 0; i<this->rows; i++) {
				cout << "     ";
				for (size_t j = 0; j<this->columns; j++) {

					//calculating length of the displayed element
					char temp[20];
					sprintf(temp, "%g", this->twoDArray[i][j]);
					unsigned int outputLen = strlen(temp);

					printf("%g", this->twoDArray[i][j]);

					// printing no. of spaces equal to the size of the longest matrix - size of displayed matrix + 3 as a tolerance
					for (size_t k = 0; k<longestsize - outputLen + 3; k++) {
						cout << " ";
					}
				}
				cout << endl;
			}
		}
	}
}
