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

Partition merge_partition(Partition p_struct, int &N){

	int nc = p_struct.current_partition.size();
	cout << nc << endl;

	if (nc < 2){return p_struct;}
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

	uint64_t c1 = p_struct.current_partition[p1].first;
	double l1 = p_struct.current_partition[p1].second;

	uint64_t c2 = p_struct.current_partition[p2].first;
	double l2 = p_struct.current_partition[p2].second;

	p_struct.current_partition.erase(p_struct.current_partition.begin() + ph);
	p_struct.current_partition.erase(p_struct.current_partition.begin() + pl);

	uint64_t new_c = c1 + c2;

	cout << bitset<n>(c1) << endl;
	cout << bitset<n>(c2) << endl;
	cout << bitset<n>(new_c) << endl; 

	double new_logE = icc_evidence(new_c, p_struct.data, N);
	p_struct.current_partition.push_back(make_pair(new_c, new_logE));

	nc = p_struct.current_partition.size();

	cout << nc << endl;
	cout << new_logE - l1 - l2 << endl;

	return p_struct;

}

Partition split_partition(Partition p_struct, int &N){

	int nc = p_struct.current_partition.size();

	int p = rand()/(RAND_MAX/nc);
	uint64_t c = p_struct.current_partition[p].first;
	double logE = p_struct.current_partition[p].second;
	
	int t = 0;

	// can't split partitions with one node
	if (bitset<n>(c).count() == 1){return p_struct;}

	uint64_t mask = rand();
	uint64_t c1 = (c & mask);
	uint64_t c2 = (c & (~mask));
	
	// prevent empty partition
	while ((c1 == 0) || (c2 == 0)){
		mask = rand();
		c1 = (c & mask);
		c2 = (c & (~mask));
		t++;
	}

	cout << bitset<n>(c) << endl; 
	cout << bitset<n>(c1) << endl;
	cout << bitset<n>(c2) << endl;
	
	p_struct.current_partition.erase(p_struct.current_partition.begin() + p);

	double new_l1 = icc_evidence(c1, p_struct.data, N);
	double new_l2 = icc_evidence(c2, p_struct.data, N);

	p_struct.current_partition.push_back(make_pair(c1, new_l1));
	p_struct.current_partition.push_back(make_pair(c2, new_l2));

	nc = p_struct.current_partition.size();

	cout << nc << endl;
	cout << new_l1 + new_l2 - logE << endl;
	cout << "got stuck " << t << " times." << endl;

	return p_struct;
}

Partition switch_partition(Partition p_struct, int &N, int rd){

	rd++;

	int nc = p_struct.current_partition.size();
	if (nc < 2) {return p_struct;}

	int i = rand()/(RAND_MAX/n);
	uint64_t x = (1 << i);

	int p1, p2;
	while (p1 == p2){
		p1 = rand()/(RAND_MAX/nc);
		p2 = rand()/(RAND_MAX/nc);
	}

	uint64_t y = p_struct.current_partition[p1].first;
	uint64_t z = p_struct.current_partition[p2].first;
	double ly = p_struct.current_partition[p1].second;
	double lz = p_struct.current_partition[p2].second;

	if (((x & y) == x) && (x != y)){
		y = y - x;
		z = z + x;
		double lny = icc_evidence(y, p_struct.data, N);
		double lnz = icc_evidence(z, p_struct.data, N);
		p_struct.current_partition[p1] = make_pair(y, lny);
		p_struct.current_partition[p2] = make_pair(z, lnz);

		cout << lny + lnz - ly - lz << endl;

	} else if (((x & z) == x) && (x != z)) {
		z = z - x;
		y = y + x;
		double lny = icc_evidence(y, p_struct.data, N);
		double lnz = icc_evidence(z, p_struct.data, N);
		p_struct.current_partition[p1] = make_pair(y, lny);
		p_struct.current_partition[p2] = make_pair(z, lnz);

		cout << lny + lnz - ly - lz << endl;
		
	} else {
		cout << "node not in partition" << endl;
		// call function recursively until switch is found
		p_struct = switch_partition(p_struct, N, rd);

	}

	cout << "recursion depth: " << rd << endl;

	return p_struct;
}