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
	

	pStats partition_stats;
	partition_stats.fname = fname;

	double logE, new_logE, delta_logE;
	double p, u;
	int step = 0;
	int steps_since_improve = 0;
	int max_no_improve = 10000;

	float T0 = 100;
	float T = T0;
	int update_schedule = 100;

	// read dataset
	map<uint32_t, int> data = get_data(N, fname);

	// initialize partitions
	true_partition = fixed_partition(4);
	partition = random_partition();

	logE = evidence(partition, data, N);
	partition_stats.best_logE = logE;
	partition_stats.best_partition = partition;
	partition_stats.true_logE = evidence(true_partition, data, N);

	cout << "TRUE LOG E: " << partition_stats.true_logE << endl;
	cout << "INITIAL LOG E: " << partition_stats.best_logE << endl;
	cout << "INITIAL PARTITION: ";
	partition_print(partition);
	cout << endl;

	while (step < max_steps){

		if (steps_since_improve > max_no_improve) {
			cout << "No improvement in log-evidence for " << max_no_improve << " iterations. Stopping." << endl;
			break;
		}

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
		if (new_logE > partition_stats.best_logE){
			partition_stats.best_logE = new_logE;
			partition_stats.best_partition = new_partition;
			cout << step << " " << T << " " << partition_stats.best_logE << " ";
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

	partition_stats.voi = get_voi(true_partition, partition_stats.best_partition);
	cout << endl;
	cout << "FINAL LOG E: " << partition_stats.best_logE << endl;
	cout << "DELTA LOG E: " << partition_stats.best_logE - partition_stats.true_logE << endl;
	cout << "VOI: " << partition_stats.voi << endl;

	partition_write(partition_stats);
	write_statistics(partition_stats);




	return 0;
}