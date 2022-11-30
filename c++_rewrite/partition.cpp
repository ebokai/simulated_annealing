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

vector<pair<uint64_t, double>> merge_partition(vector<pair<uint64_t, double>> partition, map<uint64_t, int> &data, int &N){

	int nc = partition.size();
	cout << nc << endl;

	if (nc < 2){return partition;}
	int p1, p2;

	while (p1 == p2){
		p1 = rand()/(RAND_MAX/nc);
		p2 = rand()/(RAND_MAX/nc);
	}

	cout << p1 << " " << p2 << endl;

	int pl, ph;
	if (p1 > p2){
		ph = p1;
		pl = p2;
	} else {
		ph = p2;
		pl = p1;
	}

	uint64_t c1 = partition[p1].first;
	double l1 = partition[p1].second;

	uint64_t c2 = partition[p2].first;
	double l2 = partition[p2].second;

	partition.erase(partition.begin() + ph);
	partition.erase(partition.begin() + pl);

	uint64_t new_c = c1 + c2;

	cout << bitset<n>(c1) << endl;
	cout << bitset<n>(c2) << endl;
	cout << bitset<n>(new_c) << endl; 

	double new_logE = icc_evidence(new_c, data, N);
	partition.push_back(make_pair(new_c, new_logE));

	nc = partition.size();
	cout << nc << endl;
	cout << new_logE - l1 - l2 << endl;

	return partition;

}