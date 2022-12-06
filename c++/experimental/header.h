#include <iostream>
#include <vector>
#include <bitset>
#include <random>
#include <map>
#include <string>
#include <fstream>

using namespace std;

const int n = 20;
const uint64_t one = 1;


struct Partition{

	double T;
	double current_log_evidence = 0;
	double best_log_evidence = 0;

	unsigned int N = 0;
	unsigned int nc = 0;

	uint64_t occupied_partitions = 0;
	uint64_t occupied_partitions_gt2_nodes = 0; // communities with more than two nodes (for split and switch)

	map<uint64_t, unsigned int> data;
	vector<uint64_t> current_partition = vector<uint64_t>(n);
	vector<double> partition_evidence = vector<double>(n);
	
};

unsigned int randomBitIndex(uint64_t v);
double icc_evidence(uint64_t community, Partition &p_struct);
Partition get_data(string fname, Partition &p_struct);
Partition random_partition(Partition &p_struct);
Partition merge_partition(Partition &p_struct);
Partition split_partition(Partition &p_struct);
Partition switch_partition(Partition &p_struct);