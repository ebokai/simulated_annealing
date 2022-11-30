#include "header.h"
#include <time.h>


int main(int argc, char **argv){

	srand(time(NULL));

	string fname = argv[1];
	int N = 0;
	int rd = 0;

	Partition partition;
	partition.current_partition = fixed_partition(4, 5);
	partition.data = get_data(N, fname);

	uint64_t community;
	for (int i = 0; i < 4; i++){
		community = partition.current_partition[i].first; 
		partition.current_partition[i] = make_pair(community, icc_evidence(community, partition.data, N));
	}

	// DO METROPOLIS STEP DIRECTLY IN CANDIDATE FUNCTIONS?
	// MAKE STRUCT THAT HAS CURRENT LOG E, CURRENT PARTITION AND BEST LOG E AND BEST PARTITION
	partition.current_partition = merge_partition(partition.current_partition, partition.data, N);
	partition.current_partition = split_partition(partition.current_partition, partition.data, N);

	cout << "before switch:" << endl;
	for (int i = 0; i < partition.current_partition.size(); i++){
		cout << bitset<n>(partition.current_partition[i].first) << endl;
	}

	partition.current_partition = switch_partition(partition.current_partition, partition.data, N, rd);

	cout << "after switch:" << endl;
	for (int i = 0; i < partition.current_partition.size(); i++){
		cout << bitset<n>(partition.current_partition[i].first) << endl;
	}
	return 0;
}