from scipy.special import gammaln
import numpy as np 

def mcm_evidence(data, partition):

	# data should be an N x n array
	# with states in "list" format, i.e.
	# data = [[0,0,1],[1,1,1],[1,0,0],...]

	logE = 0

	N = np.alen(data)

	for p in partition:

		ra = len(p)

		if ra == 0: continue
		
		data_p = data[:,p]
		data_p = [''.join([str(s) for s in state]) for state in data_p]

		u,c = np.unique(data_p, return_counts = True)

		pf = gammaln(2**(ra-1)) - gammaln(N + 2**(ra-1))
		gc = pf + np.sum(gammaln(c + 1/2) - 1/2 * np.log(np.pi))

		logE += gc

	return logE

data = np.loadtxt('./data/J0_mu0_nw0_dat0.dat', dtype=str)
data = np.array([[int(s) for s in state] for state in data])

c = [1,1,4,2,4,2,3,2,1,4,1,3,1,3,2,3,4,2,3,4]

p = [[] for i in range(4)]
for i in range(20):
	p[c[i]-1].append(i)
print(p)

ev = mcm_evidence(data, p)
print('LOG E:',ev)


p = [[] for i in range(4)]
for i in range(20):
	p[c[i]-1].append(19-i)
print(p)

ev = mcm_evidence(data, p)
print('LOG E:',ev)
