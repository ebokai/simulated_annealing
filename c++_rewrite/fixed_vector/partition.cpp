#include "header.h"


Partition fixed_partition(Partition &p_struct, int nc, int npc){

	// vector<pair<uint64_t, double>> partition;
	uint64_t p;

	for (int i = 0; i < nc; i++){

		if (i == 0){
			p = (1 << npc) - 1;
		} else {
			p = (p << npc);
		}

		p_struct.current_partition[i] = make_pair(p,0);
		p_struct.occupied += (1 << i);
		//cout << bitset<n>(p) << endl; 
	}

	return p_struct;
}

Partition random_partition(Partition &p_struct){

	// vector<pair<uint64_t, double>> partition;
	uint64_t c; // candidate community
	uint64_t assigned = 0; // assigned nodes

	int i = 0;
	
	while(bitset<n>(~assigned).count() > 0){
		c = rand();
		c = c - (assigned & c); // remove already assigned nodes from candidate
		assigned += c; // add newly assigned nodes

		// avoid adding empty communities
		if (bitset<n>(c).count() > 0){
			p_struct.current_partition[i] = make_pair(c,0);
			p_struct.occupied += (1 << i);
			i++;
			cout << "new community: " << bitset<n>(c) << endl;
			cout << "assigned nodes: " << bitset<n>(assigned) << endl;
			cout << endl;
		}
	}

	return p_struct;
}

Partition merge_partition(Partition &p_struct, int &N){

	int nc = bitset<n>(p_struct.occupied).count();
	//cout << nc << endl;

	if (nc < 2){return p_struct;}

	// int p1 = rand()/(RAND_MAX/nc);
	// int k1 = 1 + rand()/(RAND_MAX/(nc-1));
	// int p2 = (p1 + k1) % nc;

	bool f1 = true;
	uint64_t p1;

	while (f1) {
		p1 = rand()/(RAND_MAX/n);
		if (bitset<n>(p_struct.occupied & (1 << p1)).count()==1){
			f1 = false;
		}
		
	}

	bool f2 = true;
	uint64_t p2;

	while (f2) {
		p2 = rand()/(RAND_MAX/n);
		if ((bitset<n>(p_struct.occupied & (1 << p2)).count()==1) && (p1 != p2)){
			f2 = false;
		}
		
	}


	//cout << nc << " " << p1 << " " << p2 << " " << k1 << endl;

	// int pl, ph;
	// ph = (p1 + p2 + abs(p1-p2))/2;
	// pl = (p1 + p2 - abs(p1-p2))/2;

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
		p_struct.current_partition[p1] = make_pair(new_c, new_logE);
		p_struct.current_partition[p2] = make_pair(0, 0);
		p_struct.occupied -= (1 << p2);
		p_struct.current_logE += dlogE;
	}

	//cout << nc << endl;
	//cout << dlogE << endl;

	return p_struct;

}

Partition split_partition(Partition &p_struct, int &N){

	//int nc = p_struct.current_partition.size();

	bool f1 = true;
	uint64_t p1;
	int ch = 0;

	while (f1) {
		p1 = rand()/(RAND_MAX/n);
		if (bitset<n>(p_struct.occupied & (1 << p1)).count()==1){
			f1 = false;
		}
		ch++;
		//cout << ch << endl;
	}

	
	uint64_t c = p_struct.current_partition[p1].first;
	if (bitset<n>(c).count() == 1){return p_struct;}
	double logE = p_struct.current_partition[p1].second;
	
	int t = 0;

	// can't split partitions with one node
	

	uint64_t mask = rand();
	uint64_t c1 = (c & mask);
	uint64_t c2 = (c & (~mask));
	
	// prevent empty partition
	while ((c1 == 0) || (c2 == 0)){
		mask = rand();
		c1 = (c & mask);
		c2 = (c & (~mask));
		t++;
		//cout << "t: " << t << endl;
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
		p_struct.current_partition[p1] = make_pair(c1, new_l1);
		
		// find empty spot in vector for split partition
		bool f2 = true;
		int i = 0;
		while ((f2) && (i < n)) {
			c = p_struct.current_partition[i].first;
			//cout << "x: " << i << " " << c << endl;
			if (c == 0){
				p_struct.current_partition[i] = make_pair(c2, new_l2);
				p_struct.occupied += (1 << i);
				f2 = false;
			}
			i++;
		}
		p_struct.current_logE += dlogE;
	}


	// nc = p_struct.current_partition.size();
	//cout << nc << endl;
	//cout << dlogE << endl;
	//cout << "got stuck " << t << " times." << endl;

	return p_struct;
}

Partition switch_partition(Partition &p_struct, int &N){

	int nc = bitset<n>(p_struct.occupied).count();
	if (nc < 2){return p_struct;}

	int node = rand()/(RAND_MAX/n);
	uint64_t x = (1 << node);
	uint64_t c, cx;
	uint64_t nx;
	double lx;
	double lnx;
	double dlogE;
	int k;
	vector<int> idx;

	for (int i = 0; i < n; i++){

		c = p_struct.current_partition[i].first; // community i
		k = bitset<n>(c).count(); // check if comm. i occupied 
		cx = bitset<n>(c & x).count();
		
		if ((cx > 0) && k < 2){return p_struct;}

		if (cx > 0){
			// remove node from containing partition
			nx = (p_struct.current_partition[i].first - x);
			lx = p_struct.current_partition[i].second;
			lnx = icc_evidence(nx, p_struct.data, N);
			dlogE = lnx - lx;
			p_struct.current_partition[i] = make_pair(nx, lnx);
			p_struct.current_logE += dlogE;
		}

		if ((cx == 0) && (k > 0)){
			idx.push_back(i);
		}
	}

	// add node to other partition
	int is = idx.size();
	int cp = rand()/(RAND_MAX/is);
	int ip = idx[cp];

	lx = p_struct.current_partition[ip].second;
	nx = (p_struct.current_partition[ip].first + x);
	lnx = icc_evidence(nx, p_struct.data, N);
	dlogE = lnx - lx;


	p_struct.current_partition[ip] = make_pair(nx, lnx);
	p_struct.current_logE += dlogE;


	return p_struct;
}