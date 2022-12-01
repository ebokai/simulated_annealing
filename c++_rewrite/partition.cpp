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
		//cout << bitset<n>(p) << endl; 
	}

	return partition;
}

vector<pair<uint64_t, double>> random_partition(){

	vector<pair<uint64_t, double>> partition;
	uint64_t c; // candidate community
	uint64_t assigned = 0; // assigned nodes
	
	while(bitset<n>(~assigned).count() > 0){
		c = rand();
		c = c - (assigned & c); // remove already assigned nodes from candidate
		assigned += c; // add newly assigned nodes

		// avoid adding empty communities
		if (bitset<n>(c).count() > 0){
			partition.push_back(make_pair(c,0));
			cout << "new community: " << bitset<n>(c) << endl;
			cout << "assigned nodes: " << bitset<n>(assigned) << endl;
			cout << endl;
		}
	}

	return partition;
}

Partition merge_partition(Partition &p_struct, int &N){

	int nc = p_struct.current_partition.size();
	//cout << nc << endl;

	if (nc < 2){return p_struct;}
	int p1, p2;

	while (p1 == p2){
		p1 = rand()/(RAND_MAX/nc);
		p2 = rand()/(RAND_MAX/nc);
	}

	//cout << p1 << " " << p2 << endl;

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

	uint64_t new_c = c1 + c2;

	//cout << bitset<n>(c1) << endl;
	//cout << bitset<n>(c2) << endl;
	//cout << bitset<n>(new_c) << endl; 

	double new_logE = icc_evidence(new_c, p_struct.data, N);
	double dlogE = new_logE - l1 - l2;

	double p = exp(dlogE/p_struct.T);
	double u = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

	if (p > u){
		p_struct.current_partition.erase(p_struct.current_partition.begin() + ph);
		p_struct.current_partition.erase(p_struct.current_partition.begin() + pl);
		p_struct.current_partition.push_back(make_pair(new_c, new_logE));
		p_struct.current_logE += dlogE;
	}


	
	

	//cout << nc << endl;
	//cout << dlogE << endl;

	return p_struct;

}

Partition split_partition(Partition &p_struct, int &N){

	int nc = p_struct.current_partition.size();

	int p1 = rand()/(RAND_MAX/nc);
	uint64_t c = p_struct.current_partition[p1].first;
	double logE = p_struct.current_partition[p1].second;
	
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

	//cout << bitset<n>(c) << endl; 
	//cout << bitset<n>(c1) << endl;
	//cout << bitset<n>(c2) << endl;
	

	double new_l1 = icc_evidence(c1, p_struct.data, N);
	double new_l2 = icc_evidence(c2, p_struct.data, N);
	double dlogE = new_l1 + new_l2 - logE;

	double p = exp(dlogE/p_struct.T);
	double u = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

	if (p > u){
		p_struct.current_partition.erase(p_struct.current_partition.begin() + p1);
		p_struct.current_partition.push_back(make_pair(c1, new_l1));
		p_struct.current_partition.push_back(make_pair(c2, new_l2));
		p_struct.current_logE += dlogE;
	}


	// nc = p_struct.current_partition.size();
	//cout << nc << endl;
	//cout << dlogE << endl;
	//cout << "got stuck " << t << " times." << endl;

	return p_struct;
}

Partition switch_partition(Partition &p_struct, int &N, int rd){

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
	double dlogE, p, u;

	if (((x & y) == x) && (x != y)){
		y = y - x;
		z = z + x;
		double lny = icc_evidence(y, p_struct.data, N);
		double lnz = icc_evidence(z, p_struct.data, N);
		dlogE = lny + lnz - ly - lz;

		p = exp(dlogE/p_struct.T);
		u = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		if (p > u){
			p_struct.current_partition[p1] = make_pair(y, lny);
			p_struct.current_partition[p2] = make_pair(z, lnz);
			p_struct.current_logE += dlogE;
		}

		

		//cout << dlogE << endl;

	} else if (((x & z) == x) && (x != z)) {
		z = z - x;
		y = y + x;
		double lny = icc_evidence(y, p_struct.data, N);
		double lnz = icc_evidence(z, p_struct.data, N);
		dlogE = lny + lnz - ly - lz;

		p = exp(dlogE/p_struct.T);
		u = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		if (p > u){
			p_struct.current_partition[p1] = make_pair(y, lny);
			p_struct.current_partition[p2] = make_pair(z, lnz);
			p_struct.current_logE += dlogE;
		}


		//cout << dlogE << endl;
		
	} else {
		//cout << "node not in partition" << endl;
		// call function recursively until switch is found
		p_struct = switch_partition(p_struct, N, rd);

	}

	//cout << "recursion depth: " << rd << endl;

	return p_struct;
}