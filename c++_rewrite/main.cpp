#include "header.h"
#include <time.h>


int main(int argc, char **argv){

	srand(time(NULL));

	string fname = argv[1];
	int N = 0;
	int rd = 0;

	Partition p_struct;
	p_struct.current_partition = fixed_partition(4, 5);
	p_struct.data = get_data(N, fname);

	uint64_t community;
	for (int i = 0; i < 4; i++){
		community = p_struct.current_partition[i].first; 
		p_struct.current_partition[i] = make_pair(community, icc_evidence(community, p_struct.data, N));
	}

	// DO METROPOLIS STEP DIRECTLY IN CANDIDATE FUNCTIONS?
	// MAKE STRUCT THAT HAS CURRENT LOG E, CURRENT p_struct AND BEST LOG E AND BEST p_struct
	p_struct = merge_partition(p_struct, N);
	p_struct = split_partition(p_struct, N);

	cout << "before switch:" << endl;
	for (int i = 0; i < p_struct.current_partition.size(); i++){
		cout << bitset<n>(p_struct.current_partition[i].first) << endl;
	}

	p_struct = switch_partition(p_struct, N, rd);

	cout << "after switch:" << endl;
	for (int i = 0; i < p_struct.current_partition.size(); i++){
		cout << bitset<n>(p_struct.current_partition[i].first) << endl;
	}
	return 0;
}