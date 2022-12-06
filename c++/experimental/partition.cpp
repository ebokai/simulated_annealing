#include "header.h"

Partition random_partition(Partition &p_struct){

	// vector<pair<uint64_t, double>> partition;
	uint64_t community; // candidate community
	uint64_t assigned = 0; // assigned nodes

	int i = 0;
	unsigned int community_size;
	
	while(bitset<n>(~assigned).count() > 0){
		community = rand();
		community = (bitset<n>(community)).to_ulong();
		community = community - (assigned & community); // remove already assigned nodes from candidate
		assigned += community; // add newly assigned nodes
		community_size = bitset<n>(community).count();

		// avoid adding empty communities
		if (community_size > 0){
			p_struct.current_partition[i] = community;
			p_struct.partition_evidence[i] = icc_evidence(community, p_struct);
			p_struct.current_log_evidence += p_struct.partition_evidence[i];

			p_struct.occupied_partitions += (1 << i);
			if (community_size > 1){
				p_struct.occupied_partitions_gt2_nodes += (1 << i);
			}
			cout << "new community: " << bitset<64>(community) << endl;
			cout << "log-evidence: " << p_struct.partition_evidence[i] << endl;
			cout << "assigned nodes: " << bitset<n>(assigned) << endl;
			cout << endl;
			p_struct.nc++;
			i++;
		}
	}

	p_struct.best_log_evidence = p_struct.current_log_evidence;

	cout << "generated " << p_struct.nc << " communities" << endl;
 
	return p_struct;
}


Partition merge_partition(Partition &p_struct){

	//cout << "===== MERGE =====" << endl;

	//cout << "GT2 (MS): " << bitset<n>(p_struct.occupied_partitions_gt2_nodes) << endl;

	if (p_struct.nc < 2){return p_struct;}

	unsigned int p1 = randomBitIndex(p_struct.occupied_partitions);
	unsigned int p2 = p1;
	while (p1 == p2){
		p2 = randomBitIndex(p_struct.occupied_partitions);
	}

	uint64_t merged_community = p_struct.current_partition[p1] + p_struct.current_partition[p2];
	double merged_evidence = icc_evidence(merged_community, p_struct);
	double delta_log_evidence = merged_evidence - p_struct.partition_evidence[p1] - p_struct.partition_evidence[p2];
	double p = exp(delta_log_evidence/p_struct.T);
	double u = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

	//cout << p1 << " " << bitset<n>(p_struct.current_partition[p1]) << endl;
	//cout << p2 << " " << bitset<n>(p_struct.current_partition[p2]) << endl;
	//cout << p1 << " " << bitset<n>(merged_community) << endl;
	
	if (p > u){
		//cout << "MERGE ACCEPTED" << endl;
		//cout << p1 << " " << p2 << " ==> " << p1 << endl;
		p_struct.current_partition[p1] = merged_community;
		p_struct.current_partition[p2] = 0;
		p_struct.partition_evidence[p1] = merged_evidence;
		p_struct.partition_evidence[p2] = 0;
		p_struct.occupied_partitions -= (one << p2);
		p_struct.current_log_evidence += delta_log_evidence;

		if ((p_struct.occupied_partitions_gt2_nodes & (one << p2)) == (one << p2)){
			p_struct.occupied_partitions_gt2_nodes -= (one << p2);
		}
		
		if ((p_struct.occupied_partitions_gt2_nodes & (one << p1)) == 0){
			p_struct.occupied_partitions_gt2_nodes += (one << p1);
		}
		
		p_struct.nc -= 1;
	}

	//cout << "GT2 (ME): " << bitset<n>(p_struct.occupied_partitions_gt2_nodes) << endl;
	
	return p_struct;
}

Partition split_partition(Partition &p_struct){

	//cout << "===== SPLIT =====" << endl;

	//cout << "GT2 (SS): " << bitset<n>(p_struct.occupied_partitions_gt2_nodes) << endl;

	if (p_struct.nc == n){return p_struct;}
	if (p_struct.occupied_partitions_gt2_nodes == 0){return p_struct;}

	unsigned int p1 = randomBitIndex(p_struct.occupied_partitions_gt2_nodes);
	uint64_t community = p_struct.current_partition[p1];

	uint64_t mask = rand();
	uint64_t c1 = (community & mask);
	uint64_t c2 = (community & (~mask));


	while ((bitset<n>(c1) == 0) || (bitset<n>(c2) == 0)){
		mask = rand();
		c1 = (community & mask);
		c2 = (community & (~mask));
	}

	//cout << p1 << " " << bitset<n>(community) << endl;
	//cout << p1 << " " << bitset<n>(c1) << endl;
	

	double log_evidence_1 = icc_evidence(c1, p_struct);
	double log_evidence_2 = icc_evidence(c2, p_struct);
	double delta_log_evidence = log_evidence_1 + log_evidence_2 - p_struct.partition_evidence[p1];
	double p = exp(delta_log_evidence/p_struct.T);
	double u = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

	if (p > u){
		p_struct.current_partition[p1] = c1;
		p_struct.partition_evidence[p1] = log_evidence_1;
		
		bitset<n> not_occupied(~p_struct.occupied_partitions);
		uint64_t not_occupied_int = not_occupied.to_ulong();
		unsigned int p2 = randomBitIndex(not_occupied_int);
		//cout << p2 << " " << bitset<n>(c2) << endl;

		p_struct.current_partition[p2] = c2;
		p_struct.partition_evidence[p2] = log_evidence_2;
		p_struct.occupied_partitions += (one << p2);
		p_struct.current_log_evidence += delta_log_evidence;

		if (bitset<n>(c1).count() < 2){
			p_struct.occupied_partitions_gt2_nodes -= (one << p1);
		}

		if (bitset<n>(c2).count() > 1){
			p_struct.occupied_partitions_gt2_nodes += (one << p2);
		}

		//cout << "SPLIT ACCEPTED" << endl;
		//cout << p1 << " ==> " << p1 << " " << p2 << endl;

		p_struct.nc += 1;


	}

	//cout << "GT2 (SE): " << bitset<n>(p_struct.occupied_partitions_gt2_nodes) << endl;

	return p_struct;
}

Partition switch_partition(Partition &p_struct){

	//cout << "===== SWITCH =====" << endl;
	//cout << "GT2 (SWS): " << bitset<n>(p_struct.occupied_partitions_gt2_nodes) << endl;

	if (p_struct.nc < 2){return p_struct;}
	if (p_struct.nc == n){return p_struct;}
	if (p_struct.occupied_partitions_gt2_nodes == 0){return p_struct;}

	unsigned int p1 = randomBitIndex(p_struct.occupied_partitions_gt2_nodes);
	uint64_t c1 = p_struct.current_partition[p1];

	//cout << p1 << " " << bitset<n>(p_struct.current_partition[p1]) << endl;
	//cout << bitset<64>(p_struct.current_partition[p1]) << endl;
	unsigned int node = randomBitIndex(c1);

	unsigned int p2 = p1;
	while (p1 == p2){
		p2 = randomBitIndex(p_struct.occupied_partitions);
	}

	//cout << p2 << " " << bitset<n>(p_struct.current_partition[p2]) << endl;

	uint64_t c2 = p_struct.current_partition[p2];

	uint64_t new_c1 = c1 - (one << node);
	uint64_t new_c2 = c2 + (one << node);

	//cout << p1 << " " << bitset<n>(new_c1) << endl;
	//cout << p2 << " " << bitset<n>(new_c2) << endl;

	double log_evidence_1 = icc_evidence(new_c1, p_struct);
	double log_evidence_2 = icc_evidence(new_c2, p_struct);
	double delta_log_evidence = log_evidence_1 + log_evidence_2 - p_struct.partition_evidence[p1] - p_struct.partition_evidence[p2];
	double p = exp(delta_log_evidence/p_struct.T);
	double u = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

	if (p > u){
		//cout << "SWITCH ACCEPTED" << endl;
		p_struct.current_partition[p1] = new_c1;
		p_struct.current_partition[p2] = new_c2;
		p_struct.partition_evidence[p1] = log_evidence_1;
		p_struct.partition_evidence[p2] = log_evidence_2;
		p_struct.current_log_evidence += delta_log_evidence;

		if ((bitset<n>(c1).count() > 1) && (bitset<n>(new_c1).count() < 2)){
			p_struct.occupied_partitions_gt2_nodes -= (one << p1);
		} 

		if ((bitset<n>(c2).count() < 2) && (bitset<n>(new_c2).count() > 1)){
			p_struct.occupied_partitions_gt2_nodes += (one << p2);
		} 

	}

	//cout << "GT2 (SWE): " << bitset<n>(p_struct.occupied_partitions_gt2_nodes) << endl;

	return p_struct;
}

