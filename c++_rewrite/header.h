#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <map>

using namespace std;

const int n = 20;

vector<pair<uint64_t, double>> fixed_partition(int nc, int npc);
vector<pair<uint64_t, double>> merge_partition(vector<pair<uint64_t, double>> partition, map<uint64_t, int> &data, int &N);

map<uint64_t, int> get_data(int &N, string fname);
map<uint64_t, int> build_pdata(map<uint64_t, int> &data, uint64_t pbit);

double icc_evidence(uint64_t community, map<uint64_t, int> &data, int &N);

unsigned int count_bits(uint64_t number);
