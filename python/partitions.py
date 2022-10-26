import numpy as np


def random_partition(n, p):

	partition = [[]]
	
	for i in range(n):

		partition[-1].append(i)
		
		u = np.random.random()
		
		if u < p and i != n-1:
			partition.append([])
	
	return partition