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


using namespace std;

void inputHandling(string, vector<Matrix>&, vector<string>&);



struct operators
{
	int pos;
	string op;
};

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
					if (fileLine[fileLine.length() - 1] == ';');
					else temp += ";";
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
		bool newEntry = true;
		while (true) {
			static string temp = "";
			try {
				if (newEntry)cout << ">> ";
				else cout << "\t";

				string line = "";
				getline(cin, line, '\n');

				static int openBrackets = 0, closeBrackets = 0;
				/** Detect Multi-line Commands **/
				for (size_t i = 0; i<line.length(); i++) {
					if (line[i] == '[')openBrackets++;
					else if (line[i] == ']')closeBrackets++;
				}
				while (line.find("\r") != line.npos)line.erase(line.find("\r"), 1);
				while (line.find("\n") != line.npos)line.erase(line.find("\n"), 1);

				if (openBrackets>closeBrackets) {  //command haven't ended yet
					temp += line;
					if (line[line.length() - 1] == ';');
					else temp += ";";
					newEntry = false;
					continue;
				}
				else {
					openBrackets = 0, closeBrackets = 0;
					commandLine = temp + line;
					temp = "";
					newEntry = true;
				}
				/** END DETECTING **/

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
				newEntry = true;
				temp = "";
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

	for (size_t ii = 0; ii<input.length(); ii++)
	{
		if (input[ii] == '=')
		{
			while (input[ii + 1] == ' ')input.erase(ii + 1, 1);
		}
	}



	/*************START STRING INITIALIZING ******************/

	if (input.find("[") != input.npos && input.find("[") == input.find("=") + 1) {
		if (input.find("]") != input.npos) {
			if (input.rfind("]") != input.length() - 1) {
				string afterMatrix = input.substr(input.rfind("]") + 1);
				if (afterMatrix.find_first_not_of(" ;") == afterMatrix.npos)print = false;
				input.erase(input.rfind("]") + 1, afterMatrix.length());
			}
		}
		else throw ("Close the brackets.");

		/* storing */
		Matrix inputAssignment(arrayOfLHS[0], input, storedMatrices, systemCommands);
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


	else if ( ( RHS.find("+") != RHS.npos )
                || ( (RHS.find("-") != RHS.npos) && (RHS.find("-")>0 || (RHS.find("-") == 0 && RHS.find_first_not_of(numbers) != RHS.npos) || (RHS.find("-") == 0 && RHS.find_first_of(alphabets) == RHS.npos && RHS.find_last_of("+-/") != RHS.npos && RHS.find_last_of("+-/")>1)) )
                || ( RHS.find("*") != RHS.npos ) || ( RHS.find("/") != RHS.npos )
                || ( RHS.find("inv") != RHS.npos ) || ( RHS.find("'") != RHS.npos ) || ( RHS.find("`") != RHS.npos )
                || ( RHS.find("det") != RHS.npos ) || ( RHS.find("(") != RHS.npos  )|| ( RHS.find(")") != RHS.npos ) || ( RHS.find("^") != RHS.npos) )
	{
		Matrix* temp = new Matrix;
		*temp = Matrix::multiOpHandling(RHS, storedMatrices, systemCommands, print);
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
	}

	/**********END STRING INITIALIZTING *************/



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
