#include "header.h"

map<uint32_t, int> get_data(int &N, string fname){

	map<uint32_t, int> data;
	uint32_t state;
	string fpath = "../data/set_02/" + fname + ".dat";
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

void partition_write(map<int, int> partition, string fname, string out_path){
	
	string fpath = "../sa_comms/" + out_path + "/" + fname + "_comms.dat";
	ofstream myfile(fpath);

	map<int, int>::iterator it;

	for(it = partition.begin(); it != partition.end(); it++){
		myfile << it->first << ";" << it->second << endl;
		
	}

	myfile.close();

}