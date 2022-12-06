#include <iostream>
#include <vector>
#include <bitset>
#include <random>
#include <map>
#include <unordered_map>
#include <string>
#include <fstream>


using namespace std;

const int n = 20; 
const uint64_t ONE = 1; // uint64_t representation of 1
const double EPSILON = 1e-4; // minimum change in log-evidence


struct Partition{

	double T; // annealing temperature
	double current_log_evidence = 0;
	double best_log_evidence = 0;

	unsigned int N = 0; // number of samples in dataset
	unsigned int nc = 0; // number of communities

	uint64_t occupied_partitions = 0; // occupied communities (with at least one node)
	uint64_t occupied_partitions_gt2_nodes = 0; // communities with at least two nodes (for split and switch)

	map<uint64_t, unsigned int> data;
	vector<uint64_t> current_partition = vector<uint64_t>(n);
	vector<double> partition_evidence = vector<double>(n);
	unordered_map<uint64_t, double> calculated_log_evidence; // store previously calculated log-evidence (slower)
	
};

bool DoubleSame(double a, double b);
unsigned int randomBitIndex(uint64_t v);
double icc_evidence(uint64_t community, Partition &p_struct);
Partition get_data(string fname, Partition &p_struct);
Partition random_partition(Partition &p_struct);
Partition merge_partition(Partition &p_struct);
Partition split_partition(Partition &p_struct);
Partition switch_partition(Partition &p_struct);