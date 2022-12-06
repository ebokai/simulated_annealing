#include "header.h"

map<uint64_t, unsigned int> build_pdata(map<uint64_t, unsigned int> &data, uint64_t pbit){

	map<uint64_t, unsigned int> pdata;
	map<uint64_t, unsigned int>::iterator it;

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

double icc_evidence(uint64_t community, Partition &p_struct){

	double logE = 0;
	int k;

	map<uint64_t, unsigned int> pdata = build_pdata(p_struct.data, community);
	map<uint64_t, unsigned int>::iterator it;

	unsigned int rank = (bitset<n>(community).count());
	double rank_pow = (1 << (rank - 1));
	double pf = lgamma(rank_pow) - lgamma(p_struct.N + rank_pow);

	logE += pf;
	for (it = pdata.begin(); it != pdata.end(); it ++){
			k = it -> second;
			logE += lgamma(k + 0.5) - lgamma(0.5);
		}

	//cout << "ICC rank: " << rank << endl;

	return logE;
}

