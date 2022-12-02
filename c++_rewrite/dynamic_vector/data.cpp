#include "header.h"

map<uint64_t, int> get_data(int &N, string fname){

	// PARSE DATAFILE 

	map<uint64_t, int> data;
	uint64_t state;
	string fpath = "./data/" + fname + ".dat";
	string line, subline;
	ifstream myfile(fpath);

	while (getline(myfile, line)){
		subline = line.substr(0,n);
		state = bitset<n>(subline).to_ulong();
		data[state] += 1;
		N += 1;
	}

	myfile.close();

	return data;
}