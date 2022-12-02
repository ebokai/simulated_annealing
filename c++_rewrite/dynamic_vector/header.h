#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <map>

using namespace std;

const int n = 20;

struct Partition{
	map<uint64_t, int> data;
	vector<pair<uint64_t, double>> current_partition;
	vector<pair<uint64_t, double>> best_partition;
	double T;
	double current_logE = 0;
	double best_logE;
};

// PARTITION GENERATORS
vector<pair<uint64_t, double>> fixed_partition(int nc, int npc);
vector<pair<uint64_t, double>> random_partition();

// CANDIDATE FUNCTIONS
Partition merge_partition(Partition &p_struct, int &N);
Partition split_partition(Partition &p_struct, int &N);
Partition switch_partition(Partition &p_struct, int &N, int rd);

// DATA FUNCTIONS
map<uint64_t, int> get_data(int &N, string fname);
map<uint64_t, int> build_pdata(map<uint64_t, int> &data, uint64_t pbit);

// HELPER FUNCTIONS
double icc_evidence(uint64_t community, map<uint64_t, int> &data, int &N);
void print_partition(Partition p_struct);