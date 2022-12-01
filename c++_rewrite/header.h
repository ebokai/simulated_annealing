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
};

vector<pair<uint64_t, double>> fixed_partition(int nc, int npc);
Partition merge_partition(Partition p_struct, int &N);
Partition split_partition(Partition p_struct, int &N);
Partition switch_partition(Partition p_struct, int &N, int rd);

map<uint64_t, int> get_data(int &N, string fname);
map<uint64_t, int> build_pdata(map<uint64_t, int> &data, uint64_t pbit);

double icc_evidence(uint64_t community, map<uint64_t, int> &data, int &N);

unsigned int count_bits(uint64_t number);
