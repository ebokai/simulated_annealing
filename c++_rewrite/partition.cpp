#include "header.h"


vector<pair<uint64_t, double>> fixed_partition(int nc, int npc){

	vector<pair<uint64_t, double>> partition;
	uint64_t p;

	for (int i = 0; i < nc; i++){

		if (i == 0){
			p = (1 << npc) - 1;
		} else {
			p = (p << npc);
		}

		partition.push_back(make_pair(p,0));
		cout << bitset<n>(p) << endl; 
	}

	return partition;
}