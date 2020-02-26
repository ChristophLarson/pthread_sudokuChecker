#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <pthread.h>
using namespace std;

/*
1 thread to check each column
1 thread to check each row
1 threads to check each 3x3 square
*/
const int numThreads = 27;
const int puzzleDimensions = 9;

//Empty 2D array that will be filled from input file
int puzzle[puzzleDimensions][puzzleDimensions];

//Array shared by threads. 1 represents valid region, 0 represents invalid region
bool correct[numThreads];

//Structure for passing data to threads
struct subsection
{
	int row;
	int column;
};

// Checks that 1-9 appear exactly once in given column
void* validCol (void* param) {
	// ensures valid column is passed
	subsection *params = (subsection*) param;
	int row = params->row;
	int col = params->column;

	if (col > 8 || row != 0) {
		fprintf(stderr, "Error: Please select a column 0-9 and the 0th row. You chose "
		"row %d, col %d\n", row, col);
	}

	// Function logic
	int appearance[9] = {0};
	int checkRow;
	for(checkRow = 0; checkRow < puzzleDimensions; checkRow++) {
		int answer = puzzle[checkRow][col];
		/*condition below verifies valid input and checks if the value of "answer"
		already appears in the given column*/
		if(appearance[answer - 1] == 1 || answer < 1 || answer > 9) {
			pthread_exit(NULL);
		} else {
			appearance[answer - 1] = 1;
		}
	}
	//upon leaving for loop without exiting the thread, col is valid
	correct[col] = 1;
	pthread_exit(NULL);
}


// Checks that 1-9 appear exactly once in given row
void* validRow (void* param) {
// ensures valid column is passed
subsection *params = (subsection*) param;
int row = params->row;
int col = params->column;

if (row > 8 || col != 0) {
	fprintf(stderr, "Error: Please select a row 0-9 and the 0th column. You chose "
	"row %d, col %d\n", row, col);
}

// Function logic
int appearance[9] = {0};
int checkCol;
for(checkCol = 0; checkCol < puzzleDimensions; checkCol++) {
	int answer = puzzle[row][checkCol];
	/*condition below verifies valid input and checks if the value of "answer"
	already appears in the given row*/
	if(appearance[answer - 1] == 1 || answer < 1 || answer > 9) {
		pthread_exit(NULL);
	} else {
		appearance[answer - 1] = 1;
	}
}
//upon leaving for loop without exiting the thread, row is valid
correct[row + puzzleDimensions] = 1; /* offset in validity array to account for
the already-checked columns*/
pthread_exit(NULL);
}


// Checks that 1-9 appear exactly once in given 3x3 square
void* validSquare(void* param) {
	// Ensures valid square is passed
	subsection *params = (subsection*) param;
	int row = params->row;
	int col = params->column;
	if (row > 6 || row % 3 != 0 || col > 6 || col % 3 != 0) {
		fprintf(stderr, "Error: Column and row indices must be 0, 3, or 6. "
		"You chose row %d, col %d\n", row, col);
		pthread_exit(NULL);
	}

	int validityArray[9] = {0};
	int checkRow, checkCol;
	for (checkRow = row; checkRow < row + 3; checkRow++) {
		for (checkCol = col; checkCol < col + 3; checkCol++) {
			int answer = puzzle[checkRow][checkCol];
			if (answer < 1 || answer > 9 || validityArray[answer - 1] == 1) {
				pthread_exit(NULL);
			} else {
				validityArray[answer - 1] = 1;
			}
		}
	}
	validityArray[18 + row + col/3] = 1; /* Maps the subsection to an index in the last 8
	indices of the valid array*/
	pthread_exit(NULL);
}

int main() {

	// Inputting the proposed solution text file
	cout << "Enter the name of the text file that " <<
	"you wish to check --> ";

	string file; // Going to test this file for Sudoku correctness.
	cin >> file;

	ifstream fin; //Input stream to read given file
	fin.open(file, ios::in); //Opening file for reading

	//Current number when reading file
	int ch;

	for (int row = 0; row < puzzleDimensions; row++) { //Iterate through rows
		for (int col = 0; col < puzzleDimensions; col++) { //Iterate through columns
			fin >> ch;
			puzzle[row][col] = ch;
			cout <<" "<<puzzle[row][col]<< " ";
			}
			cout << '\n';
		}

  return 0;
}
