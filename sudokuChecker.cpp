#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <pthread.h>
using namespace std;

/*
1 thread to check columns
1 thread to check rows
9 threads to check 3x3 squares
*/
const int numThreads = 11;

//Empty 2d array that will be filled from input file
int puzzle[9][9];

//structure for passing data to threads
struct parameters
{
	int row;
	int column;
};

int main() {


	cout << "Enter the name of the text file (within this directory) that " <<
	"you wish to check --> ";

	string file; // Going to test this file for Sudoku correctness.
	cin >> file;

	ifstream fin; //Input stream to read given file
	fin.open(file, ios::in); //Opening file for reading

	char ch; //Current character when reading file

	for (int col = 0; col < 9; col++) { //Iterate through columns
		for (int row = 0; row < 9; row++) { //Iterate through rows
			fin.get(ch);
			puzzle[row][col] = ch;
			cout << puzzle;
			cout << '\n';
		}
	}





/* use this later
	parameters *data = (parameters*) malloc(sizeof(parameters));
	data->row = 1;
	*/


  return 0;
}
