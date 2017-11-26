
#include<stdio.h>
#include <sstream>
#include "Matrix.h"
#include <iomanip>
#include <string>

/************START CONSTRUCTORS*******************/
double** twoDArray = NULL;

//Default Constructor:
Matrix::Matrix()
:twoDArray(NULL){}

//Scalar Constructor:
Matrix::Matrix(double value)
:twoDArray(NULL),
 notMatrix(true),
 rows(0),
 columns(0),
 num(value){}

//Size Constructor:
Matrix::Matrix(unsigned int rows,unsigned int cols)
:twoDArray(NULL),
 notMatrix(false),
 num(0)
{
    this->setSize(rows,cols);
}

//Copy Constructor:
Matrix::Matrix(const Matrix& A)
:twoDArray(NULL),
 num(0)
{
    copyMatrix(&A);
}

//String Constructor
Matrix::Matrix(string name,string matrixString)
:twoDArray(NULL),
 notMatrix(false),
 rows(0),
 columns(0),
 num(0)
{
    if(name.find_first_not_of(alphabets)==0) throw("Variable name is not valid.");

    this->name = name;

    if(matrixString.find("[")!=matrixString.npos)matrixString.erase(0,matrixString.find("[")+1);

    while(matrixString.find("]")!=matrixString.npos)
        matrixString.erase(matrixString.begin()+matrixString.find("]"),matrixString.end());

    vector<string> arrayOfRows(0);
    if(matrixString.find(";")==matrixString.npos){
        arrayOfRows.push_back(matrixString);
        this->rows=1;
    }
    else{
        while(1){

            if(matrixString.find(";")!=matrixString.npos && matrixString.find(";")!=matrixString.length()-1){
                arrayOfRows.push_back(matrixString.substr(0,matrixString.find(";")));
                matrixString.erase(0,matrixString.find(";")+1);
                this->rows++;
            }
            else{
                arrayOfRows.push_back(matrixString);
                this->rows++;
                break;
            }
        }
    }
    unsigned int colSafe = 0;
    vector<double> inputElements(0);
    for(size_t i=0;i<this->rows;i++){
        this->columns=0;
        char* rowBuffer = new char[arrayOfRows[i].length()+1];
        strcpy(rowBuffer,arrayOfRows[i].c_str());
        char* e;     //check for atof errors (two floating point, ..etc)
        char* token = strtok(rowBuffer , " ,");
        while(token){
            double element = 0;
            this->columns++;
            element = strtod(token, &e);
            if (*e != '\0' && *e != ';'){   // if there's error it'll be passed to 'e' so it will not be null
                throw("Invalid numbers");
            }
            inputElements.push_back(element);
            token = strtok(NULL," ,");
        }
        colSafe += this->columns;
        delete rowBuffer;
    }

    arrayOfRows.clear();

    if (this->columns != colSafe/(double)this->rows)throw("Size Error."); // handle this error (if no of columns is not constant)

    this->setSize(this->rows,this->columns);
    int k=0;
    for(size_t i=0;i<this->rows;i++)
        for (size_t j =0 ;j<this->columns;j++){
            this->twoDArray[i][j]=inputElements[k];
            k++;
        }
    inputElements.clear();
}
/*********************END CONSTRUCTORS*******************/

/*********************DESTRUCTOR***********************/
Matrix::~Matrix(){
    if(this->twoDArray){
        for(size_t i=0;i<this->rows;i++)
            delete[] this->twoDArray[i];
        this->twoDArray=NULL;
    }
    this->rows = 0, this->columns=0, this->num=0, this->name="";
    this->notMatrix=false;
}
/*****************END DESTRUCTOR***********************/
void Matrix::copyMatrix(const Matrix* A){
    if(this->name=="")
        this->setName(A->name);
    if(this->twoDArray){
        for(size_t i=0;i<this->rows;i++)
            delete[] (this->twoDArray[i]);
        this->twoDArray=NULL;
    }
    if(A->notMatrix){
        this->rows=0;
        this->columns=0;
        this->num = A->num;
        this->notMatrix=true;
    }
    else{
        this->notMatrix=false;
        this->setSize(A->rows,A->columns);
        for(size_t i = 0 ; i < this-> rows ; i++)
            for(size_t j=0;j<this->columns;j++)
                this->twoDArray[i][j]=A->twoDArray[i][j];
    }
}

/***********OPERATORS*****************/
//Assignment Operators:
Matrix& Matrix::operator=(Matrix& A){
    this->copyMatrix(&A);
    return *this;
}

Matrix& Matrix::operator=(double value){
    this->setNum(value);
    return *this;
}

//Mathematical Operators:
Matrix& Matrix::operator+(Matrix& A){
    Matrix* result = add(*this,A);
    return *result;
}
Matrix& Matrix::operator-(Matrix& A){
    Matrix* result = subtract(*this,A);
    return *result;
}
Matrix& Matrix::operator+(double value){
    Matrix *temp = new Matrix, *result;
    temp->setNum(value);
    result = add(*this,*temp);
    return *result;
}
Matrix& Matrix::operator-(double value){
    Matrix *temp = new Matrix, *result;
    temp->setNum(value);
    result = subtract(*this,*temp);
    return *result;
}
Matrix& Matrix::operator*(Matrix& A){
    Matrix* result = product(*this,A);
    return *result;
}

Matrix& Matrix::operator*(double value){
    Matrix  *temp = new Matrix, *result;
    temp->setNum(value);
    result=product(*this,*temp);
    return *result;
}

Matrix& Matrix::operator/(Matrix& A){
    Matrix* result = divide(*this,A);
    return *result;
}

Matrix& Matrix::operator/(double value){
    Matrix *temp = new Matrix, *result;
    temp->setNum(value);
    result=divide(*this,*temp);
    return *result;
}

//negative unary operator:
Matrix& Matrix::operator-(){
    Matrix* result = this->negative();
    return *result;
}


/*******SETTERS AND GETTERS***********/
//Setters:
void Matrix::setName(string name){
    this->name=name;
}

void Matrix::setNum(double num){
    if(this->twoDArray){
        for(size_t i = 0;i<this->rows;i++)
            delete[] this->twoDArray[i];
        this->twoDArray = NULL;
    }
    this->num=num;
    this->notMatrix=true;
}

void Matrix::setSize(unsigned int rows,unsigned int cols){
    if(this->twoDArray)
        for(size_t i=0;i<this->rows;i++)
            delete[] this->twoDArray[i];
    this->rows=rows;
    this->columns=cols;
    this->twoDArray=new double*[rows];
    for(size_t i=0;i<rows;i++){
        this->twoDArray[i]=new double[cols];
        for(size_t j =0 ; j<cols;j++)
            twoDArray[i][j]=0.0;
    }
    this->notMatrix=false;
}

void Matrix::setElement(unsigned int i, unsigned int j, double value){
    this->twoDArray[i][j]=value;
}

//Getters:
string Matrix::getName(){
    return this->name;
}

int Matrix::getRows(){
    return this->rows;
}

int Matrix::getColumns(){
    return this->columns;
}

double Matrix::getNum(){
    return this->num;
}

double Matrix::getElement(unsigned int i,unsigned int j){
    return this->twoDArray[i][j];
}

bool Matrix::is_square(){
    if (this->rows==this->columns) return true;
    else return false;
}

bool Matrix::is_scalar(){
    return this->notMatrix;
}

/********************OPERATIONS******************************/

Matrix* Matrix::add(Matrix& A,Matrix& B){
    Matrix* temp = new Matrix;

    if(A.notMatrix&&B.notMatrix){
        temp->setNum(A.num + B.num);
        return temp;
    }
    else if(!A.notMatrix && !B.notMatrix){
        if(A.rows!=B.rows || A.columns!=B.columns)throw("Dimensions Errors (Addition).");
        temp->setSize(A.rows,A.columns);
        for(size_t i=0;i< temp->rows ;i++)
            for(size_t j=0;j< temp->columns ;j++)
                temp->twoDArray[i][j]=A.twoDArray[i][j]+B.twoDArray[i][j];
        return temp;
    }
    else throw("Operation Error: try '.+'");
}

Matrix* Matrix::pseudoAdd(Matrix& A, Matrix& B){
    if(A.notMatrix && !B.notMatrix){
        Matrix* temp = new Matrix;
        temp->setSize(B.rows,B.columns);
        for(size_t i=0;i<temp->rows;i++)
            for(size_t j=0;j<temp->columns;j++)
                temp->twoDArray[i][j]=A.num+B.twoDArray[i][j];
        return temp;
    }
    else if(!A.notMatrix && B.notMatrix){
        Matrix* temp = new Matrix;
        temp->setSize(A.rows,A.columns);
        for(size_t i=0;i<temp->rows;i++)
            for(size_t j=0;j<temp->columns;j++)
                temp->twoDArray[i][j]=A.twoDArray[i][j]+B.num;
        return temp;
    }
    else throw ("Operator Error: try '+'");
}

Matrix* Matrix::subtract(Matrix& A,Matrix& B){
    if(A.notMatrix&&B.notMatrix){
        Matrix* temp = new Matrix;
        temp->setNum(A.num - B.num);
        return temp;
    }
    else if(!A.notMatrix && !B.notMatrix){
        Matrix* temp = new Matrix;
        if(A.rows!=B.rows || A.columns!=B.columns)throw("Dimensions Errors (Subtraction).");
        temp->setSize(A.rows,A.columns);
        for(size_t i=0;i< temp->rows ;i++)
            for(size_t j=0;j< temp->columns ;j++)
                temp->twoDArray[i][j]=A.twoDArray[i][j]-B.twoDArray[i][j];
        return temp;
    }
    else throw ("Operator Error: try '.-'");
}

Matrix* Matrix::pseudoSubtract(Matrix& A, Matrix& B){
    if(A.notMatrix && !B.notMatrix){
        Matrix* temp = new Matrix;
        temp->setSize(B.rows,B.columns);
        for(size_t i=0;i<temp->rows;i++)
            for(size_t j=0;j<temp->columns;j++)
                temp->twoDArray[i][j]=A.num-B.twoDArray[i][j];
        return temp;
    }
    else if(!A.notMatrix && B.notMatrix){
        Matrix* temp = new Matrix;
        temp->setSize(A.rows,A.columns);
        for(size_t i=0;i<temp->rows;i++)
            for(size_t j=0;j<temp->columns;j++)
                temp->twoDArray[i][j]=A.twoDArray[i][j]-B.num;
        return temp;
    }
    else throw ("Operator Error: try '-'");
}

Matrix* Matrix::product(Matrix& A,Matrix& B){
    if(A.notMatrix && B.notMatrix){     //scalar * scalar
		Matrix* temp = new Matrix;
		temp->setNum(A.num * B.num);
		return temp;
	}
    else if(!A.notMatrix && !B.notMatrix){
        if(A.columns!=B.rows)throw("Dimensions Error (Multiplication).");
        Matrix* temp = new Matrix;
        temp->setSize(A.rows,B.columns);
        for(size_t i = 0;i<A.rows;i++)
            for(size_t j = 0;j<B.rows;j++)
                for(size_t k=0;k<A.columns;k++)  //or B.rows
                    temp->twoDArray[i][j] += A.twoDArray[i][k] * B.twoDArray[k][j];
        return temp;
    }
    else throw ("Operator Error: try '.*'");
}

Matrix* Matrix::pseudoProduct(Matrix& A,Matrix& B){
    if(A.notMatrix && !B.notMatrix){  //scalar .* matrix
        Matrix* temp = new Matrix;
        temp->setSize(B.rows,B.columns);
		for(size_t i=0;i<(temp->rows);i++)
			for(size_t j=0;j<(temp->columns);j++)
				temp->twoDArray[i][j]= A.num * B.twoDArray[i][j];
        return temp;
	}
	else if(!A.notMatrix && B.notMatrix){  //matrix .* scalar
		Matrix* temp = new Matrix;
		temp->setSize(A.rows,A.columns);
		for(size_t i=0;i<(temp->rows);i++)
			for(size_t j=0;j<(temp->columns);j++)
				temp->twoDArray[i][j]= A.twoDArray[i][j] * B.num;
        return temp;
	}
	else if(!A.notMatrix && !B.notMatrix){				//matrix .* matrix
		if(A.rows != B.rows || A.columns != B.columns) throw("Dimensions Error.");
        Matrix* temp = new Matrix;
        temp->setSize(A.rows,A.columns);
        for(size_t i=0;i<(temp->rows);i++)
            for(size_t j=0;j<(temp->columns);j++)
                temp->twoDArray[i][j]= A.twoDArray[i][j] * B.twoDArray[i][j];

		return temp;
	}
	else throw("Operator Error: try '*'");
}

Matrix* Matrix::divide(Matrix& A,Matrix& B){
    if(A.notMatrix && B.notMatrix){
        Matrix* temp = new Matrix;
        if(B.num == 0.0) throw("nooo");
        temp->setNum( A.num / B.num);
        return temp;
    }
    else if(!A.notMatrix && !B.notMatrix){
        if(A.columns!=B.rows || !B.is_square())throw("Dimensions Error (Division).");
        Matrix* temp = new Matrix;
        temp->setSize(A.rows,B.columns);
        temp = product(A,*(B.inverse()));
        return temp;
    }
    else throw("Operator Error: try './'");
}

Matrix* Matrix::pseudoDiv(Matrix& A,Matrix& B){
	if(A.notMatrix && !B.notMatrix){  //scalar ./ matrix
		Matrix* temp = new Matrix;
		temp->setSize(B.rows,B.columns);
		for(size_t i=0;i< temp->rows ;i++)
			for(size_t j=0;j< temp->columns ;j++){
				if(B.twoDArray[i][j] == 0.0) {
                    delete temp;
                    throw("nooo");
				}
				temp->twoDArray[i][j]= A.num / B.twoDArray[i][j];
			}
        return temp;
	}
	else if(!A.notMatrix && B.notMatrix){  //matrix ./ scalar
		if(B.num == 0.0) throw("nooo");
		Matrix* temp = new Matrix;
		temp->setSize(A.rows,A.columns);
		for(size_t i=0; i<temp->rows ;i++)
			for(size_t j=0;j<(temp->columns);j++)
				temp->twoDArray[i][j]= A.twoDArray[i][j] / B.num;
        return temp;
	}
	else if(!A.notMatrix && !B.notMatrix){				//matrix ./ matrix
		if(A.rows != B.rows || A.columns != B.columns) throw("Dimensions Error.");
        Matrix* temp = new Matrix;
		temp->setSize(A.rows,A.columns);
        for(size_t i=0;i<(temp->rows);i++)
            for(size_t j=0;j<(temp->columns);j++){
                if(B.twoDArray[i][j]==0.0){
                    delete temp;
                    throw("nooo");
                }
                temp->twoDArray[i][j]= A.twoDArray[i][j] / B.twoDArray[i][j];
            }
        return temp;
    }
    else throw ("Operator Error: try '/'");
}


Matrix* Matrix::negative(){
    Matrix* temp = new Matrix;
    if(this->notMatrix){
        temp->setNum(0.0 - this->num);
        return temp;
    }
    temp->setSize(this->rows,this->columns);
    for (size_t i = 0 ;i<temp->rows;i++)
        for(size_t j = 0; j<temp->columns;j++)
            temp->twoDArray[i][j] = (0.0 - this->twoDArray[i][j]);
    return temp;
}

Matrix* Matrix::inverse(){
    if(!this->is_square())throw("Dimensions Error (Inverse).");

    double det = this->determinant();
    if (det==0)throw("Matrix has no inverse.");
    det = 1.0 / det;

    Matrix* temp = new Matrix;
    temp->setSize(this->rows,this->columns);
    Matrix* temp2 = new Matrix;
	temp2->notMatrix=true;
	temp2->num = det;
	if(temp->rows==1){
        delete temp2;      //no need in this case
        temp->twoDArray[0][0]=det;
		return temp;
    }
	else if(temp->rows==2){
        for(size_t i=0;i<temp->rows;i++)
            for(size_t j=0;j<temp->columns;j++){
                if(i==j) temp->twoDArray[i][j]=this->twoDArray[!i][!j];
                else temp->twoDArray[i][j]=(0 - this->twoDArray[i][j]);
            }
        temp=pseudoProduct(*temp,*temp2);
        delete temp2;
        return temp;
    }
    else{
        temp = this->adjoint();
        temp = pseudoProduct(*temp,*temp2);
        delete temp2;
        return temp;
    }
}

Matrix* Matrix::transpose(){
    Matrix* temp = new Matrix;
    temp->setSize((this->columns),(this->rows));
    for(size_t i=0;i<this->columns;i++)
        for(size_t j=0;j<this->rows;j++)
            temp->twoDArray[i][j]=this->twoDArray[j][i];
        return temp;
}

Matrix* Matrix::adjoint(){
    Matrix* temp = new Matrix;
    temp->setSize(this->rows,this->columns);
    temp = this->cofactor();
    temp = temp->transpose();
    return temp;
}

Matrix* Matrix::cofactor(){     //el plus wel minus hattwaza3 3ala el elements
    Matrix* temp = new Matrix;
    temp->setSize(this->rows,this->columns);
    for(size_t i=0;i<this->rows;i++)
        for(size_t j=0;j<this->columns;j++)
            temp->twoDArray[i][j]= pow(-1,i+j) * (this->determinant(true,i,j));
    return temp;
}

void print(double** a,int n)
{
	for(int i =0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			cout<<a[i][j]<<"	";
		}
		cout<<endl;
	}
	cout<<"================================"<<endl;
}

double Matrix::determinant(bool minor , unsigned posRow,unsigned posCol){
    //di halet el determinant el tabee3y elly ana bas 3ayz keyamo
    if(!this->is_square())throw("Dimensions Error (Determinant).");
	if(!minor){
        if(this->columns == 1)return this->twoDArray[0][0];
        else if(this->columns == 2)
                return ((this->twoDArray[0][0])*(this->twoDArray[1][1]) - (this->twoDArray[1][0])*(this->twoDArray[0][1]));
        else{   //Determinant

				double det =1;
			unsigned int n = this->rows;
			double**a = new double*[n];
			for(size_t i =0;i<n;i++)
				a[i] = new double[n];

			//filling a[][]

			for(size_t i =0;i<n;i++)
			{
				for(size_t j=0;j<n;j++)
				{
					a[i][j] = this->twoDArray[i][j];
				}

			}


			//check if tri of zeros
			int zero_tri_flag=0;

				for(size_t i=0;i<n;i++)
				{
					for(size_t j=0;j<i;j++)
					{
						if(a[i][j]!=0 && a[i][j]!=-0)
						{
							zero_tri_flag = 1;
							break;

						}
					}
					if(zero_tri_flag == 1)break;
				}

				if(zero_tri_flag == 0)
				{
					for(size_t i =0;i<n;i++)
					{
						det = det * a[i][i];
					}
					return det;
				}

				else
				{
					int col_piv_flag =0;
					zero_tri_flag = 0;

					
					for(size_t i=0;i<n-1;i++)
					{
						//largest pivot
						double largest_pivot =a[i][i];
						int index_of_largest_pivot =i;
						for(int o =i+1;o<n;o++)
						{
							if(fabs(a[o][i])>fabs(largest_pivot))
							{
								largest_pivot = a[o][i];
								index_of_largest_pivot = o;
							}
						}
						if(index_of_largest_pivot!=i)
						{
						for(size_t j=0;j<n;j++)
							{
								double temp;
								temp = a[index_of_largest_pivot][j];
								a[index_of_largest_pivot][j]=a[i][j];
								a[i][j] = temp;
							}
						det = det*-1;

						}
						
						int x;
						int zero_tri_flag1=0;
							for(size_t x =0;x<n;x++)
							{
								for(size_t y =0;y<x;y++)
								{
									if(a[x][y]!=0 && a[x][y]!=-0)
									{
										zero_tri_flag1 = 1;
										break;
									}
								}
								if(zero_tri_flag1==1)break;
							}
							if(zero_tri_flag1==0)
							{
								for(size_t j =0;j<n;j++)
								{
									det = det * a[j][j];
								}
								return det;
							}
							zero_tri_flag1 = 0;


						int in_while_flag=0;
						if(a[i][i]==0||a[i][i]==-0)
						{
							in_while_flag = 1;
							//swap with the lower row

							for(size_t f= i+1;f<n;f++)
							{
								if(a[f][i]!=0 && a[f][i]!=-0)
								{

									for(size_t j=0;j<n;j++)
									{
										double temp;
										temp = a[f][j];
										a[f][j]=a[i][j];
										a[i][j] = temp;
									}

									col_piv_flag=1;
									break;
								}

							}

							det = det *-1;
							
							int zero_tri_flag1=0;
							for(size_t x =0;x<n;x++)
							{
								for(size_t y =0;y<x;y++)
								{
									if(a[x][y]!=0 && a[x][y]!=-0)
									{
										zero_tri_flag1 = 1;
										break;
									}
								}
								if(zero_tri_flag1==1)break;
							}
							if(zero_tri_flag1==0)
							{
								for(size_t j =0;j<n;j++)
								{
									det = det * a[j][j];
								}
								return det;
							}
							zero_tri_flag1 = 0;
						}



						if(col_piv_flag ==0 && in_while_flag==1)
						{
							in_while_flag=0;
							col_piv_flag=1;
							det =0;
							return det;
						}
						col_piv_flag=0;
						in_while_flag=0;


						//making pivot =1
						/*det = det * a[i][i];
						double temp2=a[i][i];
						for(size_t j =i;j<n;j++)
						{
							a[i][j] = a[i][j]/temp2;
						}*/
//amsek kol el rows ely t7t el i  w adrab el row bta3 el pivot fel elemnt ely ta7t el pivot a[i+1][i] w aminuso meno
						for(size_t k=i+1;k<n;k++)
						{
							double temp = a[k][i]/a[i][i];
							
							for(size_t j =i;j<n;j++)
							{
								a[k][j]=a[k][j] - temp * a[i][j];
								//if(fabs(a[k][i])<(pow(10,-15)))a[k][i]=0;
								a[k][i]=0;
							}
						}
						
						//since change in matrix therefore check for zero tri

					    int zero_tri_flag2=0;

						for(size_t x =0;x<n;x++)
							{
								for(size_t y =0;y<x;y++)
								{
									if(a[x][y]!=0 && a[x][y]!=-0)
									{
										zero_tri_flag2 = 1;
										break;
									}
								}
								if(zero_tri_flag2==1)break;
							}
							if(zero_tri_flag2==0)
							{
								for(size_t j =0;j<n;j++)
								{
									det = det * a[j][j];
								}
								return det;
							}

							zero_tri_flag2 =0;
					}
				}
			}
    }
    //di halet el determinant bta3 el submatrix 3ashan akamel beh el inverse
    else{
        Matrix* temp = new Matrix;
        temp->setSize((this->rows)-1,(this->columns)-1);

		for(size_t i=0;i<temp->rows;i++)
			for(size_t j=0;j<temp->columns;j++)
			{

				size_t x = i<posRow? i:i+1;
				size_t y = j<posCol?j:j+1;
				//cout<<posRow<<"  "<<posRow<<"  "<<x<<"  "<<y<<endl;
				temp->twoDArray[i][j]=this->twoDArray[x][y];
			}
	/*
        size_t subRow=0;
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

void Matrix::eye(unsigned int rows/*=0*/, unsigned int columns/*=0*/){
    if(rows == 0 && columns == 0){
        this->num = 1.0;
        return;
    }
    else if(rows==0 || columns==0) throw("Dimension error (eye).");
    this->setSize(rows,columns);
    for(size_t i =0 ;i<this->rows;i++){
        for(size_t j=0;j<this->columns;j++){
            this->twoDArray[i][j] = (i==j)? 1 : 0;
        }
    }
}

/*********************START SUBMATRIX************************/
Matrix* Matrix::subMatrix(string boundaries){
    if(this->notMatrix)throw("Cannot use this function on a scalar value.");
    if(boundaries.find(",")==boundaries.npos)throw("Invalid format for submatrix");

    Matrix* temp = new Matrix;

    bool rowisrange = false;
    bool colisrange = false;

    string rowPart,colPart;
    unsigned int rows[2],cols[2];  //if range
    unsigned int row,col;          //if one element
    rowPart = boundaries.substr(0,boundaries.find(","));
    colPart = boundaries.substr(boundaries.find(",")+1);

    if(rowPart.find(":")!=rowPart.npos){
        rowisrange = true;
        if(rowPart==":"){  // ':' fadya (all rows)
            rows[0] = 1;
            rows[1] = this->rows;
        }
        else if(rowPart.find(":")==0 && rowPart.find(":")!=rowPart.length()-1){  // ':N' men el bdaya le N
            string endRange = rowPart.substr(rowPart.find(":")+1);
            rows[0] = 1;
            char* e;
            rows[1] = strtod(endRange.c_str(),&e);
            if(*e!='\0' || rows[1]>this->rows || rows[1]<=rows[0]) throw("Invalid dimensions.");
        }
        else if(rowPart.find(":")!=0 && rowPart.find(":")==rowPart.length()-1){  // 'M:' men M bdaya lel akher
            string startRange = rowPart.substr(0,rowPart.find(":"));
            rows[1] = this->rows;
            char* e;
            rows[0] = strtod(startRange.c_str(),&e);
            if(*e!='\0' || rows[0]<1 || rows[0]>=rows[1]) throw("Invalid dimensions.");
        }
        else if(rowPart.find(":")!=0 && rowPart.find(":")!=rowPart.length()-1){  //'M:N' men M le N
            string startRange = rowPart.substr(0,rowPart.find(":"));
            string endRange = rowPart.substr(rowPart.find(":")+1);
            char *e1,*e2;
            rows[0] = strtod(startRange.c_str(),&e1);
            rows[1] = strtod(endRange.c_str(),&e2);
            if(*e1!='\0' || *e2!='\0' || rows[0] < 1 || rows[1]>this->rows || rows[0]>=rows[1]) throw("Invalid dimensions.");
        }
    }
    else{
        rowisrange=false;
        char* e;
        row = strtod(rowPart.c_str(),&e);
        if(*e!='\0' || row<1 || row>this->rows) throw("Invalid dimensions.");
    }

    if(colPart.find(":")!=colPart.npos){
        colisrange = true;
        if(colPart==":"){          // ':' fadya (all columns)
            cols[0] = 1;
            cols[1] = this->columns;
        }
        else if(colPart.find(":")==0 && colPart.find(":")!=colPart.length()-1){  // ':N' men el bdaya le N
            string endRange = colPart.substr(colPart.find(":")+1);
            cols[0] = 1;
            char* e;
            cols[1] = strtod(endRange.c_str(),&e);
            if(*e!='\0' || cols[1]>this->columns || cols[1]<=cols[0]) throw("Invalid dimensions.");
        }
        else if(colPart.find(":")!=0 && colPart.find(":")==colPart.length()-1){  // 'M:' men M bdaya lel akher
            string startRange = colPart.substr(0,colPart.find(":"));
            cols[1] = this->columns;
            char* e;
            cols[0] = strtod(startRange.c_str(),&e);
            if(*e!='\0' || cols[0]<1 || cols[0]>=cols[1]) throw("Invalid dimensions.");
        }
        else if(colPart.find(":")!=0 && colPart.find(":")!=colPart.length()-1){  //'M:N' men M le N
            string startRange = colPart.substr(0,colPart.find(":"));
            string endRange = colPart.substr(colPart.find(":")+1);
            char *e1,*e2;
            cols[0] = strtod(startRange.c_str(),&e1);
            cols[1] = strtod(endRange.c_str(),&e2);
            if(*e1!='\0' || *e2!='\0' || cols[0] < 1 ||cols[1]>this->columns || cols[0]>=cols[1]) throw("Invalid dimensions.");
        }
    }
    else{
        colisrange = false;
        char* e;
        col = strtod(colPart.c_str(),&e);
        if(*e!='\0' || col<1 || col>this->columns) throw("Invalid dimensions.");
    }
    if(rowisrange && colisrange){
        temp->setSize(rows[1]-rows[0]+1,cols[1]-cols[0]+1);
        int tempCol = cols[0];
        for(size_t i = 0; i<temp->rows;i++){
            for(size_t j = 0;j<temp->columns;j++){
                temp->twoDArray[i][j] = this->twoDArray[rows[0]-1][tempCol-1];
                tempCol++;
            }
            rows[0]++;
            tempCol = cols[0];
        }
    }
    else if(rowisrange && !colisrange){
        temp->setSize(rows[1]-rows[0]+1 , 1);
        for(size_t i = 0; i<temp->rows;i++){
            for(size_t j = 0;j<temp->columns;j++){
                temp->twoDArray[i][j] = this->twoDArray[rows[0]-1][col-1];
            }
            rows[0]++;
        }
    }
    else if(!rowisrange && colisrange){
        temp->setSize(1 , cols[1]-cols[0]+1);
        unsigned int tempCol = cols[0];
        for(size_t i = 0; i<temp->rows;i++){
            for(size_t j = 0;j<temp->columns;j++){
                temp->twoDArray[i][j] = this->twoDArray[row-1][tempCol-1];
                tempCol++;
            }
            tempCol = cols[0];
        }
    }
    else {
        temp->notMatrix=true;
        temp->rows = 0, temp->columns=0;   //egra2 rotiny 3ashan kol el scalar values yb2o shabah ba3d :D
        double value = this->twoDArray[row-1][col-1];
        temp->num = value;
    }
    return temp;
}
/***********************END SUBMATRIX************************/

//Matrix* Matrix::concatenate(Matrix& A,Matrix& B){
//    if(A.rows != A.columns)throw("Cannot concatenate matrices with different heights.");
//
//}

std::string Converttostring (float number){
    std::ostringstream buff;
    buff<<fixed<<setprecision(2)<<number;
    return buff.str();
}
void Matrix::printMatrix(bool name/*=true*/,unsigned int num_equal/*=0*/, const vector<string>&arrayOfNames){

	if(name == false){
        if(this->notMatrix){
            cout<<"     "<<this->num<<endl;
        }
        else{
            cout<<endl;
            unsigned int longestsize=0;
            //********calculating length of longest element in the displayed matrix*********
            for(size_t i =0;i<this->rows;i++)
                for(size_t j=0;j<this->columns;j++){
                    char temp[20];
                    sprintf(temp,"%g",this->twoDArray[i][j]);
                    if(strlen(temp)>longestsize)longestsize = strlen(temp);
                }
            //***********displaying**************
            for(size_t i =0;i<this->rows;i++){
                cout<<"     ";
                for(size_t j=0;j<this->columns;j++){

                    //calculating length of the displayed element
                    char temp[20];
                    sprintf(temp,"%g",this->twoDArray[i][j]);
                    unsigned int outputLen = strlen(temp);
                   printf("%g",this->twoDArray[i][j]);

                    // printing no. of spaces equal to the size of the longest matrix - size of displayed matrix + 3 as a tolerance
                    for(size_t k =0; k<longestsize-outputLen+3; k++){
                        cout<<" ";
                    }
                }
                cout<<endl;
            }
        }
	}
	else{  //name = true
	    cout<<"     ";
            for(size_t i = 0; i<num_equal;i++)
                cout<<arrayOfNames[i]<<" = ";
	    if(this->notMatrix){
            cout<<this->num<<endl;
	    }
	    else{
            cout<<endl;
            unsigned int longestsize=0;
            //********calculating length of longest element in the displayed matrix*********
            for(size_t i =0;i<this->rows;i++)
                for(size_t j=0;j<this->columns;j++){
                    char temp[20];
                    sprintf(temp,"%g",this->twoDArray[i][j]);
                    if(strlen(temp)>longestsize)longestsize = strlen(temp);
                }
            //***********displaying**************
            for(size_t i =0;i<this->rows;i++){
                cout<<"     ";
                for(size_t j=0;j<this->columns;j++){

                    //calculating length of the displayed element
                    char temp[20];
                    sprintf(temp,"%g",this->twoDArray[i][j]);
                    unsigned int outputLen = strlen(temp);

                    printf("%g",this->twoDArray[i][j]);

                    // printing no. of spaces equal to the size of the longest matrix - size of displayed matrix + 3 as a tolerance
                    for(size_t k =0; k<longestsize-outputLen+3; k++){
                        cout<<" ";
                    }
                }
                cout<<endl;
            }
	    }
	}
}
