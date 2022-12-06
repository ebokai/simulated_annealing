#include "header.h"

map<uint32_t, int> get_data(int &N, string fname){

	// PARSE DATAFILE 

	map<uint32_t, int> data;
	uint32_t state;
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

void partition_print(map<int, int> partition){

	// CODE FOR PRINTING PARTITION

	int np = max_comm(partition);

	cout << "[";

	for (int i = 0; i <= np; i++){
		cout << "[";
		for (int j = 0; j < n; j++){
			if (partition[j] == i){
				cout << " " << j << " ";
			}
		}
		cout << "]";
	}

	cout << "]" << endl;
}

void partition_write(pStats partition_stats){

	// WRITE PARTITION TO FILE
	
	string fpath = "./partitions/" + partition_stats.fname + "_partition.dat";
	ofstream myfile(fpath);
	map<int,int> partition = partition_stats.best_partition;

	map<int, int>::iterator it;

	for(it = partition.begin(); it != partition.end(); it++){
		myfile << it->first << ";" << it->second << endl;
	}

	myfile.close();

}