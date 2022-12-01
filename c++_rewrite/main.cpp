#include "header.h"
#include <time.h>
#include <ctime> 
#include <ratio>
#include <chrono>


int main(int argc, char **argv){

	srand(time(NULL));

	string fname = argv[1];
	int N = 0;
	int rd = 0;

	Partition p_struct;
	//p_struct.current_partition = fixed_partition(4, 5);
	p_struct.current_partition = random_partition();
	p_struct.data = get_data(N, fname);

 	// CALCULATE LOG E FOR GENERATED COMMUNITIES
	uint64_t community;
	for (unsigned int i = 0; i < p_struct.current_partition.size(); i++){
		community = p_struct.current_partition[i].first; 
		p_struct.current_partition[i] = make_pair(community, icc_evidence(community, p_struct.data, N));
		p_struct.current_logE += p_struct.current_partition[i].second;
		// cout << bitset<n>(p_struct.current_partition[i].first) << endl;
	}

	cout << "initial log-evidence: " << p_struct.current_logE << endl;

	double T0 = 100;
	int update_schedule = 100;
	int steps_since_improve = 0;
	int max_no_improve = 10000;
	int iterations = 0;
	p_struct.T = T0;
	p_struct.best_partition = p_struct.current_partition;
	p_struct.best_logE = p_struct.current_logE;

	cout << "==========" << endl;

	auto start = chrono::system_clock::now();

	for (int i = 0; i < 100000; i++){

		iterations++;

		if (steps_since_improve > max_no_improve) {
			cout << "No improvement in log-evidence for " << max_no_improve << " iterations. Stopping." << endl;
			break;
		}

		int f = rand()/(RAND_MAX/3);
		switch(f){
		case 0: 
			p_struct = merge_partition(p_struct, N);
			break;
		case 1:
			p_struct = split_partition(p_struct, N);
			break;
		case 2:
			p_struct = switch_partition(p_struct, N, rd);
		}


		if (p_struct.current_logE > p_struct.best_logE){
			p_struct.best_partition = p_struct.current_partition;
			p_struct.best_logE = p_struct.current_logE;
			cout << "best log-evidence: " << p_struct.best_logE << " @T = " << p_struct.T << endl;
			steps_since_improve = 0;
		} else {
			steps_since_improve++;
		}

		if (i % update_schedule == 0){
			p_struct.T = T0 / (1 + log(1 + i));
		}

	}

	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed = end - start;
	cout << "Elapsed time: " << elapsed.count() << "s" << endl;
	cout << "Iterations per second: " << static_cast <double> (iterations) / elapsed.count() << endl;

	for (unsigned int i = 0; i < p_struct.best_partition.size(); i++){
		community = p_struct.best_partition[i].first;
		cout << bitset<n>(community) << endl;
		}
	
	


	return 0;
}