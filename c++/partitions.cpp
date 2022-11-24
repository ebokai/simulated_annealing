#include "header.h"


bool compare(const pair<int, int>&a, const pair<int, int>&b){
   return a.second<b.second;
}

int rand_ab(int a, int b){
	// return a or b
	int x = rand()/(RAND_MAX/2);
	return a + x * (b - a);
}

int max_comm(map<int,int> partition){

	// return maximum community index
	// for counting communities
	
	int maxc = 0;

	for (int i = 0; i < n; i++)
	{
		if(partition[i] > maxc)
		{
			maxc = partition[i];
		}
	}
	return maxc;
}

// FIXED PARTITION
map<int, int> fixed_partition(int nc){

	map<int, int> partition;
	int npc = n / nc;
	for (int i = 0; i < n; i++){
		partition[i] = i / npc;
	}

	return partition;
}


// RANDOM PARTITION
map<int, int> random_partition(){

	map<int,int> partition;

	int k = 0;
	int c;
	for (int i = 0; i < n; i++){
		c = rand() / (RAND_MAX/(k + 1));
		partition[i] = c;
		if ((c + 1) > k){
			k++;
		}
	}
	return partition;

}


// MERGE PARTITIONS 
map<int, int> merge_partition(map<int,int> partition){
	int np = max_comm(partition) + 1;
	if (np < 2){return partition;}

	int p1 = 0, p2 = 0;
	int pi;
	map<int, int> new_partition;

	// pick two distinct communities
	while (p1 == p2)
	{
		p1 = rand() / (RAND_MAX/np);
		p2 = rand() / (RAND_MAX/np);
	}

	int plow, phigh;

	if (p1 > p2){
		phigh = p1;
		plow = p2;
	} else {
		phigh = p2;
		plow = p1;
	}

	// merging step
	for (int i = 0; i < n; i++){
		pi = partition[i];
		if (pi == phigh){
			new_partition[i] = plow;
		} else {
			new_partition[i] = pi;
		}
	}

	// update community labels 
	for (int i = 0; i < n; i++){
		pi = new_partition[i];
		if (pi > phigh){
			new_partition[i] -= 1;
		}
	}

	return new_partition;
}

// SPLIT PARTITIONS
map<int, int> split_partition(map<int,int> partition){

	int np = max_comm(partition) + 1;
	
	// pick a random community 
	int p1 = rand()/(RAND_MAX/np);
	int pi;

	// check if community contains at least 2 nodes 	
	int np1 = 0;
	for (int i = 0; i < n; i++){
		if (partition[i] == p1){
			np1 += 1;
		}
	}

	if (np1 < 2) {return partition;}
	map<int, int> new_partition;

	int nx = 0;

	// randomly assign nodes to new community 
	for (int i = 0; i < n; i++){
		pi = partition[i];

		if (pi == p1){
			if (nx == 0){
				// keep at least one node in original community
				new_partition[i] = p1;
				nx += 1;
			} else {
				new_partition[i] = rand_ab(p1, np);
			}
			
		} else {
			new_partition[i] = pi;
		}
	}

	return new_partition;
}

// SWITCH NODES BETWEEN COMMS 
map<int, int> switch_partition(map<int,int> partition){

	int np = max_comm(partition) + 1;
	if (np < 2) {return partition;}

	// pick a node ---------------------
	int node = rand()/(RAND_MAX/n);
	int pn = partition[node];
	// ---------------------------------

	// prevent emptying partition ------
	int nx = 0;
	for (int i = 0; i < n; i++){
		if(partition[i] == pn){
			nx += 1;
		}
	}

	if (nx == 1){return partition;}
	// ---------------------------------

	// pick new community for node -----
	int p1 = rand()/(RAND_MAX/np);
	while (p1 == pn){
		p1 = rand()/(RAND_MAX/np);
	} 
	partition[node] = p1;
	// ---------------------------------

	return partition;
}