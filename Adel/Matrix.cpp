#include "Matrix.h"

/************START CONSTRUCTORS*******************/

//Default Constructor:
Matrix::Matrix()
	:notMatrix(false), rows(0), columns(0), num(0), name(""), twoDArray(NULL) {}

//Scalar Constructor:
Matrix::Matrix(double value)
	: notMatrix(true), rows(0), columns(0), num(value), name(""), twoDArray(NULL) {}

//Size Constructor:
Matrix::Matrix(unsigned int rows, unsigned int cols)
	: notMatrix(false), rows(rows), columns(cols), num(0), name(""), twoDArray(NULL)
{
	this->setSize(rows, cols);
}

//Copy Constructor:
Matrix::Matrix(const Matrix& A)
	:notMatrix(false), rows(0), columns(0), num(0), name(""), twoDArray(NULL)
{
	copyMatrix(&A);
}

//String Constructor
Matrix::Matrix(string name, string matrixString)
	:notMatrix(false), rows(0), columns(0), num(0), name(name), twoDArray(NULL)
{
	if (name.find_first_not_of(alphabets) == 0) throw("Variable name is not valid.");

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

void Matrix::addMatrixToMatrix(Matrix &x, int r, int c)
{

	if (x.notMatrix)
	{
		x.notMatrix = false;
		x.columns++;
		x.rows++;
		x.twoDArray = new double*[1];
		x.twoDArray[0] = new double[1];
		x.twoDArray[0][0] = x.num;
	}
	//cout<<"r = "<<r<<" c = "<<c<<endl;
	//x.printMatrix();
	if (this->twoDArray == NULL && r == 0 && c == 0)
	{
		this->rows = x.rows;
		this->columns = x.columns;
		this->notMatrix = false;
		this->twoDArray = new double*[this->rows];
		for (int i = 0; i<this->rows; i++)
		{
			this->twoDArray[i] = new double[this->columns];
		}

		for (int i = 0; i<this->rows; i++)
			for (int j = 0; j<this->columns; j++)
			{
				this->twoDArray[i][j] = x.twoDArray[i][j];
			}
	}


	else if (this->rows >= r + x.rows && this->columns >= c + x.columns)
	{
		int a = r, b = c;
		for (int i = 0; i<x.rows; i++)
		{
			b = c;
			for (int j = 0; j<x.columns; j++)
			{
				this->twoDArray[a][b] = x.twoDArray[i][j];
				b++;
			}
			a++;
		}
	}

	else if (this->rows<r + x.rows && this->columns >= c + x.columns)
	{
		Matrix temp(*this);

		for (int i = 0; i<this->rows; i++)
			delete[] this->twoDArray[i];
		delete[]this->twoDArray;

		this->rows = r + x.rows;

		this->twoDArray = new double*[this->rows];
		for (int i = 0; i<this->rows; i++)
			this->twoDArray[i] = new double[this->columns];

		for (int i = 0; i<temp.rows; i++)
			for (int j = 0; j<temp.columns; j++)
			{
				this->twoDArray[i][j] = temp.twoDArray[i][j];
			}
		int a = r, b = c;
		for (int i = 0; i<x.rows; i++)
		{
			b = c;
			for (int j = 0; j<x.columns; j++)
			{
				this->twoDArray[a][b] = x.twoDArray[i][j];
				b++;
			}
			a++;
		}
	}
	else if (this->rows >= r + x.rows && this->columns< c + x.columns)
	{
		Matrix temp(*this);

		for (int i = 0; i<this->rows; i++)
			delete[] this->twoDArray[i];
		delete[]this->twoDArray;

		this->columns = c + x.columns;

		this->twoDArray = new double*[this->rows];
		for (int i = 0; i<this->rows; i++)
			this->twoDArray[i] = new double[this->columns];

		for (int i = 0; i<temp.rows; i++)
			for (int j = 0; j<temp.columns; j++)
			{
				this->twoDArray[i][j] = temp.twoDArray[i][j];
			}
		int a = r, b = c;
		for (int i = 0; i<x.rows; i++)
		{
			b = c;
			for (int j = 0; j<x.columns; j++)
			{
				this->twoDArray[a][b] = x.twoDArray[i][j];
				b++;
			}
			a++;
		}

	}
	else if (this->rows< r + x.rows && this->columns< c + x.columns) throw("  invalid exepression.");
	//this->printMatrix();
}




Matrix::Matrix(string name, string matrixString, vector<Matrix> & storedMatrices, vector<string>& systemCommands)
{
	if (name.find_first_not_of(alphabets) == 0) throw("Variable name is not valid.");
	this->name = name;
	this->columns = 0;
	this->rows = 0;
	this->num = 0;
	this->notMatrix = false;
	this->twoDArray = NULL;

	int numbOfColsinFirstRow = 0;
	int tempNumCols = 0;
	int numb_elements = 0;

	while(matrixString[matrixString.length()-1]==' ')matrixString.erase(matrixString.length()-1,1); //remove spaces from the end of input

	if(matrixString[matrixString.length()-1]==';')matrixString.erase(matrixString.length()-1,1); //removing semicolon and spaces before it
	else
	{
		while(matrixString[matrixString.length()-1]==' ')
		{
			matrixString.erase(matrixString.length()-1,1);
			if(matrixString[matrixString.length()-1]==';')matrixString.erase(matrixString.length()-1,1);
		}

	}


	//handling brackets errors
	int counter1 = 0, counter2 = 0;

	for (int i = 0; i<matrixString.length(); i++)
	{
		if (matrixString[i] == '(')
			counter1++;
		else if (matrixString[i] == ')')
			counter2++;
	}
	if (counter1 != counter2)throw("invalid exepression.(close Brackets)");


	counter1 = 0; counter2 = 0;

	for (int i = 0; i<matrixString.length(); i++)
	{
		if (matrixString[i] == '[')
			counter1++;
		else if (matrixString[i] == ']')
			counter2++;
	}
	if (counter1 != counter2)throw("invalid exepression.(close Brackets)");




	matrixString.erase(0, matrixString.find("[") + 1);
	while (matrixString[0] == ' ')matrixString.erase(0, 1);
	matrixString.erase(matrixString.rfind("]"), 1);
	while (matrixString[matrixString.length() - 1] == ' ')matrixString.erase(matrixString.length() - 1, 1);

	//string operationsString = "+*/^'();";

	string operationsString = "+*/^;";
	int currentRow = 0, currentCol = 0;
	

	for(int i=0;i<matrixString.length();i++)
	{
		if(matrixString[i]=='.')
		{
			while(matrixString[i+1]==' ')matrixString.erase(i+1,1);
			while(matrixString[i-1]==' '){matrixString.erase(i-1,1);i--;}
		}
		else if(operationsString.find(matrixString[i])!=std::string::npos)
		{
			while(matrixString[i+1]==' ')matrixString.erase(i+1,1);
			while(matrixString[i-1]==' '){matrixString.erase(i-1,1);i--;}
			if(matrixString[i] == ';')if(matrixString[i+1] == ';' )throw("invalid exepression(;;)");
		}
		else if(matrixString[i]=='\'')
		{
			while(matrixString[i-1]==' '){matrixString.erase(i-1,1);i--;}
		}
		else if(matrixString[i]==',')
		{
			while(matrixString[i+1]==' ')matrixString.erase(i+1,1);
			while(matrixString[i-1]==' '){matrixString.erase(i-1,1);i--;}
			if(matrixString[i+1]==',') throw("invalid exepression (,,)");
		}
		else if(matrixString[i]=='-')
		{
			if(matrixString[i+1]==' ')
			{
				if(i==0){while(matrixString[i+1]==' ')matrixString.erase(i+1,1);}
				else
				{
					while(matrixString[i+1]==' ')matrixString.erase(i+1,1);
					while(matrixString[i-1]==' '){matrixString.erase(i-1,1);i--;}
				}
			}
		}
		else if(matrixString[i]=='(')
		{
			while(matrixString[i+1]==' ')matrixString.erase(i+1,1);
		}
		else if(matrixString[i]==')')
		{
			while(matrixString[i-1]==' '){matrixString.erase(i-1,1);i--;}
		}
		else if(matrixString[i]==' ')
		{
			while(matrixString[i+1]==' ')matrixString.erase(i+1,1);
		}
	}






	string alphabetsString = "abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (int i = 0; i<matrixString.length(); i++)
	{
		string temp = "";
		if (matrixString[i] == ',' || matrixString[i] == ' ')
		{

			while (matrixString[i] == ',' || matrixString[i] == ' ')
				i++;
			i--;
			currentCol++;
		}
		else if (matrixString[i] == ';' || matrixString[i] == '\n')
		{
			currentRow++;
			currentCol = 0;
		}
		else if (matrixString[i] == '[')
		{

			temp += matrixString[i];
			int counter3 = 1, k = i + 1;
			while (counter3 != 0)
			{
				temp += matrixString[k];
				if (matrixString[k] == '[')counter3++;
				if (matrixString[k] == ']')counter3--;
				k++;
			}
			
			Matrix x("noname", temp, storedMatrices, systemCommands);
			numb_elements += x.rows * x.columns;


			this->addMatrixToMatrix(x, currentRow, currentCol);


			if (matrixString[i + temp.length()] == ',' || matrixString[i + temp.length()] == ' ')
			{
				currentCol += x.columns - 1;
			}
			else if (matrixString[i + temp.length()] == ';' || matrixString[i + temp.length()] == '\n')
			{
				currentRow += x.rows - 1;
			}


			i += temp.length() - 1;
		}
		else
		{
			int k = i;
			while ((matrixString[k] != ' ' && matrixString[k] != ',' && matrixString[k] != ';' && matrixString[k] != '\n' )  )
			{
				temp += matrixString[k];
				k++;
				if(matrixString[k-1]=='(')
				{
					
					while(matrixString[k-1]!=')')
					{
						temp+=matrixString[k];
						k++;
					}
				}

				if (k == matrixString.length())break;
			}



			int multi_op_flag = 0;
			string multioperationsString = "+-/*^'";
			for (int j = 0; j<temp.length(); j++)
			{
				if (multioperationsString.find(temp[j]) != std::string::npos)
				{
					multi_op_flag = 1;
					break;
				}
				else if(temp.find("inv(")!=std::string::npos)
				{
					multi_op_flag = 1;
					break;
				}
				else if(temp.find("sin(")!=std::string::npos)
				{
					multi_op_flag = 1;
					break;
				}
				else if(temp.find("cos(")!=std::string::npos)
				{
					multi_op_flag = 1;
					break;
				}
				else if(temp.find("tan(")!=std::string::npos)
				{
					multi_op_flag = 1;
					break;
				}
				else if(temp.find("asin(")!=std::string::npos)
				{
					multi_op_flag = 1;
					break;
				}
				else if(temp.find("acos(")!=std::string::npos)
				{
					multi_op_flag = 1;
					break;
				}
				else if(temp.find("atan(")!=std::string::npos)
				{
					multi_op_flag = 1;
					break;
				}
				else if(temp.find("sqrt(")!=std::string::npos)
				{
					multi_op_flag = 1;
					break;
				}
				else if(temp.find("rand(")!=std::string::npos)
				{
					multi_op_flag = 1;
					break;
				}
				else if(temp.find("eye(")!=std::string::npos)
				{
					multi_op_flag = 1;
					break;
				}
				else if(temp.find("zeros(")!=std::string::npos)
				{
					multi_op_flag = 1;
					break;
				}
				else if(temp.find("ones(")!=std::string::npos)
				{
					multi_op_flag = 1;
					break;
				}
			
			}

			if (multi_op_flag)
			{

				//zawdt el print de, lw hya b true bytb3,  false msh bytb3
				bool print = false;
				//cout<<"temp = "<<temp<<endl;
				Matrix x = multiOpHandling(temp, storedMatrices, systemCommands, print);
				if (x.notMatrix == true)
				{
					numb_elements += 1;
				}
				else
				{
					numb_elements += x.rows * x.columns;
				}
				this->addMatrixToMatrix(x, currentRow, currentCol);
				if (matrixString[i + temp.length()] == ',' || matrixString[i + temp.length()] == ' ')
				{
					currentCol += x.columns - 1;
				}
				else if (matrixString[i + temp.length()] == ';' || matrixString[i + temp.length()] == '\n')
				{
					currentRow += x.rows - 1;
				}
			}
			else
			{
				if (alphabetsString.find(temp[0]) != std::string::npos)
				{
					for (int j = 0; j<storedMatrices.size(); j++)
					{
						if (storedMatrices[j].name == temp)
						{
							if (storedMatrices[j].notMatrix == true)
							{
								numb_elements += 1;
							}
							else
							{
								numb_elements += storedMatrices[j].rows * storedMatrices[j].columns;
							}

							this->addMatrixToMatrix(storedMatrices[j], currentRow, currentCol);
							if (matrixString[i + temp.length()] == ',' || matrixString[i + temp.length()] == ' ')
							{
								currentCol += storedMatrices[j].columns - 1;
							}
							else if (matrixString[i + temp.length()] == ';' || matrixString[i + temp.length()] == '\n')
							{
								currentRow += storedMatrices[j].rows - 1;
							}
							break;
						}
					}
				}
				else
				{
					Matrix x(stod(temp));
					numb_elements += 1;

					this->addMatrixToMatrix(x, currentRow, currentCol);
				}
			}
			i += temp.length() - 1;
		}
		//cout<<"CR = "<<currentRow<<"	"<<"CL= "<<currentCol<<endl;
	}
	//this->printMatrix();

	if ((this->rows * this->columns) != numb_elements)throw("  invalid exepression(dimensions error)");

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
Matrix& Matrix::operator=(Matrix A) {
	this->copyMatrix(&A);
	return *this;
}

Matrix& Matrix::operator=(double value) {
	this->setNum(value);
	return *this;
}

//Mathematical Operators:
Matrix Matrix::operator+(Matrix& A) {
	Matrix* result = add(*this, A);
	return *result;
}
Matrix Matrix::operator-(Matrix& A) {
	Matrix* result = subtract(*this, A);
	return *result;
}
Matrix Matrix::operator+(double value) {
	Matrix *temp = new Matrix, *result;
	temp->setNum(value);
	result = add(*this, *temp);
	return *result;
}
Matrix Matrix::operator-(double value) {
	Matrix *temp = new Matrix, *result;
	temp->setNum(value);
	result = subtract(*this, *temp);
	return *result;
}
Matrix Matrix::operator*(Matrix& A) {
	Matrix* result = product(*this, A);
	return *result;
}

Matrix Matrix::operator*(double value) {
	Matrix  *temp = new Matrix, *result;
	temp->setNum(value);
	result = product(*this, *temp);
	return *result;
}

Matrix Matrix::operator/(Matrix& A) {
	Matrix* result = divide(*this, A);
	return *result;
}

Matrix Matrix::operator/(double value) {
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
		if (B.num == 0.0) throw("undefined value");
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
					throw("undefined value");
				}
				temp->twoDArray[i][j] = A.num / B.twoDArray[i][j];
			}
		return temp;
	}
	else if (!A.notMatrix && B.notMatrix) {  //matrix ./ scalar
		if (B.num == 0.0) throw("undefined value");
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
					throw("undefined value");
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
	temp->setSize(this->columns, this->rows);
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

void print(double** a, int n)
{
	for (int i = 0; i<n; i++)
	{
		for (int j = 0; j<n; j++)
		{
			cout << a[i][j] << "	";
		}
		cout << endl;
	}
	cout << "================================" << endl;
}

double Matrix::determinant(bool minor, unsigned posRow, unsigned posCol) {
	//di halet el determinant el tabee3y elly ana bas 3ayz keyamo
	if (!this->is_square())throw("Dimensions Error (Determinant).");
	if (!minor) {
		if (this->columns == 1)return this->twoDArray[0][0];
		else if (this->columns == 2)
			return ((this->twoDArray[0][0])*(this->twoDArray[1][1]) - (this->twoDArray[1][0])*(this->twoDArray[0][1]));
		else {   //Determinant

			double det = 1;
			unsigned int n = this->rows;
			double**a = new double*[n];
			for (size_t i = 0; i<n; i++)
				a[i] = new double[n];

			//filling a[][]

			for (size_t i = 0; i<n; i++)
			{
				for (size_t j = 0; j<n; j++)
				{
					a[i][j] = this->twoDArray[i][j];
				}

			}


			//check if tri of zeros
			int zero_tri_flag = 0;

			for (size_t i = 0; i<n; i++)
			{
				for (size_t j = 0; j<i; j++)
				{
					if (a[i][j] != 0 && a[i][j] != -0)
					{
						zero_tri_flag = 1;
						break;

					}
				}
				if (zero_tri_flag == 1)break;
			}

			if (zero_tri_flag == 0)
			{
				for (size_t i = 0; i<n; i++)
				{
					det = det * a[i][i];
				}
				return det;
			}

			else
			{
				int col_piv_flag = 0;
				zero_tri_flag = 0;


				for (size_t i = 0; i<n - 1; i++)
				{
					//largest pivot
					double largest_pivot = a[i][i];
					unsigned int index_of_largest_pivot = i;
					for (unsigned int o = i + 1; o<n; o++)
					{
						if (fabs(a[o][i])>fabs(largest_pivot))
						{
							largest_pivot = a[o][i];
							index_of_largest_pivot = o;
						}
					}
					if (index_of_largest_pivot != i)
					{
						for (size_t j = 0; j<n; j++)
						{
							double temp;
							temp = a[index_of_largest_pivot][j];
							a[index_of_largest_pivot][j] = a[i][j];
							a[i][j] = temp;
						}
						det = det*-1;

					}

					int zero_tri_flag1 = 0;
					for (size_t x = 0; x<n; x++)
					{
						for (size_t y = 0; y<x; y++)
						{
							if (a[x][y] != 0 && a[x][y] != -0)
							{
								zero_tri_flag1 = 1;
								break;
							}
						}
						if (zero_tri_flag1 == 1)break;
					}
					if (zero_tri_flag1 == 0)
					{
						for (size_t j = 0; j<n; j++)
						{
							det = det * a[j][j];
						}
						return det;
					}
					zero_tri_flag1 = 0;


					int in_while_flag = 0;
					if (a[i][i] == 0 || a[i][i] == -0)
					{
						in_while_flag = 1;
						//swap with the lower row

						for (size_t f = i + 1; f<n; f++)
						{
							if (a[f][i] != 0 && a[f][i] != -0)
							{

								for (size_t j = 0; j<n; j++)
								{
									double temp;
									temp = a[f][j];
									a[f][j] = a[i][j];
									a[i][j] = temp;
								}

								col_piv_flag = 1;
								break;
							}

						}

						det = det *-1;

						int zero_tri_flag1 = 0;
						for (size_t x = 0; x<n; x++)
						{
							for (size_t y = 0; y<x; y++)
							{
								if (a[x][y] != 0 && a[x][y] != -0)
								{
									zero_tri_flag1 = 1;
									break;
								}
							}
							if (zero_tri_flag1 == 1)break;
						}
						if (zero_tri_flag1 == 0)
						{
							for (size_t j = 0; j<n; j++)
							{
								det = det * a[j][j];
							}
							return det;
						}
						zero_tri_flag1 = 0;
					}



					if (col_piv_flag == 0 && in_while_flag == 1)
					{
						in_while_flag = 0;
						col_piv_flag = 1;
						det = 0;
						return det;
					}
					col_piv_flag = 0;
					in_while_flag = 0;


					//making pivot =1
					/*det = det * a[i][i];
					double temp2=a[i][i];
					for(size_t j =i;j<n;j++)
					{
					a[i][j] = a[i][j]/temp2;
					}*/
					//amsek kol el rows ely t7t el i  w adrab el row bta3 el pivot fel elemnt ely ta7t el pivot a[i+1][i] w aminuso meno
					for (size_t k = i + 1; k<n; k++)
					{
						double temp = a[k][i] / a[i][i];

						for (size_t j = i; j<n; j++)
						{
							a[k][j] = a[k][j] - temp * a[i][j];
							//if(fabs(a[k][i])<(pow(10,-15)))a[k][i]=0;
							a[k][i] = 0;
						}
					}

					//since change in matrix therefore check for zero tri

					int zero_tri_flag2 = 0;

					for (size_t x = 0; x<n; x++)
					{
						for (size_t y = 0; y<x; y++)
						{
							if (a[x][y] != 0 && a[x][y] != -0)
							{
								zero_tri_flag2 = 1;
								break;
							}
						}
						if (zero_tri_flag2 == 1)break;
					}
					if (zero_tri_flag2 == 0)
					{
						for (size_t j = 0; j<n; j++)
						{
							det = det * a[j][j];
						}
						return det;
					}

					zero_tri_flag2 = 0;
				}
			}
		}
	}
	//di halet el determinant bta3 el submatrix 3ashan akamel beh el inverse
	else {
		Matrix* temp = new Matrix;
		temp->setSize(this->rows - 1, this->columns - 1);

		for (size_t i = 0; i<temp->rows; i++)
			for (size_t j = 0; j<temp->columns; j++)
			{

				size_t x = i<posRow ? i : i + 1;
				size_t y = j<posCol ? j : j + 1;
				//cout<<posRow<<"  "<<posRow<<"  "<<x<<"  "<<y<<endl;
				temp->twoDArray[i][j] = this->twoDArray[x][y];
			}
		return temp->determinant();
	}
	throw("Can't Solve Determinant.");
}

void Matrix::eye(unsigned int rows/*=0*/, unsigned int cols/*=0*/) {
	if (rows == 0 || cols == 0) throw("Dimension error (eye).");
	this->setSize(rows, cols);
	for (size_t i = 0; i<this->rows; i++)
		for (size_t j = 0; j<this->columns; j++)
			this->twoDArray[i][j] = (i == j) ? 1.0 : 0.0;
}

void Matrix::ones(unsigned int rows/*=0*/, unsigned int cols/*=0*/) {
	if (rows == 0 || cols == 0) throw("Dimension error (ones).");
	this->setSize(rows, cols);
	for (size_t i = 0; i<this->rows; i++)
		for (size_t j = 0; j<this->columns; j++)
			this->twoDArray[i][j] = 1.0;
}

void Matrix::zeros(unsigned int rows/*=0*/, unsigned int cols/*=0*/) {
	if (rows == 0 || cols == 0) throw("Dimension error (ones).");
	this->setSize(rows, cols);
	for (size_t i = 0; i<this->rows; i++)
		for (size_t j = 0; j<this->columns; j++)
			this->twoDArray[i][j] = 0.0;
}

void Matrix::random(unsigned int rows/*=0*/, unsigned int cols/*=0*/) {
	if (rows == 0 || cols == 0) throw("Dimension error (ones).");
	this->setSize(rows, cols);
	for (size_t i = 0; i<this->rows; i++)
		for (size_t j = 0; j<this->columns; j++)
			this->twoDArray[i][j] = (double)rand() / RAND_MAX;
}

Matrix* Matrix::sinMatrix() {
	Matrix *temp = new Matrix;
	if (this->notMatrix) {
		temp->setNum(sin(this->num));
	}
	else {
		temp->setSize(this->rows, this->columns);
		for (unsigned int i = 0; i<rows; i++)
			for (unsigned int j = 0; j<columns; j++)
				temp->twoDArray[i][j] = sin(this->twoDArray[i][j]);
	}
	return temp;
}


Matrix* Matrix::powerMatrix(Matrix &M, Matrix&P) {
	if (!M.notMatrix && P.notMatrix) {  //matrix .^ scalar
		Matrix* temp = new Matrix;
		temp->setSize(M.rows, M.columns);
		for (size_t i = 0; i<temp->rows; i++)
			for (size_t j = 0; j<(temp->columns); j++)
				temp->twoDArray[i][j] = pow(M.twoDArray[i][j], P.num);
		return temp;
	}
	else if (M.notMatrix && P.notMatrix) {				//matrix .^ matrix
		if (M.rows != P.rows || M.columns != P.columns) throw("Dimensions Error.");
		Matrix* temp = new Matrix;
		temp->setNum(pow(M.num, P.num));
		return temp;
	}
}


Matrix* Matrix::sqrtMatrix() {
	Matrix *temp = new Matrix;
	if (this->notMatrix) {
		temp->setNum(sqrt(this->num));
	}
	else
	{
		temp->setSize(this->rows, this->columns);
		for (unsigned int i = 0; i < rows; i++)
		{
			for (unsigned int j = 0; j < columns; j++)
			{
				if (this->twoDArray[i][j] < 0) { throw("complex results aren't supported "); }
				else temp->twoDArray[i][j] = sqrt(this->twoDArray[i][j]);
			}
		}
	}
	return temp;
}


Matrix* Matrix::cosMatrix() {
	Matrix *temp = new Matrix;
	if (this->notMatrix) {
		temp->setNum(cos(this->num));
	}
	else {
		temp->setSize(this->rows, this->columns);
		for (unsigned int i = 0; i<rows; i++)
			for (unsigned int j = 0; j<columns; j++)
				temp->twoDArray[i][j] = cos(this->twoDArray[i][j]);
	}
	return temp;
}

Matrix* Matrix::tanMatrix() {
	Matrix *temp = new Matrix;
	if (this->notMatrix) {
		temp->setNum(tan(this->num));
	}
	else {
		temp->setSize(this->rows, this->columns);
		for (unsigned int i = 0; i<rows; i++)
			for (unsigned int j = 0; j<columns; j++) {
				if (this->twoDArray[i][j] == PI / 2.0) throw/* Handle Here */;
				temp->twoDArray[i][j] = tan(this->twoDArray[i][j]);
			}
	}
	return temp;
}

Matrix* Matrix::asinMatrix() {
	Matrix *temp = new Matrix;
	if (this->notMatrix) {
		temp->setNum(asin(this->num));
	}
	else {
		temp->setSize(this->rows, this->columns);
		for (unsigned int i = 0; i<rows; i++)
			for (unsigned int j = 0; j<columns; j++)
				temp->twoDArray[i][j] = asin(this->twoDArray[i][j]);
	}
	return temp;
}

Matrix* Matrix::acosMatrix() {
	Matrix *temp = new Matrix;
	if (this->notMatrix) {
		temp->setNum(acos(this->num));
	}
	else {
		temp->setSize(this->rows, this->columns);
		for (unsigned int i = 0; i<rows; i++)
			for (unsigned int j = 0; j<columns; j++)
				temp->twoDArray[i][j] = acos(this->twoDArray[i][j]);
	}
	return temp;
}

Matrix* Matrix::atanMatrix() {
	Matrix *temp = new Matrix;
	if (this->notMatrix) {
		temp->setNum(sin(this->num));
	}
	else {
		temp->setSize(this->rows, this->columns);
		for (unsigned int i = 0; i<rows; i++)
			for (unsigned int j = 0; j<columns; j++)
				temp->twoDArray[i][j] = atan(this->twoDArray[i][j]);
	}
	return temp;
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


std::string Converttostring(float number) {
	std::ostringstream buff;
	buff << fixed << setprecision(2) << number;
	return buff.str();
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




























struct operators
{
	int pos;
	string op;
};



/*function 3shan mafesh temp wa7ed ytkrar marten*/
void storeTemp(Matrix & M, vector<Matrix>& storedMatrices)
{
	int index = -1;
	for (size_t i = 0; i < storedMatrices.size(); i++)
	{
		if (M.getName() == storedMatrices[i].getName())
		{
			index = i; break;
		}
	}
	if (index == -1)storedMatrices.push_back(M);
	else storedMatrices[index] = M;
}

//temporary name increment function
string incrementTempName(string temporaryName)
{
	if (temporaryName[6] < '9')temporaryName[6]++;
	else
	{
		temporaryName[6] = '1';
		if (temporaryName[5] < '9')
			temporaryName[5]++;
		else
		{
			temporaryName[5] = '1';
			(temporaryName[4] < '9') ? temporaryName[4]++ : temporaryName[4] = '1';
		}
	}
	return temporaryName;
}




/*MultiOperations Handeling Function*/

Matrix Matrix::multiOpHandling(string RHS, vector<Matrix>& storedMatrices, vector<string>& systemCommands, bool &print)
{


	//getting the operators in the i/p
	vector <operators> ops(0);
	int openedBracketsNo = 0;
	int closedBracketsNo = 0;
	operators currentOp;
	Matrix* temp = new Matrix;

	static string temporaryName = "temp111"; //m3mlthash 00 3shan el 0 e3tbrha ely hya bta3t el termination de wtala3 error
	if (RHS[RHS.length() - 1] == ';')
	{
		print = false;
		RHS.erase((RHS.length() - 1), 1);
	}

	if (RHS.find("+") == RHS.npos && RHS.find("-") == RHS.npos && RHS.find("*") == RHS.npos && RHS.find("/") == RHS.npos && RHS.find("inv") == RHS.npos && RHS.find("'") == RHS.npos && RHS.find("`") == RHS.npos && RHS.find("det") == RHS.npos && RHS.find('(') == RHS.npos && RHS.find(')') == RHS.npos && RHS.find('[') == RHS.npos && RHS.find(']') == RHS.npos && RHS.find('^') == RHS.npos)
	{
		if (RHS.find_first_not_of(numbers) == RHS.npos) {        //numbers
			double value = 0;
			char* e;
			value = strtod(RHS.c_str(), &e);
			if (*e != 0) throw("Invalid number!");
			Matrix temp = value;
			temp.setName(temporaryName);
			storeTemp(temp, storedMatrices);
			temporaryName = incrementTempName(temporaryName);
			return temp;
		}
		for (size_t i = 0; i < storedMatrices.size(); i++)
		{
			if (storedMatrices[i].getName() == RHS)
			{
				Matrix temp = storedMatrices[i];
				return temp;
			}
		}
		throw("Error no stored matrix with this name");
	}

	for (size_t i = 0; i < RHS.length(); i++)
	{

		if (RHS[i] == '(')
			openedBracketsNo++;
		else if (RHS[i] == ')')
			closedBracketsNo++;
		else if (RHS[i] == '.')
		{
			if (RHS[i + 1] == '+' || RHS[i + 1] == '-' || RHS[i + 1] == '/' || RHS[i + 1] == '*')
			{
				currentOp.op = RHS[i, i + 1];
				currentOp.pos = i;
				ops.push_back(currentOp);
				i++;
			}
		}
		else if (RHS[i] == '+' || RHS[i] == '-' || RHS[i] == '/' || RHS[i] == '*')
		{
			currentOp.op = RHS[i];
			currentOp.pos = i;
			ops.push_back(currentOp);
		}
	}
	if (openedBracketsNo > closedBracketsNo)throw("Error opened bracket without closure");
	if (openedBracketsNo < closedBracketsNo)throw("Error closed bracket without opening");


	//special functions
	while (RHS.find("eye(") != RHS.npos)
	{
		int startPosition = RHS.find("eye(");
		int endPosition = RHS.substr(startPosition).find(')') + startPosition;
		if (endPosition == RHS.npos)throw("Error opened bracket without closure");
		string insideBrackets = RHS.substr(startPosition + 4, endPosition - (startPosition+4));
		if (insideBrackets.find_first_not_of("0123456789,") != insideBrackets.npos)throw("invalid special function format (eye)");
		string firstP, secondP;
		int rowsP, columnsP;
		firstP = insideBrackets.substr(0,insideBrackets.find(','));
		secondP = insideBrackets.substr((insideBrackets.find(',')+1));
		rowsP = strtod(firstP.c_str(), NULL);
		columnsP = strtod(secondP.c_str(), NULL);
		Matrix result;
		result.eye(rowsP, columnsP);
		result.setName(temporaryName);
		storeTemp(result, storedMatrices);
		RHS.erase(startPosition, endPosition - startPosition);
		openedBracketsNo--; closedBracketsNo--;
		RHS.insert(startPosition, temporaryName);
		temporaryName = incrementTempName(temporaryName);
	}
	while (RHS.find("zeros(") != RHS.npos)
	{
		int startPosition = RHS.find("zeros(");
		int endPosition = RHS.substr(startPosition).find(')') + startPosition;
		if (endPosition == RHS.npos)throw("Error opened bracket without closure");
		string insideBrackets = RHS.substr(startPosition + 6, endPosition - (startPosition + 6));
		if (insideBrackets.find_first_not_of("0123456789,") != insideBrackets.npos)throw("invalid special function format (zeros)");
		string firstP, secondP;
		int rowsP, columnsP;
		firstP = insideBrackets.substr(0, insideBrackets.find(','));
		secondP = insideBrackets.substr((insideBrackets.find(',') + 1));
		rowsP = strtod(firstP.c_str(), NULL);
		columnsP = strtod(secondP.c_str(), NULL);
		Matrix result;
		result.zeros(rowsP, columnsP);
		result.setName(temporaryName);
		storeTemp(result, storedMatrices);
		RHS.erase(startPosition, endPosition - startPosition);
		openedBracketsNo--; closedBracketsNo--;
		RHS.insert(startPosition, temporaryName);
		temporaryName = incrementTempName(temporaryName);
	}
	while (RHS.find("ones(") != RHS.npos)
	{
		int startPosition = RHS.find("ones(");
		int endPosition = RHS.substr(startPosition).find(')') + startPosition;
		if (endPosition == RHS.npos)throw("Error opened bracket without closure");
		string insideBrackets = RHS.substr(startPosition + 5, endPosition - (startPosition + 5));
		if (insideBrackets.find_first_not_of("0123456789,") != insideBrackets.npos)throw("invalid special function format (ones)");
		string firstP, secondP;
		int rowsP, columnsP;
		firstP = insideBrackets.substr(0, insideBrackets.find(','));
		secondP = insideBrackets.substr((insideBrackets.find(',') + 1));
		rowsP = strtod(firstP.c_str(), NULL);
		columnsP = strtod(secondP.c_str(), NULL);
		Matrix result;
		result.ones(rowsP, columnsP);
		result.setName(temporaryName);
		storeTemp(result, storedMatrices);
		RHS.erase(startPosition, endPosition - startPosition);
		openedBracketsNo--; closedBracketsNo--;
		RHS.insert(startPosition, temporaryName);
		temporaryName = incrementTempName(temporaryName);
	}
	while (RHS.find("rand(") != RHS.npos)
	{
		int startPosition = RHS.find("rand(");
		int endPosition = RHS.substr(startPosition).find(')') + startPosition;
		if (endPosition == RHS.npos)throw("Error opened bracket without closure");
		string insideBrackets = RHS.substr(startPosition + 5, endPosition - (startPosition + 5));
		if (insideBrackets.find_first_not_of("0123456789,") != insideBrackets.npos)throw("invalid special function format (rand)");
		string firstP, secondP;
		int rowsP, columnsP;
		firstP = insideBrackets.substr(0, insideBrackets.find(','));
		secondP = insideBrackets.substr((insideBrackets.find(',') + 1));
		rowsP = strtod(firstP.c_str(), NULL);
		columnsP = strtod(secondP.c_str(), NULL);
		Matrix result;
		result.random(rowsP, columnsP);
		result.setName(temporaryName);
		storeTemp(result, storedMatrices);
		RHS.erase(startPosition, endPosition - startPosition);
		openedBracketsNo--; closedBracketsNo--;
		RHS.insert(startPosition, temporaryName);
		temporaryName = incrementTempName(temporaryName);
	}
	//end special functions



	//Brackets (including the det, inv and trig functions brackets)
	while (openedBracketsNo > 0)
	{
		int openBIndex, closeBIndex;
		openBIndex = RHS.rfind('(');
		closeBIndex = RHS.substr(openBIndex).find(')') + openBIndex;
		if (closeBIndex == RHS.npos)throw("Error opened bracket without closure");

		string insideBrackets = RHS.substr(openBIndex + 1, closeBIndex - (openBIndex + 1));
		Matrix temporary;
		temporary = multiOpHandling(insideBrackets, storedMatrices, systemCommands, print);
		RHS.erase(openBIndex + 1, closeBIndex - (openBIndex + 1));
		RHS.insert(openBIndex + 1, temporary.getName());
		temporaryName = incrementTempName(temporaryName);
		closeBIndex = RHS.substr(openBIndex).find(')') + openBIndex;


		bool oneOperandOp = false;
		bool inverseTrigDet = false;

		if (openBIndex >= 4)
		{
			/* INVERSE TRIGONOMETRIC */
			if (RHS[openBIndex - 1] == 'n' && RHS[openBIndex - 2] == 'i' && RHS[openBIndex - 3] == 's' && RHS[openBIndex - 4] == 'a'
				|| RHS[openBIndex - 1] == 's' && RHS[openBIndex - 2] == 'o' && RHS[openBIndex - 3] == 'c' && RHS[openBIndex - 4] == 'a'
				|| RHS[openBIndex - 1] == 'n' && RHS[openBIndex - 2] == 'a' && RHS[openBIndex - 3] == 't' && RHS[openBIndex - 4] == 'a')
			{
				oneOperandOp = true;
				inverseTrigDet = true;
				string operand = temporary.getName();
				bool found = false;
				char functionDet = RHS[openBIndex - 2];

				for (unsigned int i = 0; i < storedMatrices.size(); i++)
					if (operand == storedMatrices[i].getName()) {
						found = true;
						*temp = storedMatrices[i];
						break;
					}
				if (!found) {
					if (operand == "PI")  *temp = PI;
					else if (operand.find_first_not_of(numbers) != operand.npos) throw("Operand not found.");
					else *temp = strtod(operand.c_str(), NULL);
				}

				switch (functionDet)
				{
				case 'i':
					temp = temp->asinMatrix();
					break;
				case 'o':
					temp = temp->acosMatrix();
					break;
				case 'a':
					temp = temp->atanMatrix();
					break;
				}
				temp->setName(temporaryName);

				storeTemp(*temp, storedMatrices);
				RHS.erase(openBIndex - 4, closeBIndex - (openBIndex - 5));
				openedBracketsNo--; closedBracketsNo--;
				RHS.insert(openBIndex - 4, temporaryName);
				temporaryName = incrementTempName(temporaryName);
			}

			/*Square Root*/
			if (RHS[openBIndex - 1] == 't' && RHS[openBIndex - 2] == 'r' && RHS[openBIndex - 3] == 'q' && RHS[openBIndex - 4] == 's')
			{
				oneOperandOp = true;
				string operand = temporary.getName();
				bool found = false;
				for (unsigned int i = 0; i < storedMatrices.size(); i++)
					if (operand == storedMatrices[i].getName())
					{
						found = true;
						*temp = storedMatrices[i];
					}
				if (!found)
				{

					if (operand.find_first_not_of(numbers) != operand.npos) throw("Operand not found.");
					if (operand.find_first_not_of(numbers) == operand.npos && operand.find("temp") == operand.npos) {  //if the user used a scalar nonsaved value
						char *e;
						double value = strtod(operand.c_str(), &e);
						if (*e != 0) throw("invalid number.");
						else *temp = value;
					}
				}

				temp = temp->sqrtMatrix();
				temp->setName(temporaryName);

				storeTemp(*temp, storedMatrices);

				RHS.erase(openBIndex - 4, closeBIndex - (openBIndex - 5));
				openedBracketsNo--; closedBracketsNo--;
				RHS.insert(openBIndex - 4, temporaryName);
				temporaryName = incrementTempName(temporaryName);
			}
		}

		if (openBIndex >= 3)
		{

			//inverse
			if (RHS[openBIndex - 1] == 'v' && RHS[openBIndex - 2] == 'n' && RHS[openBIndex - 3] == 'i')
			{
				oneOperandOp = true;
				int operandIndex = -1;
				for (size_t i = 0; i < storedMatrices.size(); i++)
					if (temporary.getName() == storedMatrices[i].getName())
						operandIndex = i;
				if (operandIndex == -1)throw("Operand not defined.");
				else {
					if (storedMatrices[operandIndex].is_scalar()) throw("You must enter a matrix.");
					else
					{
						temp = storedMatrices[operandIndex].inverse();
						temp->setName(temporaryName);
					}
				}
				storeTemp(*temp, storedMatrices);

				RHS.erase(openBIndex - 3, closeBIndex - (openBIndex - 4));
				openedBracketsNo--; closedBracketsNo--;
				RHS.insert(openBIndex - 3, temporaryName);
				temporaryName = incrementTempName(temporaryName);
			}

			//end inverse


			//determinant
			else if (RHS[openBIndex - 1] == 't' && RHS[openBIndex - 2] == 'e' && RHS[openBIndex - 3] == 'd')
			{
				oneOperandOp = true;
				int operandIndex = -1;
				for (size_t i = 0; i < storedMatrices.size(); i++)
				{
					if (temporary.getName() == storedMatrices[i].getName())
					{
						operandIndex = i;
						break;
					}
				}
				if (operandIndex == -1)throw("Operand not defined.");
				else {
					if (storedMatrices[operandIndex].is_scalar()) throw("You must enter a matrix.");
					else {
						*temp = storedMatrices[operandIndex].determinant();
						temp->setName(temporaryName);
					}

					storeTemp(*temp, storedMatrices);

					RHS.erase(openBIndex - 3, closeBIndex - (openBIndex - 4));
					openedBracketsNo--; closedBracketsNo--;
					RHS.insert(openBIndex - 3, temporaryName);
					temporaryName = incrementTempName(temporaryName);
				}
			}
			//end determinant


			//TRIG
			else if (RHS[openBIndex - 1] == 'n' && RHS[openBIndex - 2] == 'i' && RHS[openBIndex - 3] == 's'
				|| RHS[openBIndex - 1] == 's' && RHS[openBIndex - 2] == 'o' && RHS[openBIndex - 3] == 'c'
				|| RHS[openBIndex - 1] == 'n' && RHS[openBIndex - 2] == 'a' && RHS[openBIndex - 3] == 't')
			{
				if (!inverseTrigDet)
				{
					oneOperandOp = true;
					string operand = temporary.getName();
					bool found = false;
					char functionDet = RHS[openBIndex - 2];
					for (unsigned int i = 0; i < storedMatrices.size(); i++)
						if (operand == storedMatrices[i].getName()) {
							found = true;
							*temp = storedMatrices[i];
							break;
						}
					if (!found) {
						if (operand == "PI")  *temp = PI;
						else if (operand.find_first_not_of(numbers) != operand.npos) throw("Operand not found.");
						else *temp = strtod(operand.c_str(), NULL);
					}

					switch (functionDet)
					{
					case 'i':
						temp = temp->sinMatrix();
						break;
					case 'o':
						temp = temp->cosMatrix();
						break;
					case 'a':
						temp = temp->tanMatrix();
						break;
					}
					temp->setName(temporaryName);

					storeTemp(*temp, storedMatrices);

					RHS.erase(openBIndex - 3, closeBIndex - (openBIndex - 4));
					openedBracketsNo--; closedBracketsNo--;
					RHS.insert(openBIndex - 3, temporaryName);
					temporaryName = incrementTempName(temporaryName);
				}
			}
			//ENDTRIG



		}

		if (!oneOperandOp)
		{
			RHS.erase(openBIndex, 1);
			openedBracketsNo--;
			RHS.erase(closeBIndex - 1, 1);
			closedBracketsNo--;
		}


	}
	//end Brackets

	while (RHS.find('(') != RHS.npos) {
		RHS.erase(RHS.find('('), 1); openedBracketsNo--;
	}
	while (RHS.find(')') != RHS.npos) {
		RHS.erase(RHS.find(')'), 1); closedBracketsNo--;
	}

	//transpose
	while (RHS.find("`") != RHS.npos || RHS.find("'") != RHS.npos)
	{
		unsigned int pos;
		if (RHS.find("`") != RHS.npos)
			pos = RHS.find("`");
		else
			pos = RHS.find("'");
		string transposeOperand = "";
		int transposeOperandstart;
		for (size_t i = pos - 1; i >= 0; i--)
		{
			if (i > RHS.length())break;
			if (RHS[i] == '+' || RHS[i] == '-' || RHS[i] == '*' || RHS[i] == '/' || RHS[i] == ')' || RHS[i] == '(')
				break;
			char c = RHS[i];
			transposeOperandstart = i;
			transposeOperand = c + transposeOperand;
		}
		int operandIndex = -1;
		for (size_t i = 0; i < storedMatrices.size(); i++)
		{
			if (transposeOperand == storedMatrices[i].getName())
				operandIndex = i;
		}
		if (operandIndex == -1)throw("Operand not defined.");
		else
		{
			if (storedMatrices[operandIndex].is_scalar()) throw("You must enter a matrix.");
			else
			{
				temp = storedMatrices[operandIndex].transpose();
				temp->setName(temporaryName);
				storeTemp(*temp, storedMatrices);

				RHS.erase(transposeOperandstart, (pos + 1) - transposeOperandstart);
				RHS.insert(transposeOperandstart, temporaryName);
				temporaryName = incrementTempName(temporaryName);

			}
		}

	}  //end transpose



	   //negate
	if (RHS.find("+-") != RHS.npos || RHS.find("--") != RHS.npos || RHS.find("^-") != RHS.npos || RHS.find("/-") != RHS.npos || RHS.find("*-") != RHS.npos || RHS.find('-') == 0)
	{
		vector <size_t> posNegatives;
		for (size_t i = 0; i < RHS.length(); i++)
		{
			if (RHS[i] == '-')posNegatives.push_back(i);
		}
		for (size_t i = posNegatives.size(); i >= 1; i--)
		{
			size_t pos = posNegatives[i - 1];
			if (pos != 0)
			{
				if (RHS[pos - 1] != '+' && RHS[pos - 1] != '-' && RHS[pos - 1] != '*' && RHS[pos - 1] != '/')
					continue;
			}
			string negOperand;
			for (size_t j = pos + 1; j < RHS.length(); j++)
			{
				if (RHS[j] == '+' || RHS[j] == '-' || RHS[j] == '*' || RHS[j] == '/' || RHS[j] == ')' || RHS[j] == '(' || RHS[j] == 39 /* ely hya ' */ || RHS[j] == '`' || RHS[j] == ']' || RHS[j] == '[' || RHS[j] == '^')
					break;
				char c = RHS[j];
				negOperand.push_back(c);
			}
			int operandindex = -1;
			for (size_t j = 0; j < storedMatrices.size(); j++)
			{
				if (negOperand == storedMatrices[j].getName())
					operandindex = j;
			}
			if (operandindex == -1)
			{
				if (negOperand.find_first_not_of(numbers) != negOperand.npos) throw("Operand not found.");
				else
				{
					temp->setNum(0 - (strtod(negOperand.c_str(), NULL)));
					temp->setName(temporaryName);
				}
			}
			else
			{
				Matrix m = -storedMatrices[operandindex];
				*temp = m;
				temp->setName(temporaryName);
			}
			RHS.erase(pos, (negOperand.length() + 1));
			RHS.insert(pos, temporaryName);
			storeTemp(*temp, storedMatrices);
			temporaryName = incrementTempName(temporaryName);
		}
	}


	//Power
	/////////////////////////////////////
	//////////////////////////////////////
	////////////////////////////////////////
	///////////////////////////////////////////
	///////////////////////////////////////////
	while (RHS.find('^') != RHS.npos)
	{
		Matrix *parameter1 = new Matrix, *parameter2 = new Matrix;
		string op;
		int opIndex;
		vector<string> operands(2);
		vector<int> operandIndeces(2);
		for (size_t i = 1; i < RHS.length(); i++)
		{
			if (RHS[i] == '.' && (RHS[i + 1] == '^'))
			{
				for (size_t j = i; j >= 1; j--)
				{
					if (RHS[j - 1] == '+' || RHS[j - 1] == '-' || RHS[j - 1] == '*' || RHS[j - 1] == '/' || RHS[j - 1] == ')' || RHS[j - 1] == '(' || RHS[j - 1] == 39 /* ely hya ' */ || RHS[j - 1] == '`' || RHS[j - 1] == '^' || RHS[j - 1] == ']' || RHS[j - 1] == '[')
						break;
					char c = RHS[j - 1];
					operands[0] = c + operands[0];
				}
				for (size_t j = i + 2; j < RHS.length(); j++)
				{
					if (RHS[j] == '+' || RHS[j] == '-' || RHS[j] == '*' || RHS[j] == '/' || RHS[j] == ')' || RHS[j] == '(' || RHS[j] == 39 /* ely hya ' */ || RHS[j] == '`' || RHS[j] == ']' || RHS[j] == '[')
						break;
					char c = RHS[j];
					operands[1].push_back(c);
				}

				opIndex = i;
				op = RHS.substr(i, 2);
				break;
			}
			if (RHS[i] == '^')
			{
				for (size_t j = i; j >= 1; j--)
				{
					if (RHS[j - 1] == '+' || RHS[j - 1] == '-' || RHS[j - 1] == '*' || RHS[j - 1] == '/' || RHS[j - 1] == ')' || RHS[j - 1] == '(' || RHS[j - 1] == 39 /* ely hya ' */ || RHS[j - 1] == '`' || RHS[j - 1] == '^' || RHS[j - 1] == ']' || RHS[j - 1] == '[')
						break;
					char c = RHS[j - 1];
					operands[0] = c + operands[0];
				}
				for (size_t j = i + 1; j < RHS.length(); j++)
				{
					if (RHS[j] == '+' || RHS[j] == '-' || RHS[j] == '*' || RHS[j] == '/' || RHS[j] == ')' || RHS[j] == '(' || RHS[j] == 39 /* ely hya ' */ || RHS[j] == '`' || RHS[j] == '^' || RHS[j] == ']' || RHS[j] == '[')
						break;
					char c = RHS[j];
					operands[1].push_back(c);
				}
				op = RHS[i];
				opIndex = i;
				break;
			}
		}

		for (size_t i = 0; i < storedMatrices.size(); i++) {
			if (operands[0] == storedMatrices[i].getName())
				operandIndeces[0] = i;
			if (operands[1] == storedMatrices[i].getName())
				operandIndeces[1] = i;
		}
		bool firstNo = false, secondNo = false;
		double fNo, sNo;

		if ((operandIndeces[0] == -1 && operands[0].find_first_not_of(alphabets) == operands[0].npos)
			|| (operandIndeces[1] == -1 && operands[1].find_first_not_of(alphabets) == operands[1].npos)) throw("operand not defined.");

		if (operands[0].find_first_not_of(numbers) == operands[0].npos && operands[0].find("temp") == operands[0].npos) {  //if the user used a scalar nonsaved value
			char *e;
			double value = strtod(operands[0].c_str(), &e);
			if (*e != 0) throw("invalid number.");
			else *parameter1 = value;
			firstNo = true;
			fNo = value;
		}
		else *parameter1 = storedMatrices[operandIndeces[0]];
		if (operands[1].find_first_not_of(numbers) == operands[1].npos && operands[1].find("temp") == operands[1].npos) {
			char *e;
			double value = strtod(operands[1].c_str(), &e);
			if (*e != 0) throw("invalid number.");
			else *parameter2 = value;
			secondNo = true;
			sNo = value;
		}
		else *parameter2 = storedMatrices[operandIndeces[1]];


		if (firstNo&&secondNo)*temp = pow(fNo, sNo);
		else
			temp = Matrix::powerMatrix(*parameter1, *parameter2);

		RHS.erase(opIndex - operands[0].length(), operands[0].length() + operands[1].length() + op.length());
		RHS.insert(opIndex - operands[0].length(), temporaryName);

		temp->setName(temporaryName);
		storeTemp(*temp, storedMatrices);
		temporaryName = incrementTempName(temporaryName);

		delete parameter1;
		delete parameter2;
	}






	while (RHS.find("+") != RHS.npos || RHS.find("-") != RHS.npos || RHS.find("*") != RHS.npos || RHS.find("/") != RHS.npos)
	{

		Matrix *parameter1 = new Matrix, *parameter2 = new Matrix;
		string op;
		int opIndex;
		vector<string> operands(2);
		vector<int> operandIndeces   ;//****************************************************************************error resolved by adel*****
		operandIndeces.push_back(-1);
		operandIndeces.push_back(-1);
		bool timesDivideDetect = false;
		for (size_t i = 1; i < RHS.length(); i++)
		{
			if (RHS[i] == '.' && (RHS[i + 1] == '*' || RHS[i + 1] == '/'))
			{
				for (size_t j = i; j >= 1; j--)
				{
					if (RHS[j - 1] == '+' || RHS[j - 1] == '-' || RHS[j - 1] == '*' || RHS[j - 1] == '/' || RHS[j - 1] == ')' || RHS[j - 1] == '(' || RHS[j - 1] == 39 /* ely hya ' */ || RHS[j - 1] == '`' || RHS[j - 1] == '^' || RHS[j - 1] == ']' || RHS[j - 1] == '[')
						break;
					char c = RHS[j - 1];
					operands[0] = c + operands[0];
				}
				for (size_t j = i + 2; j < RHS.length(); j++)
				{
					if (RHS[j] == '+' || RHS[j] == '-' || RHS[j] == '*' || RHS[j] == '/' || RHS[j] == ')' || RHS[j] == '(' || RHS[j] == 39 /* ely hya ' */ || RHS[j] == '`' || RHS[j] == '^' || RHS[j] == ']' || RHS[j] == '[')
						break;
					char c = RHS[j];
					operands[1].push_back(c);
				}
				timesDivideDetect = true;
				opIndex = i;
				op = RHS.substr(i, 2);
				break;
			}
			if (RHS[i] == '*' || RHS[i] == '/')
			{
				for (size_t j = i; j >= 1; j--)
				{
					if (RHS[j - 1] == '+' || RHS[j - 1] == '-' || RHS[j - 1] == '*' || RHS[j - 1] == '/' || RHS[j - 1] == ')' || RHS[j - 1] == '(' || RHS[j - 1] == 39 /* ely hya ' */ || RHS[j - 1] == '`' || RHS[j - 1] == '^' || RHS[j - 1] == ']' || RHS[j - 1] == '[')
						break;
					char c = RHS[j - 1];
					operands[0] = c + operands[0];
				}
				for (size_t j = i + 1; j < RHS.length(); j++)
				{
					if (RHS[j] == '+' || RHS[j] == '-' || RHS[j] == '*' || RHS[j] == '/' || RHS[j] == ')' || RHS[j] == '(' || RHS[j] == 39 /* ely hya ' */ || RHS[j] == '`' || RHS[j] == '^' || RHS[j] == ']' || RHS[j] == '[')
						break;
					char c = RHS[j];
					operands[1].push_back(c);
				}
				op = RHS[i];
				opIndex = i;
				timesDivideDetect = true;
				break;
			}

		}

		if (timesDivideDetect == false)
		{
			for (size_t i = 1; i < RHS.length(); i++)
			{
				if (RHS[i] == '.' && (RHS[i + 1] == '-' || RHS[i + 1] == '+'))
				{
					for (size_t j = i; j >= 1; j--)
					{
						if (RHS[j - 1] == '+' || RHS[j - 1] == '-' || RHS[j - 1] == '*' || RHS[j - 1] == '/' || RHS[j - 1] == ')' || RHS[j - 1] == '(' || RHS[j - 1] == 39 /* ely hya ' */ || RHS[j - 1] == '`' || RHS[j - 1] == '^' || RHS[j - 1] == ']' || RHS[j - 1] == '[')
							break;
						char c = RHS[j - 1];
						operands[0] = c + operands[0];
					}
					for (size_t j = i + 2; j < RHS.length(); j++)
					{
						if (RHS[j] == '+' || RHS[j] == '-' || RHS[j] == '*' || RHS[j] == '/' || RHS[j] == ')' || RHS[j] == '(' || RHS[j] == 39 /* ely hya ' */ || RHS[j] == '`' || RHS[j] == '^' || RHS[j] == ']' || RHS[j] == '[')
							break;
						char c = RHS[j];
						operands[1].push_back(c);
					}
					opIndex = i;
					op = RHS.substr(i, 2);
					break;
				}
				if (RHS[i] == '+' || RHS[i] == '-')
				{
					for (size_t j = i; j >= 1; j--)
					{
						if (RHS[j - 1] == '+' || RHS[j - 1] == '-' || RHS[j - 1] == '*' || RHS[j - 1] == '/' || RHS[j - 1] == ')' || RHS[j - 1] == '(' || RHS[j - 1] == 39 /* ely hya ' */ || RHS[j - 1] == '`' || RHS[j - 1] == '^' || RHS[j - 1] == ']' || RHS[j - 1] == '[')
							break;
						char c = RHS[j - 1];
						operands[0] = c + operands[0];
					}
					for (size_t j = i + 1; j < RHS.length(); j++)
					{
						if (RHS[j] == '+' || RHS[j] == '-' || RHS[j] == '*' || RHS[j] == '/' || RHS[j] == ')' || RHS[j] == '(' || RHS[j] == 39 /* ely hya ' */ || RHS[j] == '`' || RHS[j] == '^' || RHS[j] == ']' || RHS[j] == '[')
							break;
						char c = RHS[j];
						operands[1].push_back(c);
					}
					op = RHS[i];
					opIndex = i;
					break;
				}
			}
		}

		for (size_t i = 0; i < storedMatrices.size(); i++) {
			if (operands[0] == storedMatrices[i].getName())
				operandIndeces[0] = i;
			if (operands[1] == storedMatrices[i].getName())
				operandIndeces[1] = i;
		}

		if ((operandIndeces[0] == -1 && operands[0].find_first_not_of(alphabets) == operands[0].npos)
			|| (operandIndeces[1] == -1 && operands[1].find_first_not_of(alphabets) == operands[1].npos)) throw("operand not defined.");

		if (operands[0].find_first_not_of(numbers) == operands[0].npos && operands[0].find("temp") == operands[0].npos) {  //if the user used a scalar nonsaved value, ex:(a = 1 ./ b)
			char *e;
			double value = strtod(operands[0].c_str(), &e);
			if (*e != 0) throw("invalid number.");
			else *parameter1 = value;
		}
		else *parameter1 = storedMatrices[operandIndeces[0]];
		if (operands[1].find_first_not_of(numbers) == operands[1].npos && operands[1].find("temp") == operands[1].npos) {
			char *e;
			double value = strtod(operands[1].c_str(), &e);
			if (*e != 0) throw("invalid number.");
			else *parameter2 = value;
		}
		
		else {//cout<<"operandIndeces[1] = "<<operandIndeces[1]<<endl;
		
		
		*parameter2 = storedMatrices[operandIndeces[1]];}
		

		if (op == "+") *temp = *parameter1 + *parameter2;
		else if (op == "-") *temp = *parameter1 - *parameter2;
		else if (op == "*") *temp = *parameter1 * *parameter2;
		else if (op == "/") *temp = *parameter1 / *parameter2;
		else if (op == ".+") temp = Matrix::pseudoAdd(*parameter1, *parameter2);
		else if (op == ".-") temp = Matrix::pseudoSubtract(*parameter1, *parameter2);
		else if (op == ".*") temp = Matrix::pseudoProduct(*parameter1, *parameter2);
		else if (op == "./") temp = Matrix::pseudoDiv(*parameter1, *parameter2);

		RHS.erase(opIndex - operands[0].length(), operands[0].length() + operands[1].length() + op.length());
		RHS.insert(opIndex - operands[0].length(), temporaryName);

		temp->setName(temporaryName);
		storeTemp(*temp, storedMatrices);
		temporaryName = incrementTempName(temporaryName);

		delete parameter1;
		delete parameter2;
	}
	//end mathematical operations


	for (size_t i = 0; i < storedMatrices.size(); i++)
	{
		if (storedMatrices[i].getName() == RHS)
		{
			Matrix temp = storedMatrices[i];
			return temp;
		}
	}

	return *temp;

}