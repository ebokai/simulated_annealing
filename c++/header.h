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

// FUNCTION DEFS ======================
void partition_print(map<int, int> partition);
void partition_write(map<int, int> partition, string fname, string idf, string cs);
int max_comm(map<int,int> partition);
map<uint32_t, int> get_data(int &N, string fname);
map<uint32_t, int> build_pdata(map<uint32_t, int> data, uint32_t pbit);
map<int, int> merge_partition(map<int,int> partition);
map<int, int> split_partition(map<int,int> partition);
map<int, int> switch_partition(map<int,int> partition);
double evidence(map<int,int> partition, map<uint32_t, int> data, int &N);
double get_voi(map<int,int> p1, map<int, int> p2);
// ====================================