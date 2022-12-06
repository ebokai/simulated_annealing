#include "header.h"

bool DoubleSame(double a, double b){
	return fabs(a-b) < EPSILON;
}

map<uint64_t, unsigned int> build_pdata(map<uint64_t, unsigned int> &data, uint64_t community){

	map<uint64_t, unsigned int> pdata;
	map<uint64_t, unsigned int>::iterator it;

	int ks; 
	uint64_t state, mask_state;

	for(it = data.begin(); it != data.end(); it++){

		state = it -> first;
		ks = it -> second;

		mask_state = state & community;
		pdata[mask_state] += ks;

	}

	return pdata;

}

double icc_evidence(uint64_t community, Partition &p_struct){

	// if (p_struct.calculated_log_evidence.count(community) == 1){
	// 	return p_struct.calculated_log_evidence.at(community);
	// }

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

	// p_struct.calculated_log_evidence[community] = logE;

	return logE;
}

