#include "header.h"

double icc_evidence(pair<uint64_t, double> community, map<uint64_t, int> data, int &N){

	double logE = 0;
	int k;

	map<uint64_t, int> pdata = build_pdata(data, community.first);
	map<uint64_t, int>::iterator it;

	unsigned int rank = count_bits(community.first);
	double rank_pow = (1 << (rank - 1));
	double pf = lgamma(rank_pow) - lgamma(N + rank_pow);

	logE += pf;
	for (it = pdata.begin(); it != pdata.end(); it ++){
			k = it -> second;
			logE += lgamma(k + 0.5) - lgamma(0.5);
		}

	cout << rank << endl;

	return logE;
}

map<uint64_t, int> build_pdata(map<uint64_t, int> data, uint64_t pbit){

	map<uint64_t, int> pdata;
	map<uint64_t, int>::iterator it;

	int ks; 
	uint64_t state, mask_state;

	for(it = data.begin(); it != data.end(); it++){

		state = it -> first;
		ks = it -> second;

		mask_state = state & pbit;
		pdata[mask_state] += ks;

	}

	return pdata;

}

unsigned int count_bits(uint64_t number){
	unsigned int count = 0;
	while (number != 0){
		count++;
		number &= number - 1;
	}
	return count;
}