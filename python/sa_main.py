# to do: implement VOI between comms

from candidates import merge, split, switch, sort
from evidence import mcm_evidence
from partitions import random_partition
import numpy as np 

# LOAD DATA ===================================
fname = f'HG19_k6_mu1.00_(20,4,5)_B0.55_N1000_00'
data = np.loadtxt(f'./data/{fname}.dat', dtype=str)
data = np.array([[int(s) for s in state] for state in data])
# =============================================

# INITIAL PARTITION ===========================
partition = [[1, 2, 7, 12, 0, 15, 16, 5, 13], [8], [6, 3, 10, 19, 18, 11, 4, 17, 9, 14]]
# partition = random_partition(20, 0.2)

partition = [[0,4,8,12,16],[1,5,9,13,17],[2,6,10,14,18],[3,7,11,15,19]]
logE = mcm_evidence(data, partition)

print(logE)

best_partition = partition
best_logE = logE
# =============================================

# PARAMETERS ==================================
max_an_steps = 1000 # annealing schedule steps
T0 = 100 # initial temperature
runs = 50 # number of annealing runs
# =============================================

# CANDIDATE FUNCTIONS =========================
candidate_functions = [merge, split, switch]
# =============================================

# MAIN CODE ===================================
if __name__ == '__main__':

	for run in range(runs):

		print(f'RUN: {str(run).zfill(2)} ==================')

		T = T0

		partition = best_partition
		logE = best_logE

		an_step = 0
		steps_since_improvement = 0

		while an_step < max_an_steps:

			# CREATE PARTITION CANDIDATE
			i = np.random.randint(len(candidate_functions))
			candidate_function = candidate_functions[i]
			partition_copy = [x[:] for x in partition]
			new_partition = candidate_function(partition_copy)

			# EVALUATE NEW PARTITION
			new_logE = mcm_evidence(data, new_partition)
			delta_logE = new_logE - logE

			# CHECK FOR IMPROVEMENT IN LOG E
			if new_logE > best_logE:
				logE_change = new_logE - best_logE
				best_logE = new_logE
				best_partition = new_partition
				print(f'RUN: {str(run).zfill(2)}\tT: {T:.2f}\tLOG E: {new_logE:.2f}\tdLOG E: {logE_change:.2f}\t{steps_since_improvement}\t', partition)
				steps_since_improvement = 0
			else:
				steps_since_improvement += 1

			# METROPOLIS STEP 
			p = np.exp(delta_logE/T)
			if p > np.random.random():
				partition = new_partition
				logE = new_logE

			# UPDATE TEMPERATURE
			T = T0 * (1 - an_step/max_an_steps)
			an_step += 1




