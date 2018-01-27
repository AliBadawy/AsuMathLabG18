#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include "Matrix.h"
#include<stdio.h>



/*******OS DETECTION**********/
#ifdef __linux__
#define linux 1
#else
#define linux 0
#endif // __linux__
/*****END OS DETECTION*********/

#define alphabets "abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define numbers "-.0123456789"
static const int PI = 3.14;

using namespace std;

void inputHandling(string, vector<Matrix>&, vector<string>&);
Matrix multiOpHandling(string, vector<Matrix>&, vector<string>&);


struct operators
{
	int pos;
	string op;
};
Matrix multiOpHandling(string RHS, vector<Matrix>& storedMatrices, vector<string>& systemCommands);
int main(int argv, char* argc[])
{
	/** clearing the inteface of console **/
	if (linux)
		system("clear");
	else {
		system("cls");
		system("color 70");
	}
	/******** end clearing **********/
	vector<Matrix> storedMatrices(0);
	/** setting the forbidden variable names (matlab default commands) **/
	vector<string> systemCommands(0);
	systemCommands.push_back("quit");
	systemCommands.push_back("QUIT");
	systemCommands.push_back("Quit");
	systemCommands.push_back("clc");
	systemCommands.push_back("CLC");
	systemCommands.push_back("Clc");
	systemCommands.push_back("who");
	systemCommands.push_back("clear");
	systemCommands.push_back("CLEAR");
	systemCommands.push_back("Clear");
	systemCommands.push_back("det");
	systemCommands.push_back("inv");

	string commandLine = "";

	/*** FILE MODE ***/
	if (argv > 1) {
		ifstream file;
		file.open(argc[1]);
		string fileLine = "";
		string temp = "";

		int openBrackets = 0, closeBrackets = 0;
		if (!file.bad() && !file.eof()) {
			while (getline(file, fileLine)) {
				/** DETECTING MULTIPLE LINE LARGE COMMANDS **/
				for (size_t i = 0; i<fileLine.length(); i++) {
					if (fileLine[i] == '[')openBrackets++;
					else if (fileLine[i] == ']')closeBrackets++;
				}

				while (fileLine.find("\r") != fileLine.npos)fileLine.erase(fileLine.find("\r"), 1);
				while (fileLine.find("\n") != fileLine.npos)fileLine.erase(fileLine.find("\n"), 1);

				if (openBrackets>closeBrackets) {  //command haven't ended yet
					temp += fileLine;
					continue;
				}
				else {
					openBrackets = 0, closeBrackets = 0;
					commandLine = temp + fileLine;
					temp = "";
				}
				/** END DETECTING **/
				try {
					if (commandLine == "who") {
						if (storedMatrices.size() == 0) throw("No stored variables! Memory is empty.");
						else {
							cout << "Your variables are:\n";
							for (size_t i = 0; i<storedMatrices.size() - 1; i++) {
								cout << storedMatrices[i].getName();
								if (storedMatrices[i].is_scalar()) cout << ":scalar    -    ";
								else cout << ":matrix[" << storedMatrices[i].getRows() << "x" << storedMatrices[i].getColumns() << "]    -    ";
							}
							cout << storedMatrices[storedMatrices.size() - 1].getName();
							if (storedMatrices[storedMatrices.size() - 1].is_scalar()) cout << ":scalar";
							else cout << ":matrix[" << storedMatrices[storedMatrices.size() - 1].getRows() << "x" << storedMatrices[storedMatrices.size() - 1].getColumns() << "]";
							cout << endl;
							continue;
						}
					}
					else if (commandLine == "clear" || commandLine == "CLEAR" || commandLine == "Clear") {
						if (storedMatrices.size() == 0) throw("Memory is already empty.");
						else {
							storedMatrices.clear();
							cout << "Memory cleared!\n\n";
							continue;
						}
					}
					else {
						if (commandLine.find_first_not_of(" \n\r") != commandLine.npos) {
							inputHandling(commandLine, storedMatrices, systemCommands);
							cout << endl;
						}
						else {
							continue;
						}
					}
				}
				catch (const char* err) {
					cerr << err << endl;
				}
			}
		}
		else {
			cerr << "Couldn't open file: " << argc[1] << endl;
		}
		file.close();
		if (storedMatrices.size() == 0);
		else storedMatrices.clear();
		systemCommands.clear();
		exit(0);
	}

	/*** CONSOLE MODE ***/
	else {
		while (true) {
			try {
				cout << ">> ";
				getline(cin, commandLine);
				/** extracting default matlab commands **/
				if (commandLine == "quit" || commandLine == "QUIT" || commandLine == "Quit") {
					if (storedMatrices.size() == 0);
					else storedMatrices.clear();
					systemCommands.clear();
					exit(0);
				}
				else if (commandLine == "clc" || commandLine == "CLC" || commandLine == "Clc") {
					if (linux) system("clear");
					else system("cls");
					continue;
				}
				else if (commandLine == "who") {
					if (storedMatrices.size() == 0) throw("No stored variables! Memory is empty.");
					else {
						cout << "Your variables are:\n";
						for (size_t i = 0; i<storedMatrices.size() - 1; i++) {
							cout << storedMatrices[i].getName();
							if (storedMatrices[i].is_scalar()) cout << ":scalar    -    ";
							else cout << ":matrix[" << storedMatrices[i].getRows() << "x" << storedMatrices[i].getColumns() << "]    -    ";
						}
						cout << storedMatrices[storedMatrices.size() - 1].getName();
						if (storedMatrices[storedMatrices.size() - 1].is_scalar()) cout << ":scalar";
						else cout << ":matrix[" << storedMatrices[storedMatrices.size() - 1].getRows() << "x" << storedMatrices[storedMatrices.size() - 1].getColumns() << "]";
						cout << "\n\n";
						continue;
					}
				}
				else if (commandLine == "clear" || commandLine == "CLEAR" || commandLine == "Clear") {
					if (storedMatrices.size() == 0) throw("Memory is already empty.");
					else {
						storedMatrices.clear();
						cout << "Memory cleared!\n\n";
						continue;
					}
				}
				else {
					if (commandLine.find_first_not_of(" \n\r") != commandLine.npos) {
						inputHandling(commandLine, storedMatrices, systemCommands);
						cout << endl;
					}
					else continue;
				}
			}
			catch (const char* err) {
				cerr << err << "\n\n";
			}
		}
	}
}










void inputHandling(string input, vector<Matrix>& storedMatrices, vector<string>& systemCommands) { //the main function

	while (input.find("\n") != input.npos)input.erase(input.find("\n"), 1);
	while (input.find("\r") != input.npos)input.erase(input.find("\r"), 1);

	/*********** handling A = B = [......] ************/
	size_t pos_last_equal = input.rfind("="), pos_first_equal = input.find("=");
	string before_equal;
	string RHS;
	unsigned int num_equals = 0;
	vector<string> arrayOfLHS(0);
	if (pos_last_equal != input.npos) {
		RHS = input.substr(input.rfind("=") + 1);
		before_equal = input.substr(0, pos_last_equal);
		arrayOfLHS.push_back(input.substr(0, pos_first_equal));
		num_equals++;
		while (before_equal.rfind("=") != before_equal.npos) {
			pos_first_equal = before_equal.find("=");
			before_equal = before_equal.erase(0, pos_first_equal + 1);
			if (before_equal.rfind("=") != before_equal.npos) {
				arrayOfLHS.push_back(before_equal.substr(0, before_equal.find("=")));
				num_equals++;
			}
			else {
				arrayOfLHS.push_back(before_equal);
				num_equals++;
				break;
			}
		}
		for (size_t i = 0; i<num_equals; i++)
			while (arrayOfLHS[i].find(" ") != arrayOfLHS[i].npos) arrayOfLHS[i].erase(arrayOfLHS[i].find(" "), 1);

		for (size_t i = 0; i<num_equals; i++)
			for (size_t j = 0; j<systemCommands.size(); j++)
				if (arrayOfLHS[i] == systemCommands[j]) throw("Invalid name.");
	}
	else {
		RHS = input;
		arrayOfLHS.push_back("ans");
		num_equals++;
	}
	/*********END HANDLING******************/

	/********Check existence of LHS*********/
	int* arrayOfIndeces = new int[num_equals];
	for (size_t i = 0; i<num_equals; i++) {
		arrayOfIndeces[i] = -1;
		for (size_t j = 0; j<storedMatrices.size(); j++)
			if (arrayOfLHS[i] == storedMatrices[j].getName()) {
				arrayOfIndeces[i] = j;
				break;
			}
	}

	/********End check ****************/

	while (RHS.find(" ") != RHS.npos) RHS.erase(RHS.find(" "), 1);

	bool print = true;
	//cout<<"before op"<<endl;
	/********** START OPERATION  ***********/

	/*****************END OPERATIONS********************************/




	/*************START STRING INITIALIZING ******************/

	if (input.find("[") != input.npos) {
		if (input.find("]") != input.npos) {
			if (input.find("]") != input.length() - 1) {
				string afterMatrix = input.substr(input.find("]") + 1);
				if (afterMatrix.find_first_not_of(" ;") == afterMatrix.npos)print = false;
				input.erase(input.rfind("]") + 1, afterMatrix.length());
			}
		}
		else throw ("Close the brackets.");

		/* storing */ //**********************************************************************************Adel edited here**************
		int case2=1;
		//cout<<input<<"----------"<<endl;
		string tempIn = input.substr(input.find("["));
		//cout<<input<<endl;
		for(int y=1;y<tempIn.length();y++)
		{
			string alphabetsString = "abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			string operationsString = "+-*/.^'()";
			if(tempIn[y]=='['|| alphabetsString.find(tempIn[y])!=std::string::npos || operationsString.find(tempIn[y])!=std::string::npos)
			{
				case2=1;
			}
			
		}
		/*Matrix inputAssignment;
		if(!case2)
			inputAssignment = Matrix(arrayOfLHS[0], input);
		else 
		{
			inputAssignment = Matrix(arrayOfLHS[0], input,storedMatrices);
		//cout<<"returend from constructor"<<endl;

		}*/
		Matrix inputAssignment (arrayOfLHS[0], input,storedMatrices);
		
		if (arrayOfIndeces[0] == -1) {
			storedMatrices.push_back(inputAssignment);
		}
		else {
			storedMatrices[arrayOfIndeces[0]] = inputAssignment;
		}

		for (size_t i = 1; i<num_equals; i++) {
			if (arrayOfIndeces[i] == -1) {
				inputAssignment.setName(arrayOfLHS[i]);
				storedMatrices.push_back(inputAssignment);
			}
			else {
				inputAssignment.setName(arrayOfLHS[i]);
				storedMatrices[arrayOfIndeces[i]] = inputAssignment;
			}
		}
		if (print) inputAssignment.printMatrix(true, num_equals, arrayOfLHS);
		delete arrayOfIndeces;
	}


	else if (RHS.find("+") != RHS.npos || RHS.find("-") != RHS.npos && (RHS.find("-")>0 || (RHS.find("-") == 0 && RHS.find_first_not_of(numbers) != RHS.npos) || (RHS.find("-") == 0 && RHS.find_first_of(alphabets) == RHS.npos && RHS.find_last_of("+-/") != RHS.npos && RHS.find_last_of("+-/")>1))
		|| RHS.find("*") != RHS.npos || RHS.find("/") != RHS.npos || RHS.find("inv") != RHS.npos || RHS.find("'") != RHS.npos || RHS.find("`") != RHS.npos || RHS.find("det") != RHS.npos)
	{      
		Matrix* temp = new Matrix;
		*temp=multiOpHandling(RHS, storedMatrices, systemCommands);
		storedMatrices.push_back(*temp);
		for (size_t i = 0; i<num_equals; i++) {
					temp->setName(arrayOfLHS[i]);
					if (arrayOfIndeces[i] == -1)
						storedMatrices.push_back(*temp);
					else {
						storedMatrices[arrayOfIndeces[i]] = *temp;
					}
				}
				if (print) temp->printMatrix(true, num_equals, arrayOfLHS);
				delete temp;
				delete arrayOfIndeces;
				arrayOfLHS.clear();

		//if it was an operation statement
	//		//cout<<"in strat op"<<endl;
	//	if (RHS.find(";") != RHS.npos) {                //ay haga ba3d el semicolon mesh btethseb
	//		RHS.erase(RHS.begin() + RHS.find(";"), RHS.end());
	//		print = false;
	//	}

	//	Matrix* temp = new Matrix;    // di el result bta3 el operation bnInitializo badry 3ashan hanstakhdemo gwa if conditions kter 2odam

	//	if (RHS.find("[") != RHS.npos)throw("Matrix must be declared first.");   //lazem madam operational input ydakhal matrices met3arafa
	//	if (RHS.find("]") != RHS.npos)throw("Invalid input format.");            //kda kda maynfa3sh ye2fel koos abl ma yeftaho fa tabe3y error

	//	while (RHS.find(" ") != RHS.npos) RHS.erase(RHS.find(" "), 1);


	//	/* DETERMINANT */
	//	if (RHS.find("det") != RHS.npos) {
	//		int operandIndex = -1;
	//		if (RHS.find("(") == RHS.npos)throw("Use parentheses after 'det'.");

	//		while (RHS.find("(") != RHS.npos) RHS.erase(RHS.find("("), 1);

	//		if (RHS.find(")") == RHS.npos)throw("Missing ')' after the variable.");

	//		while (RHS.find(")") != RHS.npos) RHS.erase(RHS.find(")"), 1);

	//		if (RHS.find("d") == 0) RHS.erase(RHS.find("d"), 3);
	//		else throw("Invalid input format.");    //law katab ay haga abl kelmt det ba3d el equal (gher el spaces tab3an)

	//		for (size_t i = 0; i< storedMatrices.size(); i++)
	//			if (RHS == storedMatrices[i].getName())
	//				operandIndex = i;
	//		if (operandIndex == -1)throw("Operand not defined.");
	//		else {
	//			if (storedMatrices[operandIndex].is_scalar()) throw("You must enter a matrix.");
	//			else {
	//				*temp = storedMatrices[operandIndex].determinant();
	//			}
	//		}
	//	}
	//	/* INVERSE */
	//	else if (RHS.find("inv") != RHS.npos) {

	//		int operandIndex = -1;
	//		if (RHS.find("(") == RHS.npos)throw("Use parentheses after 'inv'.");
	//		while (RHS.find("(") != RHS.npos)RHS.erase(RHS.find("("), 1);
	//		if (RHS.find(")") == RHS.npos)throw("Missing ')' after the variable.");
	//		while (RHS.find(")") != RHS.npos)RHS.erase(RHS.find(")"), 1);

	//		if (RHS.find("i") == 0) RHS.erase(RHS.find("i"), 3);
	//		else throw("Invalid input format.");    //law katab ay haga abl kelmt inv ba3d el equal (gher el spaces tab3an)

	//		for (size_t i = 0; i< storedMatrices.size(); i++)
	//			if (RHS == storedMatrices[i].getName())
	//				operandIndex = i;
	//		if (operandIndex == -1)throw("Operand not defined.");
	//		else {
	//			if (storedMatrices[operandIndex].is_scalar()) throw("You must enter a matrix.");
	//			else temp = storedMatrices[operandIndex].inverse();
	//		}
	//	}  //end inverse

	//	   /* TRANSPOSE */
	//	else if (RHS.find("`") != RHS.npos || RHS.find("'") != RHS.npos) {

	//		int operandIndex = -1;
	//		if (RHS.find("`") != RHS.npos) RHS.erase(RHS.begin() + RHS.find("`"));
	//		if (RHS.find("'") != RHS.npos) RHS.erase(RHS.begin() + RHS.find("'"));

	//		for (size_t i = 0; i< storedMatrices.size(); i++)
	//			if (RHS == storedMatrices[i].getName())
	//				operandIndex = i;
	//		if (operandIndex == -1)throw("Operand not defined.");
	//		else {
	//			if (storedMatrices[operandIndex].is_scalar()) throw("You must enter a matrix.");
	//			else temp = storedMatrices[operandIndex].transpose();
	//		}
	//	}  //end transpose

	//	   /* MATHEMATICAL OPERATIONS */
	//	else if (RHS.find("*") != RHS.npos || RHS.find("/") != RHS.npos || RHS.find("+") != RHS.npos || RHS.find("-") != RHS.npos) {
	//		//cout<<"here in Mathimatical op"<<endl;
	//		int operandIndeces[2] = { -1,-1 };
	//		string operands[2] = { "" };

	//		/* Check if only negative (A = -B)*/
	//		int i = 0;
	//		char* RHS_buffer = new char[RHS.length() + 1];
	//		strcpy(RHS_buffer, RHS.c_str());
	//		char* token = strtok(RHS_buffer, "-");
	//		while (token) {
	//			operands[i] = token;
	//			i++;
	//			token = strtok(NULL, "-");
	//		}
	//		delete RHS_buffer;
	//		if (i == 1 && RHS[0] == '-'&& RHS.find_last_of("+-*/") == 0) {
	//			RHS.erase(0, 1);
	//			int operandIndex = -1;
	//			for (size_t i = 0; i< storedMatrices.size(); i++)
	//				if (RHS == storedMatrices[i].getName()) {
	//					operandIndex = i;
	//					break;
	//				}
	//			if (operandIndex == -1)throw("Operand not defined.");
	//			else {
	//				Matrix m = -storedMatrices[operandIndex];
	//				*temp = m;
	//			}
	//		}
	//		/* end negative */

	//		/** start two-operand operations **/
	//		else {
	//			//cout<<"here 2"<<endl;
	//			Matrix *parameter1 = new Matrix, *parameter2 = new Matrix;
	//			if (RHS.find(".+") != RHS.npos || RHS.find(".-") != RHS.npos || RHS.find(".*") != RHS.npos || RHS.find("./") != RHS.npos)
	//			{
	//				//cout<<". minus"<<endl;
	//				string op;
	//				for (size_t i = 1; i<RHS.length(); i++)
	//					if (RHS[i] == '.' && (RHS[i + 1] == '*' || RHS[i + 1] == '/' || RHS[i + 1] == '-' || RHS[i + 1] == '+')) {
	//						operands[0] = RHS.substr(0, i);
	//						operands[1] = RHS.substr(i + 2);
	//						op = RHS.substr(i, 2);
	//						break;
	//					}
	//				bool negate1 = false, negate2 = false;
	//				if (operands[0][0] == '-' && operands[0].find_first_not_of(alphabets) == operands[0].npos) {
	//					operands[0].erase(0, 1);
	//					negate1 = true;
	//				}
	//				if (operands[1][0] == '-' && operands[1].find_first_not_of(alphabets) == operands[1].npos) {
	//					operands[1].erase(0, 1);
	//					negate2 = true;
	//				}
	//				for (size_t i = 0; i<storedMatrices.size(); i++) {
	//					if (operands[0] == storedMatrices[i].getName())
	//						operandIndeces[0] = i;
	//					if (operands[1] == storedMatrices[i].getName())
	//						operandIndeces[1] = i;
	//				}
	//				if ((operandIndeces[0] == -1 && operands[0].find_first_not_of(alphabets) == operands[0].npos)
	//					|| (operandIndeces[1] == -1 && operands[1].find_first_not_of(alphabets) == operands[1].npos)) throw("Operand not defined.");


	//				if (operands[0].find_first_not_of(alphabets) != operands[0].npos) {
	//					char *e;
	//					double value = strtod(operands[0].c_str(), &e);
	//					if (*e != 0) throw("Invalid number.");
	//					else *parameter1 = value;
	//				}
	//				else *parameter1 = storedMatrices[operandIndeces[0]];
	//				if (operands[1].find_first_not_of(alphabets) != operands[1].npos) {
	//					char *e;
	//					double value = strtod(operands[1].c_str(), &e);
	//					if (*e != 0) throw("Invalid number.");
	//					else *parameter2 = value;
	//				}
	//				else *parameter2 = storedMatrices[operandIndeces[1]];

	//				if (negate1) parameter1 = parameter1->negative();
	//				if (negate2) parameter2 = parameter2->negative();

	//				if (op == ".+") temp = Matrix::pseudoAdd(*parameter1, *parameter2);
	//				else if (op == ".-") temp = Matrix::pseudoSubtract(*parameter1, *parameter2);
	//				else if (op == ".*") temp = Matrix::pseudoProduct(*parameter1, *parameter2);
	//				else if (op == "./") temp = Matrix::pseudoDiv(*parameter1, *parameter2);

	//				delete parameter1;
	//				delete parameter2;
	//			}
	//			else {   // ( + - / * )
	//				char op;
	//				for (size_t i = 1; i < RHS.length(); i++)
	//					if (RHS[i] == '+' || RHS[i] == '-' || RHS[i] == '*' || RHS[i] == '/') {
	//						//cout<<"minus"<<endl;
	//						operands[0] = RHS.substr(0, i);
	//						operands[1] = RHS.substr(i + 1);
	//						op = RHS[i];
	//						break;
	//					}
	//				bool negate1 = false, negate2 = false;
	//				if (operands[0][0] == '-' && operands[0].find_first_not_of(alphabets) == operands[0].npos) {
	//					operands[0].erase(0, 1);
	//					negate1 = true;
	//				}
	//				if (operands[1][0] == '-' && operands[1].find_first_not_of(alphabets) == operands[1].npos) {
	//					operands[1].erase(0, 1);
	//					negate2 = true;
	//				}


	//				for (size_t i = 0; i<storedMatrices.size(); i++) {
	//					if (operands[0] == storedMatrices[i].getName())
	//						operandIndeces[0] = i;
	//					if (operands[1] == storedMatrices[i].getName())
	//						operandIndeces[1] = i;
	//				}

	//				if ((operandIndeces[0] == -1 && operands[0].find_first_not_of(alphabets) == operands[0].npos)
	//					|| (operandIndeces[1] == -1 && operands[1].find_first_not_of(alphabets) == operands[1].npos)) throw("Operand not defined.");

	//				if (operands[0].find_first_not_of(numbers) == operands[0].npos) {  //if the user used a scalar nonsaved value, ex:(A = 1 ./ B)
	//					char *e;
	//					double value = strtod(operands[0].c_str(), &e);
	//					if (*e != 0) throw("Invalid number.");
	//					else *parameter1 = value;
	//				}
	//				else *parameter1 = storedMatrices[operandIndeces[0]];
	//				if (operands[1].find_first_not_of(numbers) == operands[1].npos) {
	//					char *e;
	//					double value = strtod(operands[1].c_str(), &e);
	//					if (*e != 0) throw("Invalid number.");
	//					else *parameter2 = value;
	//				}
	//				else *parameter2 = storedMatrices[operandIndeces[1]];

	//				if (negate1) parameter1 = parameter1->negative();
	//				if (negate2) parameter2 = parameter2->negative();


	//				if (op == '+') *temp = *parameter1 + *parameter2;
	//				else if (op == '-') *temp = *parameter1 - *parameter2;
	//				else if (op == '*') *temp = *parameter1 * *parameter2;
	//				else if (op == '/') *temp = *parameter1 / *parameter2;
	//				delete parameter1;
	//				delete parameter2;
	//			}
	//		}
	//		/** end mathematical operations **/
	//	}
	//	/** end matrix operations **/

	//	/**********start matrix storing***********/
	//	//        Matrix result = *temp;
	//	for (size_t i = 0; i<num_equals; i++) {
	//		temp->setName(arrayOfLHS[i]);
	//		if (arrayOfIndeces[i] == -1)
	//			storedMatrices.push_back(*temp);
	//		else {
	//			storedMatrices[arrayOfIndeces[i]] = *temp;
	//		}
	//	}
	//	if (print) temp->printMatrix(true, num_equals, arrayOfLHS);
	//	delete temp;
	//	delete arrayOfIndeces;
	//	arrayOfLHS.clear();

	//	/** end storing **/ 
	}

	/**********END STRING INITIALIZTING *************/



	/*********SPECIAL FUNCTIONS**************/  //ones, zeros, eye, ... etc
	else if (RHS.find("eye") != RHS.npos || RHS.find("zeros") != RHS.npos || RHS.find("ones") != RHS.npos || RHS.find("rand") != RHS.npos) {
		Matrix* temp = new Matrix;
		if (RHS.find(";") != RHS.npos) {
			RHS.erase(RHS.begin() + RHS.find(";"), RHS.end());
			print = false;
		}

		//1. EYE:
		if (RHS.find("eye") != RHS.npos) {
			if (RHS.find("(") != RHS.npos) {
				RHS.erase(RHS.begin(), RHS.begin() + RHS.find("(") + 1);
				if (RHS.find(")") != RHS.npos) RHS.erase(RHS.begin() + RHS.find(")"));
				else throw("Close the parentheses after entering the dimensions.");
				//a. non-square eye
				char *e1, *e2;
				if (RHS.find(",") != RHS.npos) {
					int rows = 0, cols = 0;
					string rowString = RHS.substr(0, RHS.find(","));
					string colString = RHS.substr(RHS.find(",") + 1);
					rows = strtod(rowString.c_str(), &e1);
					cols = strtod(colString.c_str(), &e2);
					if (*e1 != 0 || *e2 != 0) throw("Invalid identity matrix dimensions.");
					temp->eye(rows, cols);
				}
				//b. start square eye
				else {
					int dim = 0;
					dim = strtod(RHS.c_str(), &e1);
					if (*e1 != 0) throw("Invalid identity matrix dimensions.");
					temp->eye(dim, dim);
				}
			}
			//c. no dimensions
			else *temp = 1.0;
		}

		//2.ZEROS:


		//3.ONES:



		//Storing:
		for (size_t j = 0; j<num_equals; j++) {
			temp->setName(arrayOfLHS[j]);
			if (arrayOfIndeces[j] == -1) {
				storedMatrices.push_back(*temp);
			}
			else {
				storedMatrices[arrayOfIndeces[j]] = *temp;
			}
		}
		if (print) temp->printMatrix(true, num_equals, arrayOfLHS);
		delete temp;
		delete arrayOfIndeces;
		arrayOfLHS.clear();
	}
	/**********END SPECIAL FUNCTION************/


	/*********START SUBMATRIX AND INDEXING*****/
	else if (RHS.find("(") != RHS.npos) {

		if (RHS.find(";") != RHS.npos) {
			RHS.erase(RHS.begin() + RHS.find(";"), RHS.end());
			print = false;
		}

		string boundaries = "";
		if (RHS.find(")") == RHS.npos)throw ("Close the parentheses.");
		boundaries = RHS.substr(RHS.find("(") + 1);
		while (boundaries.find(")") != boundaries.npos) boundaries.erase(boundaries.find(")"), 1);
		RHS = RHS.substr(0, RHS.find("("));

		int RHS_position = -1;

		for (size_t i = 0; i<storedMatrices.size(); i++) {
			if (RHS == storedMatrices[i].getName())
				RHS_position = i;
		}

		Matrix* temp = new Matrix;

		if (RHS_position == -1)throw("Initialize matrix before indexing.");
		else temp = storedMatrices[RHS_position].subMatrix(boundaries);

		for (size_t i = 0; i<num_equals; i++) {
			temp->setName(arrayOfLHS[i]);
			if (arrayOfIndeces[i] == -1)
				storedMatrices.push_back(*temp);
			else {
				storedMatrices[arrayOfIndeces[i]] = *temp;
			}
		}

		if (print) temp->printMatrix(true, num_equals, arrayOfLHS);
		delete temp;
		delete arrayOfIndeces;
		arrayOfLHS.clear();
	}
	/*************END SUBMATRIX AND INDEXING****************/

	/************ASSIGNMENTS*************/
	else if (input.find("=") != input.npos) {
		if (RHS.find(";") != RHS.npos) {
			print = false;
			RHS.erase(RHS.begin() + RHS.find(";"), RHS.end());
		}

		Matrix* temp = new Matrix;
		if (RHS.find_first_not_of(numbers) == RHS.npos) {        //numbers
			double value = 0;
			char* e;
			value = strtod(RHS.c_str(), &e);
			if (*e != 0) throw("Invalid number!");
			*temp = value;
		}
		else {
			int RHS_index = -1;
			for (size_t i = 0; i <storedMatrices.size(); i++)
				if (RHS == storedMatrices[i].getName()) RHS_index = i;

			if (RHS_index == -1)throw("Variable not found.");
			*temp = storedMatrices[RHS_index];
		}

		for (size_t j = 0; j<num_equals; j++) {
			temp->setName(arrayOfLHS[j]);
			if (arrayOfIndeces[j] == -1) {
				storedMatrices.push_back(*temp);
			}
			else {
				storedMatrices[arrayOfIndeces[j]] = *temp;
			}
		}
		if (print) temp->printMatrix(true, num_equals, arrayOfLHS);
		delete temp;
		delete arrayOfIndeces;
		arrayOfLHS.clear();
	}
	/*********END ASSIGNING****************/

	/******* START PRINTING SAVED MATRICES ********/
	else {
		delete arrayOfIndeces;  //not used .. have the index of the "ans" variable

		for (size_t i = 0; i<storedMatrices.size(); i++) {
			if (RHS == storedMatrices[i].getName()) {
				storedMatrices[i].printMatrix(false);
				return;
			}
		}
		throw("No matrix of this name, assign it first.");
	}
	/*******END PRINTING*****************/
}
/**END INPUT HANDLING**/

//******************************************************************************************************************************

Matrix multiOpHandling(string RHS, vector<Matrix>& storedMatrices, vector<string>& systemCommands)
{

	//getting the operators in the i/p
	vector <operators> ops(0);
	int openedBracketsNo = 0;
	int closedBracketsNo = 0;
	operators currentOp;
	Matrix* temp = new Matrix;

	static string temporaryName = "temp11"; //m3mlthash 00 3shan el 0 e3tbrha ely hya bta3t el termination de wtala3 error

	if (RHS.find("+") == RHS.npos && RHS.find("-") == RHS.npos && RHS.find("*") == RHS.npos && RHS.find("/") == RHS.npos && RHS.find("inv") == RHS.npos && RHS.find("'") == RHS.npos && RHS.find("`") == RHS.npos && RHS.find("det") == RHS.npos && RHS.find('(') == RHS.npos && RHS.find(')') == RHS.npos && RHS.find('[') == RHS.npos && RHS.find(']') == RHS.npos)
	{
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
			else throw("illegal point at RHS");//lw 7abena nzbatha numbers hnshel el error da
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

	//Brackets (including the det and inv brackets)
	while (openedBracketsNo > 0)
	{
		int openBIndex, closeBIndex;
		openBIndex = RHS.rfind('(');
		closeBIndex = RHS.substr(openBIndex).find(')') + openBIndex;
		if (closeBIndex == RHS.npos)throw("Error opened bracket without closure");

		string insideBrackets = RHS.substr(openBIndex + 1, closeBIndex - (openBIndex + 1));
		Matrix temporary;
		temporary = multiOpHandling(insideBrackets, storedMatrices, systemCommands);
		RHS.erase(openBIndex + 1, closeBIndex - (openBIndex + 1));
		RHS.insert(openBIndex + 1, temporary.getName());
		if (temporaryName[5] < '9')temporaryName[5]++;
		else
		{
			temporaryName[5] = 1;
			(temporaryName[4] < '9') ? temporaryName[4]++ : temporaryName[4] = 1;
		}
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

				storedMatrices.push_back(*temp);
				RHS.erase(openBIndex - 4, closeBIndex - (openBIndex - 5));
				openedBracketsNo--; closedBracketsNo--;
				RHS.insert(openBIndex - 4, temporaryName);
				if (temporaryName[5] < '9')temporaryName[5]++;
				else
				{
					temporaryName[5] = 1;
					(temporaryName[4] < '9') ? temporaryName[4]++ : temporaryName[4] = 1;
				}
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
				storedMatrices.push_back(*temp);
				RHS.erase(openBIndex - 3, closeBIndex - (openBIndex - 4));
				openedBracketsNo--; closedBracketsNo--;
				RHS.insert(openBIndex - 3, temporaryName);
				if (temporaryName[5] < '9')temporaryName[5]++;
				else
				{
					temporaryName[5] = 1;
					(temporaryName[4] < '9') ? temporaryName[4]++ : temporaryName[4] = 1;
				}
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

					storedMatrices.push_back(*temp);
					RHS.erase(openBIndex - 3, closeBIndex - (openBIndex - 4));
					openedBracketsNo--; closedBracketsNo--;
					RHS.insert(openBIndex - 3, temporaryName);
					if (temporaryName[5] < '9')temporaryName[5]++;
					else
					{
						temporaryName[5] = 1;
						(temporaryName[4] < '9') ? temporaryName[4]++ : temporaryName[4] = 1;
					}
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

					storedMatrices.push_back(*temp);
					RHS.erase(openBIndex - 3, closeBIndex - (openBIndex - 4));
					openedBracketsNo--; closedBracketsNo--;
					RHS.insert(openBIndex - 3, temporaryName);
					if (temporaryName[5] < '9')temporaryName[5]++;
					else
					{
						temporaryName[5] = 1;
						(temporaryName[4] < '9') ? temporaryName[4]++ : temporaryName[4] = 1;
					}
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
				storedMatrices.push_back(*temp);
				RHS.erase(transposeOperandstart, (pos + 1) - transposeOperandstart);
				RHS.insert(transposeOperandstart, temporaryName);
				if (temporaryName[5] < '9')temporaryName[5]++;
				else
				{
					temporaryName[5] = 1;
					(temporaryName[4] < '9') ? temporaryName[4]++ : temporaryName[4] = 1;
				}

			}
		}

	}  //end transpose



	   //negate
	if (RHS.find("+-") != RHS.npos || RHS.find("--") != RHS.npos || RHS.find("/-") != RHS.npos || RHS.find("*-") != RHS.npos || RHS.find('-') == 0)
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
				if (RHS[j] == '+' || RHS[j] == '-' || RHS[j] == '*' || RHS[j] == '/' || RHS[j] == ')' || RHS[j] == '(' || RHS[j] == 39 /* ely hya ' */ || RHS[j] == '`')
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
			if (operandindex == -1)throw("operand not defined.");
			else {
				Matrix m = -storedMatrices[operandindex];
				*temp = m;
				temp->setName(temporaryName);
			}
			RHS.erase(pos, negOperand.length());
			RHS.insert(pos, temporaryName);
			storedMatrices.push_back(*temp);
			if (temporaryName[5] < '9')temporaryName[5]++;
			else
			{
				temporaryName[5] = 1;
				(temporaryName[4] < '9') ? temporaryName[4]++ : temporaryName[4] = 1;
			}
		}
	}



	while (RHS.find("+") != RHS.npos || RHS.find("-") != RHS.npos || RHS.find("*") != RHS.npos || RHS.find("/") != RHS.npos)
	{

		Matrix *parameter1 = new Matrix, *parameter2 = new Matrix;
		string op;
		int opIndex;
		vector<string> operands(2);
		vector<int> operandIndeces(2);
		bool timesDivideDetect = false;
		for (size_t i = 1; i < RHS.length(); i++)
		{
			if (RHS[i] == '.' && (RHS[i + 1] == '*' || RHS[i + 1] == '/'))
			{
				for (size_t j = i; j >= 1; j--)
				{
					if (RHS[j - 1] == '+' || RHS[j - 1] == '-' || RHS[j - 1] == '*' || RHS[j - 1] == '/' || RHS[j - 1] == ')' || RHS[j - 1] == '(' || RHS[j - 1] == 39 /* ely hya ' */ || RHS[j - 1] == '`' || RHS[j - 1] == '.')
						break;
					char c = RHS[j - 1];
					operands[0] = c + operands[0];
				}
				for (size_t j = i + 2; j < RHS.length(); j++)
				{
					if (RHS[j] == '+' || RHS[j] == '-' || RHS[j] == '*' || RHS[j] == '/' || RHS[j] == ')' || RHS[j] == '(' || RHS[j] == 39 /* ely hya ' */ || RHS[j] == '`' || RHS[j] == '.')
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
					if (RHS[j - 1] == '+' || RHS[j - 1] == '-' || RHS[j - 1] == '*' || RHS[j - 1] == '/' || RHS[j - 1] == ')' || RHS[j - 1] == '(' || RHS[j - 1] == 39 /* ely hya ' */ || RHS[j - 1] == '`' || RHS[j - 1] == '.')
						break;
					char c = RHS[j - 1];
					operands[0] = c + operands[0];
				}
				for (size_t j = i + 1; j < RHS.length(); j++)
				{
					if (RHS[j] == '+' || RHS[j] == '-' || RHS[j] == '*' || RHS[j] == '/' || RHS[j] == ')' || RHS[j] == '(' || RHS[j] == 39 /* ely hya ' */ || RHS[j] == '`' || RHS[j] == '.')
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
						if (RHS[j - 1] == '+' || RHS[j - 1] == '-' || RHS[j - 1] == '*' || RHS[j - 1] == '/' || RHS[j - 1] == ')' || RHS[j - 1] == '(' || RHS[j - 1] == 39 /* ely hya ' */ || RHS[j - 1] == '`' || RHS[j - 1] == '.')
							break;
						char c = RHS[j - 1];
						operands[0] = c + operands[0];
					}
					for (size_t j = i + 2; j < RHS.length(); j++)
					{
						if (RHS[j] == '+' || RHS[j] == '-' || RHS[j] == '*' || RHS[j] == '/' || RHS[j] == ')' || RHS[j] == '(' || RHS[j] == 39 /* ely hya ' */ || RHS[j] == '`' || RHS[j] == '.')
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
						if (RHS[j - 1] == '+' || RHS[j - 1] == '-' || RHS[j - 1] == '*' || RHS[j - 1] == '/' || RHS[j - 1] == ')' || RHS[j - 1] == '(' || RHS[j - 1] == 39 /* ely hya ' */ || RHS[j - 1] == '`' || RHS[j - 1] == '.')
							break;
						char c = RHS[j - 1];
						operands[0] = c + operands[0];
					}
					for (size_t j = i + 1; j < RHS.length(); j++)
					{
						if (RHS[j] == '+' || RHS[j] == '-' || RHS[j] == '*' || RHS[j] == '/' || RHS[j] == ')' || RHS[j] == '(' || RHS[j] == 39 /* ely hya ' */ || RHS[j] == '`' || RHS[j] == '.')
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
		else *parameter2 = storedMatrices[operandIndeces[1]];


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
		storedMatrices.push_back(*temp);
		if (temporaryName[5] < '9')temporaryName[5]++;
		else
		{
			temporaryName[5] = 1;
			(temporaryName[4] < '9') ? temporaryName[4]++ : temporaryName[4] = 1;
		}

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
