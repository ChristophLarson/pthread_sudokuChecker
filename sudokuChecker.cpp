#include <iostream>
#include <cstdlib>
#include <pthread.h>
using namespace std;

const int numThreads = 11;


//structure for passing data to threads
struct parameters
{
	int row;
	int column;
};

int main() {

	parameters *data = (parameters*) malloc(sizeof(parameters));


  return 0;
}
