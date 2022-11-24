#include "header.h"
#include <time.h>

int main(int argc, char **argv){

	// initialize random seed
	srand(time(NULL));

	// parse filename
	string fname = argv[1];

	// declare variables
	int N = 0;
	map<int,int> partition;
	map<int,int> new_partition;
	map<int,int> true_partition;
	map<int,int> best_partition;

	double logE, new_logE, true_logE, best_logE, delta_logE;
	double p, u;
	int step = 0;
	int steps_since_improve = 0;

	float T0 = 100;
	float T = T0;
	int update_schedule = 100;

	// read dataset
	map<uint32_t, int> data = get_data(N, fname);

	// initialize partitions
	true_partition = fixed_partition(4);
	partition = random_partition();

	logE = evidence(partition, data, N);
	true_logE = evidence(true_partition, data, N);
	best_logE = logE;

	cout << "TRUE LOG E: " << true_logE << endl;
	cout << "INITIAL LOG E: " << best_logE << endl;
	partition_print(partition);

	while (step < max_steps){

		// pick random candidate function ---------------------------
		int f = rand()/(RAND_MAX/3);
		switch(f){
		case 0:
			new_partition = merge_partition(partition);
			break;
		case 1:
			new_partition = split_partition(partition);
			break;
		case 2:
			new_partition = switch_partition(partition);
			break;
		}

		// evaluate new partition -----------------------------------
		new_logE = evidence(new_partition, data, N);
		delta_logE = new_logE - logE;

		// keep track of current best solution ----------------------
		if (new_logE > best_logE){
			best_logE = new_logE;
			best_partition = new_partition;
			cout << step << " " << T << " " << best_logE << " ";
			partition_print(new_partition);
			steps_since_improve = 0;
		} else {
			steps_since_improve++;
		}

		// metropolis step ------------------------------------------
		p = exp(delta_logE/T);
		u = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

		if (p > u){
			partition = new_partition;
			logE = new_logE;
		}

		// cooling schedule -----------------------------------------
		if (step % update_schedule == 0){
			T = T0 / (1 + log(1 + step));
		}

		step++;
	}




	return 0;
}