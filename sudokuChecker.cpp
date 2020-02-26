/*
Larson, Christoph
clarso5@lsu.edu
PA-1 (Multithreading)
Feng Chen
cs4103-sp20
cs410340
*/
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
int correct[numThreads] = {0};

//Structure for passing data to threads
struct subsection
{
	int row;
	int column;
};

// Checks that 1-9 appear exactly once in given column
void  *validCol (void* param) {
	// ensures valid column is passed
	subsection *params = (subsection*) param;
	int row = params->row;
	int col = params->column;

	if (col > 8 || row != 0) {
		pthread_exit(NULL);
	}

	// Function logic
	int appearance[9] = {0};
	int checkRow;
	for(checkRow = 0; checkRow < puzzleDimensions; checkRow++) {
		int answer = puzzle[checkRow][col];
		/*condition below verifies valid input and checks if the value of "answer"
		already appears in the given column*/
		if(appearance[answer-1] == 1 || answer < 1 || answer > 9) {
			cout << "Thread " << pthread_self() << ", Column " << col << ", Invalid\n";
			pthread_exit(NULL);
		} else {
			appearance[answer-1] = 1;
		}
	}
	//upon leaving for loop without exiting the thread, col is valid
	correct[col] = 1;
	cout << "Thread " << pthread_self() << ", Column " << col << ", Valid\n";
	pthread_exit(NULL);
}


// Checks that 1-9 appear exactly once in given row
void *validRow (void* param) {
// ensures valid column is passed
subsection *params = (subsection*) param;
int row = params->row;
int col = params->column;

if (row > 8 || col != 0) {
	pthread_exit(NULL);
}

// Function logic
int appearance[9] = {0};
int checkCol;
for(checkCol = 0; checkCol < puzzleDimensions; checkCol++) {
	int answer = puzzle[row][checkCol];
	/*condition below verifies valid input and checks if the value of "answer"
	already appears in the given row*/
	if(appearance[answer-1] == 1 || answer < 1 || answer > 9) {
		cout << "Thread " << pthread_self() << ", Row " << col << ", Invalid\n";
		pthread_exit(NULL);
	} else {
		appearance[answer - 1] = 1;
	}
}
//upon leaving for loop without exiting the thread, row is valid
correct[puzzleDimensions + row] = 1; /* offset in validity array to account for
the already-checked columns*/
cout << "Thread " << pthread_self() << ", Row " << col << ", Valid\n";
pthread_exit(NULL);
}


// Checks that 1-9 appear exactly once in given 3x3 square
void *validSquare(void* param) {
	// Ensures valid square is passed
	subsection *params = (subsection*) param;
	int row = params->row;
	int col = params->column;
	if (row > 6 || row%3 != 0 || col > 6 || col%3 != 0) {
		pthread_exit(NULL);
	}
	// Function logic
	int appearance[9] = {0};
	int checkRow, checkCol;
	for (checkRow = row; checkRow < row+3; checkRow++) {
		for (checkCol = col; checkCol < col+3; checkCol++) {
			int answer = puzzle[checkRow][checkCol];
			if (answer < 1 || answer > 9 || appearance[answer-1] == 1) {
				row = params->row;
				col = params->column;// Reset these local variables to document thread
				printf("Thread %ld, Subgrid R%d%d%dxC%d%d%d, Invalid\n", pthread_self(),
				row, row+1, row+2, col, col+1, col+2);
				pthread_exit(NULL);
			} else {
				appearance[answer-1] = 1;
			}
		}
	}
	correct[18 + row + col/3] = 1; /* Maps the subsection to an index in
	the last 8 indices of the valid array*/
	row = params->row;
	col = params->column;// Reset these local variables to document thread
	printf("Thread %ld, Subgrid R%d%d%dxC%d%d%d, Valid\n", pthread_self(),
	row, row+1, row+2, col, col+1, col+2);
	pthread_exit(NULL);
}

int main() {
	// Inputting the proposed solution text file
	cout << "Enter the name of the text file that " <<
	"you wish to check --> ";

	string file; // Going to test this file for Sudoku correctness.
	cin >> file;

	ifstream fin; // Input stream to read given file
	fin.open(file, ios::in); // Opening file for reading

	// Current number when reading file
	int ch;

	//cout << "\nYour proposed solution is:\n";
	for (int row = 0; row < puzzleDimensions; row++) { // Iterate through rows
		for (int col = 0; col < puzzleDimensions; col++) { // Iterate through columns
			fin >> ch;
			puzzle[row][col] = ch;
			}
		}

			pthread_t threads[numThreads];

			int tCount = 0;
			int r,c;

			// Create 27 threads for the purposed described in the header
			for(r = 0; r < 9; r++) {
				for(c = 0; c < 9; c++) {
					if(r == 0) {
						subsection *colParams = (subsection*) malloc(sizeof(subsection));
						colParams->row = r;
						colParams->column = c;
						// Column threads
						pthread_create(&threads[tCount++], NULL, validCol, colParams);
					}

					if(c == 0) {
						subsection *rowParams = (subsection*) malloc(sizeof(subsection));
						rowParams->row = r;
						rowParams->column = c;
						// Row threads
						pthread_create(&threads[tCount++], NULL, validRow, rowParams);
					}

					if(c%3 == 0 && r%3 == 0) {
						subsection *squareParams = (subsection*) malloc(sizeof(subsection));
						squareParams->row = r;
						squareParams->column = c;
						// 3x3 square threads
						pthread_create(&threads[tCount++], NULL, validSquare, squareParams);
					}
				}
			}

			int i;
			for (i = 0; i < numThreads; i++) {
				pthread_join(threads[i], NULL); // Awaiting completion of all threads
			}

  return 0;
}
