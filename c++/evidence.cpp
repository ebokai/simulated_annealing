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