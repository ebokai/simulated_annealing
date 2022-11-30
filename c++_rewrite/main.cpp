#include "header.h"
#include <time.h>


int main(int argc, char **argv){

	srand(time(NULL));

	string fname = argv[1];
	int N = 0;

	vector<pair<uint64_t, double>> partition = fixed_partition(4, 5);
	map<uint64_t, int> data = get_data(N, fname);
	pair<uint64_t, double> community;

	double logE = 0;

	for (int i = 0; i < 4; i++){
		community = partition[i]; 
		logE += icc_evidence(community, data, N);

	}
	

	cout << logE << endl;
	
	return 0;
}