#include "header.h"
#include <time.h> 

int main(int argc, char **argv){

	srand(time(NULL));

	

	string kavg = argv[1];
	string mu = argv[2];
	string beta = argv[3];

	string ii, jj, fpart;

	for (int i = 0; i < 20; i++){
		ii = to_string(i);
		if (ii.length() == 1){ii = "0" + ii;}
		for (int j = 0; j < 20; j++){
			jj = to_string(j);
			if (jj.length() == 1){jj = "0" + jj;}
			string fpart = "HG" + ii + "_k" + kavg + "_mu" + mu + "_(20,4,5)_B" + beta + "_N1000_" + jj;
			cout << fpart << endl;

			string fout = "./sa/k" + kavg + "_mu" + mu + "_B" + beta + "SA_stats.txt";

			
			// MAIN CODE

			int N = 0;
	
			map<int,int> partition;
			map<int,int> new_partition;
			map<int,int> best_partition;
			map<int,int> true_partition;

			map<uint32_t, int> data = get_data(N, fpart);

			// INITIAL PARTITION
			for(int i = 0; i < n; i++){
				partition[i] = i / 5;
			}

			best_partition = partition;
			true_partition = partition;

			double logE = evidence(partition, data, N);
			double true_logE = logE;

			cout << logE << endl;
			double best_logE = logE;
			double new_logE, delta_logE;
			double p, u;
			float T = 100, T0 = 100;

			int rn = 0;

			for (int run = 0; run < 100; run++){

				// cout << "RUN: " << run << " ====================" << endl;

				T = T0;
				partition = best_partition;
				logE = best_logE;
				int na = 0;
				int nc = 0;
				int step = 0;

				if (rn > 20) {continue;}



				while ((step < max_steps) && (nc < 500)){

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
					T = T0 * (1 - (float)step/(float)max_steps);
					step++;

				}

				rn += 1;
				cout << rn << endl;
			}

			cout << true_logE << " " << best_logE << " " << best_logE - true_logE << endl;
			double dlogE = best_logE - true_logE;
			double voi = get_voi(true_partition, best_partition);

			ofstream outfile; 
			outfile.open(fout, std::ios_base::app);
			outfile << voi << ";" << true_logE << ";" << best_logE << ";" << dlogE << endl;
		}
	}



	return 0;
}