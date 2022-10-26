import numpy as np 

def merge(p):

	x = np.arange(len(p))

	if len(p) < 2: return p

	p1, p2 = sorted(np.random.choice(x, 2, replace = False))

	mp = p[p1] + p[p2]

	del p[p2]
	del p[p1]

	p.append(mp)

	return p

def split(p):

	p1 = p[np.random.randint(len(p))]

	if len(p1) < 2: return p 

	p.remove(p1)

	s = np.random.randint(1,len(p1))

	p1x = p1[s:]
	p1y = p1[:s]

	p.append(p1x)
	p.append(p1y)

	return p

def switch(p):

	x = np.arange(len(p))

	if len(p) < 2: return p

	p1, p2 = sorted(np.random.choice(x, 2, replace = False))

	n1 = np.random.choice(p[p1])

	if len(p[p1]) < 2: return p 

	p[p1].remove(n1)
	p[p2].append(n1)

	return p

def sort(p):

	p = [sorted(x[:]) for x in p]

	return p 