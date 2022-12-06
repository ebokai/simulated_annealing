#include "header.h"

Partition get_data(string fname, Partition &p_struct){

	// PARSE DATAFILE 

	uint64_t state;
	string fpath = "../data/" + fname + ".dat";
	string line, subline;
	ifstream myfile(fpath);

	while (getline(myfile, line)){
		subline = line.substr(0,n);
		state = bitset<n>(subline).to_ulong();
		p_struct.data[state] += 1;
		p_struct.N += 1;
	}

	myfile.close();

	cout << p_struct.N << endl;

	return p_struct;
}