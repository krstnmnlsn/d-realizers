from sets import Set

# accepts a permutation and constructs a corresponding set which 
# contains a tuple for each pair of indicies, ordered by their
# ordering in the permutation 
def orderSet(permut):
	setOfOrders = Set()
	for index, item1 in enumerate(permut):
		for item2 in permut[index+1:]:
			setOfOrders.add((item1, item2))
	return setOfOrders


# # test orderSet
# p = [4, 3, 2, 1]
# s = orderSet(p)
# print s

# ===================================================================
def checkOneOfD(i,j, permuts):
	foundOne = False # true when we find a permut with i < j
	for p in permuts:
		if foundOne:
			break
		if (p.index(i) < p.index(j)):
			foundOne = True
			permuts.remove(p)

	if not foundOne:
		return False

	for p in permuts:
		if (p.index(i) < p.index(j)):
			return False
	return True
# ===================================================================

# # test checkOneOfD
# perm1 = [1,2,3]
# perm2 = [3,2,1]
# perm3 = [3,1,2]
# print checkOneOfD(1,2, [perm1, perm2, perm3])


# ===================================================================
def checkEdgeProp(i,j, permuts, N):
	for k in range(N):
		k = k+1
		if k == i or k == j:
			continue
		kDone = False
		for p in permuts:
			if p.index(k) > max(p.index(i), p.index(j)):
				kDone = True
				break
		if not kDone:
			return False
	return True
# ===================================================================

# # test checkEdgeProp
# perm1 = [3,1,2]
# perm2 = [3,2,1]
# perm3 = [3,1,2]
# print checkEdgeProp(1,2, [perm1, perm2, perm3], 3)


# accepts a dimension d, a list of permutations (all must be of equal length), the 
# number of nodes in graph G and G itself in adjacency matrix form TODO:specify format.
# checks that the vertex and edge (also known as candidate pair property for all edges)
# are satisfied as well as the 1-of-d property for some dimension d, for each edge
def isdRealizer(d, permuts, N, G):
	# first check the vertex property
	combined = orderSet(permuts[0])
	for p in permuts:
		combined = combined.intersection(orderSet(p))
	if len(combined) > 0:
		print "the permutations given do not even have the vertex property fool."
		return False

	# for all possible edges, check if they are in G, 
	# if so check that they satisfy the edge property and the 1-of-d property
	# if not, check they satisfy neither of the edge or 1-of-d property
	for i in range(N):
		i = i+1
		for j in range(N):
			j = j+1
			isinG = G[i][j]
			oneOfD = checkOneOfD(i,j, permuts)
			edgeProperty = checkEdgeProp(i,j, permuts, N)
			if ((not isinG) and (oneOfD and edgeProperty)):
				return False
			if (isinG and not (oneOfD and edgeProperty)):
				return False
	return True

# # test isdRealizer
# perm1 = [1,2,3]
# perm2 = [3,2,1]
# perm3 = [3,1,2]

# node0 = {0:False, 1:True, 2:False}
# node1 = {0:False, 1:False, 2:True}
# node2 = {0:True, 1:False, 2:False}
# G = {0:node0, 1:node1, 2:node2}
# print isdRealizer(3, [perm1, perm2, perm3], 3, G)

perm1 = [1,3,4, 8,10,7, 6,9,5, 2]
perm2 = [4,1,2, 10,9,5, 7,8,6, 3]
perm3 = [4,2,3, 8,6,9,5, 10,7, 1]
perm4 = [1,2,3, 6,5,7, 8,9,10, 4]
permuts = [perm1, perm2, perm3, perm4]

node1 = {1:False 2:False 3:False 4:False 5:False 6:False 7:False 8:False 9:False 10:False}
node2 = {1:False 2:False 3:False 4:False 5:False 6:False 7:False 8:False 9:False 10:False}
node3 = {1:False 2:False 3:False 4:False 5:False 6:False 7:False 8:False 9:False 10:False}
node4 = {1:False 2:False 3:False 4:False 5:False 6:False 7:False 8:False 9:False 10:False}

node5 = {1:True 2:True 3:False 4:False 5:False 6:True 7:False 8:False 9:True 10:False}
node6 = {1:False 2:True 3:True 4:False 5:False 6:False 7:True 8:True 9:False 10:False}
node7 = {1:True 2:False 3:True 4:False 5:True 6:False 7:False 8:False 9:False 10:True}
node8 = {1: 2: 3: 4: 5: 6: 7: 8: 9: 10:}
node9 = {1: 2: 3: 4: 5: 6: 7: 8: 9: 10:}
node10 = {1: 2: 3: 4: 5: 6: 7: 8: 9: 10:}




