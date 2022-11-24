#include <map>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <bitset>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;

// PARAMETERS =========================
const int max_steps = 100000;
const int n = 20;
// ====================================

// STRUCTURES
struct pStats{
	string fname;
	double best_logE;
	double true_logE;
	double voi;
	map<int, int> best_partition;
}; 

// FUNCTION DEFS ======================

void partition_print(map<int, int> partition);
void partition_write(pStats partition_stats);
void write_statistics(pStats partition_stats);
int max_comm(map<int,int> partition);

map<uint32_t, int> get_data(int &N, string fname);
map<uint32_t, int> build_pdata(map<uint32_t, int> data, uint32_t pbit);

map<int, int> random_partition();
map<int, int> fixed_partition(int nc);
map<int, int> merge_partition(map<int,int> partition);
map<int, int> split_partition(map<int,int> partition);
map<int, int> switch_partition(map<int,int> partition);

double evidence(map<int,int> partition, map<uint32_t, int> data, int &N);
double get_voi(map<int,int> p1, map<int, int> p2);
// ====================================