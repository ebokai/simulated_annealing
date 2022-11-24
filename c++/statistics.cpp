#include "header.h"

double evidence(map<int,int> partition, map<uint32_t, int> data, int &N){

	double logE = 0;
	uint32_t rank_pow;
	double pf;
	int k;

	int np = max_comm(partition);
	map<int, uint32_t> pbits;
	map<int, int> rank;

	for(int i = 0; i < n; i++){
		pbits[partition[i]] += pow(2,i);
		rank[partition[i]] += 1;
	}

	for(int i = 0; i <= np; i++){
		map<uint32_t, int> pdata = build_pdata(data, pbits[i]);
		map<uint32_t, int>::iterator it; 
		rank_pow = pow(2, rank[i] - 1);
		pf = lgamma(rank_pow) - lgamma(N + rank_pow);
		logE += pf;
		for (it = pdata.begin(); it != pdata.end(); it ++){
			k = it -> second;
			logE += lgamma(k + 0.5) - lgamma(0.5);
		}
	}

	return logE;
}

map<uint32_t, int> build_pdata(map<uint32_t, int> data, uint32_t pbit){

	map<uint32_t, int> pdata;
	map<uint32_t, int>::iterator it;

	int ks; 
	uint32_t state, mask_state;

	for(it = data.begin(); it != data.end(); it++){

		state = it -> first;
		ks = it -> second;

		mask_state = state & pbit;
		pdata[mask_state] += ks;

	}

	return pdata;

}

double get_voi(map<int,int> p1, map<int, int> p2){

	double mut = 0; // mutual information
	double ent = 0; // entropy
	double voi = 0; // variation of information
	int n1 = 0, n2 = 0; // number of communities in partition

	map<int,uint32_t> comms1;
	map<int,uint32_t> comms2; 

	for (int i = 0; i < n; i++){
		int c1 = p1[i];
		int c2 = p2[i];

		if (c1 >= n1) {n1 = c1+1;}
		if (c2 >= n2) {n2 = c2+1;}

		comms1[c1] += pow(2,i); // represent members as binary integer
		comms2[c2] += pow(2,i);		

	}

	for (int i = 0; i < n1; i++){

		float p1 = (float)(bitset<n>(comms1[i]).count()) / (float)(n);

		for (int j = 0; j < n2; j++){

			float p2 = (float)(bitset<n>(comms2[j]).count()) / (float)(n);
			float p12 = (float)(bitset<n>(comms1[i] & comms2[j]).count())/(float)(n);

			if (p12 > 0){
				mut += p12 * log(p12/p1/p2);
				ent += p12 * log(p12);
			}


		}
	}

	voi = 1 + mut/ent;

	return voi;


}