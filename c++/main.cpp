#include "header.h"
#include <time.h> 

int main(int argc, char **argv){

	srand(time(NULL));

	string fname = argv[1];
	int cooling_schedule = atoi(argv[2]);

	
	// MAIN CODE

	int N = 0;

	map<int,int> partition;
	map<int,int> new_partition;
	map<int,int> best_partition;
	map<int,int> true_partition;

	map<uint32_t, int> data = get_data(N, fname);

	// INITIAL PARTITION
	for(int i = 0; i < n; i++){
		partition[i] = i;
	}

	best_partition = partition;
	true_partition = partition;

	double logE = evidence(partition, data, N);
	double true_logE = logE;

	cout << logE << endl;
	double best_logE = logE;
	double new_logE, delta_logE;
	double p, u;

	// cooling schedule parameters
	float T = 100, T0 = 100;
	int L = 50;
	float a = 0.0001;

	int tot_its = 0;

	

	

	int rn = 0; // number of runs without improvement

	for (int run = 0; run < 100; run++){

		cout << "RUN: " << run << " ====================" << endl;

		T = T0;
		partition = best_partition;
		logE = best_logE;
		int na = 0; // number of accepted proposals this run
		int nc = 0; // number of steps without improvement this run
		int step = 0;

		if (rn > 1) {continue;}

		while ((step < max_steps) && (nc < 50000)){

			tot_its++;

			// PICK RANDOM CANDIDATE FUNCTION
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

			// EVALUATE NEW PARTITION 
			new_logE = evidence(new_partition, data, N);
			delta_logE = new_logE - logE;

			if (new_logE > best_logE){
				best_logE = new_logE;
				best_partition = new_partition;
				cout << "RUN: " << run << " " << T << " " << best_logE << " " << nc << " ";
				partition_print(new_partition);
				nc = 0;
				rn = 0;
			} else {
				nc += 1;
			}

			// METROPOLIS STEP
			p = exp(delta_logE/T);
			u = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

			if (p > u){
				na += 1;
				partition = new_partition;
				logE = new_logE;
			}

			// UPDATE TEMPERATURE

			// COOLING SCHEDULES:

			if (na > L){

				switch(cooling_schedule){

					case 0: // linear
						T = T0 * (1 - (float)step/(float)max_steps); 
						break;

					case 1: // logarithmic
						T = T0 / (1 + log(1 + step));
						break;

					case 2: // quadratic multiplicative
						T = T0 / (1 + a * step * step);
						break;

				}

			}

			
			
			

			step++;

		}

		rn += 1;
		cout << rn << endl;
	}

	cout << true_logE << " " << best_logE << " " << best_logE - true_logE << endl;
	cout << best_logE << endl;
	cout << tot_its << endl;

	// WRITE PARTITION TO FILE (OPTIONAL)
	//partition_write(best_partition, fname);

	// currently unused 
	//double dlogE = best_logE - true_logE;
	//double voi = get_voi(true_partition, best_partition);

	return 0;
}