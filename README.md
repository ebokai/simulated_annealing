# Simulated annealing community detection

Codebase for simulated annealing to find the best partition of binary variables based on the log-evidence of Minimally Complex Models.

Starting from a random (or pre-specified) partition of n variables into k communities, the algorithm explores the space around the initial partition by proposing elementary modifications. 

1. Merging two communities 
2. Splitting a community in two
3. Switching a node between partitions

Note: the python version is mainly a proof-of-concept of the algorithm. The optimized algorithm is coded in C++.
