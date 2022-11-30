#include "header.h"
#include <time.h>


int main(int argc, char **argv){

	srand(time(NULL));

	string fname = argv[1];
	int N = 0;
	int rd = 0;

	vector<pair<uint64_t, double>> partition = fixed_partition(4, 5);
	map<uint64_t, int> data = get_data(N, fname);
	uint64_t community;

	for (int i = 0; i < 4; i++){
		community = partition[i].first; 
		partition[i] = make_pair(community, icc_evidence(community, data, N));
	}
	
	partition = merge_partition(partition, data, N);
	partition = split_partition(partition, data, N);

	cout << "before switch:" << endl;
	for (int i = 0; i < partition.size(); i++){
		cout << bitset<n>(partition[i].first) << endl;
	}

	partition = switch_partition(partition, data, N, rd);

	cout << "after switch:" << endl;
	for (int i = 0; i < partition.size(); i++){
		cout << bitset<n>(partition[i].first) << endl;
	}
	return 0;
}