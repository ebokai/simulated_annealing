#include "header.h"
#include <time.h>


int main(int argc, char **argv){

	srand(time(NULL));

	// string fname = argv[1];

	vector<pair<uint64_t, double>> partition = fixed_partition(4, 5);
	
	return 0;
}