#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <map>
#include <utility>

using namespace std;

const int n = 20;

struct Partition{
	map<uint64_t, int> data;
	vector<pair<uint64_t, double>> current_partition = vector<pair<uint64_t, double>>(n, make_pair(0,0));
	vector<pair<uint64_t, double>> best_partition = vector<pair<uint64_t, double>>(n, make_pair(0,0));
	double T;
	double current_logE = 0;
	double best_logE;
	uint64_t occupied = 0;
};

// PARTITION GENERATORS
Partition fixed_partition(Partition &p_struct, int nc, int npc);
Partition random_partition(Partition &p_struct);

// CANDIDATE FUNCTIONS
Partition merge_partition(Partition &p_struct, int &N);
Partition split_partition(Partition &p_struct, int &N);
Partition switch_partition(Partition &p_struct, int &N);

// DATA FUNCTIONS
map<uint64_t, int> get_data(int &N, string fname);
map<uint64_t, int> build_pdata(map<uint64_t, int> &data, uint64_t pbit);

// HELPER FUNCTIONS
double icc_evidence(uint64_t community, map<uint64_t, int> &data, int &N);
void print_partition(Partition p_struct);