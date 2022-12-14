#include "header.h"
#include <time.h>
#include <ctime> 
#include <ratio>
#include <chrono>

int main(int argc, char **argv) {

	random_device randDevice;
    srand(randDevice());

    string fname = argv[1];

    Partition p_struct;

    p_struct = get_data(fname, p_struct);
    // p_struct = load_partition(p_struct, fname);
    p_struct = random_partition(p_struct);

    // SHOULD TEST IF INDEPENDENT INITIAL PARTITION DOESN'T GIVE ISSUES


    unsigned int f;
    double T0 = 100;
    unsigned int update_schedule = 100;
    unsigned int iterations = 0;
    unsigned int steps_since_improve = 0;
	unsigned int max_no_improve = 10000;

    p_struct.T = T0;

    auto start = chrono::system_clock::now();

    for (int i = 0; i < 50000; i++){

    	iterations++;

    	if (p_struct.nc == n){
    		f = 0; // always merge if all independent communities
    	} else if (p_struct.nc == 1){
    		f = 1; // always split if one big community
    	} else {
    		f = rand()/(RAND_MAX/3);
    	}

		switch(f){
		case 0: 
			p_struct = merge_partition(p_struct);
			break;
		case 1:
			p_struct = split_partition(p_struct);
			break;
		case 2:
			p_struct = switch_partition(p_struct);
			break;
		}

		if (i % update_schedule == 0){
			p_struct.T = T0 / (1 + log(1 + i));
		}

		if ((p_struct.current_log_evidence > p_struct.best_log_evidence) && !(DoubleSame(p_struct.current_log_evidence, p_struct.best_log_evidence))){
			p_struct.best_log_evidence = p_struct.current_log_evidence;
			p_struct.best_partition = p_struct.current_partition;
			cout << "Best log-evidence: " << p_struct.current_log_evidence << "\t@T = " << p_struct.T << endl;
			steps_since_improve = 0;
		} else {
			steps_since_improve++;
		}

		if (steps_since_improve > max_no_improve){
			break;
		}

    }

    auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed = end - start;
	cout << "Iterations per second: " << static_cast <double> (iterations) / elapsed.count() << endl;

	for(int i = 0; i < n; i++){
		uint64_t community = p_struct.best_partition[i];
		if (bitset<n>(community).count() > 0){
			cout << bitset<n>(community) << endl;
		}
	}
    

    return 0;


}