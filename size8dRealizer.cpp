// IMPORTAANT:  if you have a graph with a d-rep, any subgraph of it also has a d-rep (?) though it may not even have enough
// edges for a schnyder wood.  so sufficient to just look at the 'larger' graphs that do have schyder woods.
// Not true, graphs with d-reps are essentially those 'induced by it', namely all edges that can be there are there.


#include <vector>
#include <bitset>
#include <set>
#include <iostream>
#include <stack>
#include <cstring>
#include <map>

using namespace std;

// n = |N(G)|
const int n = 7;

//typedef bitset<28> edges;
const int m = (n)*(n-1)/2;
typedef bitset<n> nodes;

struct edges{
	bitset<m> e;
	int root;

	edges(bitset<m> e) : e(e), root(-1) {}
	edges(bitset<m> e, int root) : e(e), root(root) {}

	const bool operator <(const edges& other) const {
		return this->e.to_ulong() < other.e.to_ulong();
	}
};

struct edgeTuple{
	edges edges1;
	edges edges2;
	edges edges3;
	edges edges4;

	edgeTuple(edges e1, edges e2, edges e3, edges e4) 
	: edges1(e1), edges2(e2), edges3(e3), edges4(e4) {}

	edges& operator[](const size_t& idx) {
		switch(idx) {
			case 0:
				return edges1;
				break;
			case 1:
				return edges2;
				break;
			case 2:
				return edges3;
				break;
			case 3:
				return edges4;
				break;
		}

		return edges1;
	}

	edges unionEdges() {
		return edges((edges1.e | edges2.e) | (edges3.e | edges4.e), edges1.root);
	}
};


struct edgesWithNodes{
	edges e;
	nodes nodesSeen;
	edgesWithNodes(edges e, nodes n) : e(e), nodesSeen(n) {}
	const bool operator<(const edgesWithNodes& other) const {
		if (this->e.e.to_ulong() != other.e.e.to_ulong()) {
			return (this->e.e.to_ulong() < other.e.e.to_ulong());
		} else {
			return this->nodesSeen.to_ulong() < other.nodesSeen.to_ulong();
		}
	}
};


int nodes2edges[n][n] = {{-1}};

set<edgesWithNodes> stHelper(int bonusNode, int nodesLeft, edgesWithNodes currEdgeSet) {
	if (nodesLeft == 0) {
		set<edgesWithNodes> STs;
		STs.insert(currEdgeSet);
		return STs;
	}
	set<edgesWithNodes> STs;
	for (int k=0; k<n; k++) {
		if (k>=n-4 && k != bonusNode) // the last 4 nodes are the special verticies, use only 1 at a time
			continue;
		if (currEdgeSet.nodesSeen[k]) // want an edge from an unseen node
			continue;
		for (int j=0; j<n; j++) {
			if (j>=n-4 && j != bonusNode) // the last 4 nodes are the special verticies
				continue;
			if (!currEdgeSet.nodesSeen[j]) // to a seen node
				continue;

			int edge = nodes2edges[j][k];
			currEdgeSet.e.e[edge] = true;
			currEdgeSet.nodesSeen[k] = true;
			set<edgesWithNodes> STs2 = stHelper(bonusNode, nodesLeft-1, currEdgeSet);
			STs.insert(STs2.begin(), STs2.end());
			currEdgeSet.e.e[edge] = false;
			currEdgeSet.nodesSeen[k] = false;
		}
	}
	return STs;
}


vector<edges> getAllSpanningTrees(int n) {
	vector<edges> STvect;

	for (int i=0; i<4; i++) {
		edgesWithNodes starter(edges(0,n-4+i),0); // defining the root nodes here
		starter.nodesSeen[n-4+i] = true;
		set<edgesWithNodes> STset = stHelper(n-4+i, n-4, starter);
		set<edges> STsetReduced;
		set<edgesWithNodes>::iterator it = STset.begin();
		while (it != STset.end()) {
			STsetReduced.insert((*it).e);
			it++;
		}
		set<edges>::iterator it2 = STsetReduced.begin();
		while (it2 != STsetReduced.end()) {
			STvect.push_back(*it2);
			it2++;
		}
	}
	return STvect;
}

bool DAG[4][n][n] = {{{0}}};
bool DAGComb[4][n][n] = {{{0}}};
int visited[n] = {0};

void constructDAG(int j, edges edgeSet) {
	memset(visited, 0, sizeof(visited));
	stack<pair<int,int> > next;
	int start = edgeSet.root;
	next.push(make_pair(start,-1)); 
	while (!next.empty()) {
		pair<int,int> node = next.top();
		next.pop();
		if (visited[node.first]) continue;
		visited[node.first] = true;
		for (int i=0; i<n; i++) {
			if (node.first != i && edgeSet.e[nodes2edges[node.first][i]] && i != node.second) {
				next.push(make_pair(i, node.first));
				DAG[j][node.first][i] = 1;
			}
		}
	}	
}

void printDAG(int j) {
	cout << "{";
	for (int i=0; i<n; i++) {
		cout << "{";
		for (int k=0; k<n; k++) {
			cout << DAG[j][i][k];
			if (k<n-1) cout << ",";
		}
		cout << "}";
		if (i<n-1) cout <<"," << endl;
	}
	cout << "}," << endl << endl;
}

void printDAGComb(int j) {
	cout << "=========================" << endl;
	for (int i=0; i<n; i++) {
		for (int k=0; k<n; k++) {
			cout << DAGComb[j][i][k] << " ";
		}
		cout << endl;
	}
	cout << "=========================" << endl;
}

int visitedW[n] = {0};
int visitedB[n] = {0};

bool hasCycles(edges edgeSet, int node) {
	if (visitedB[node]) {
		return false;
	}
	if (visitedW[node]) {
		return true;
	}
	visitedW[node] = true;
	for (int i=0; i<n; i++) {
		if (DAGComb[0][node][i]) {
			if (hasCycles(edgeSet, i))
				return true;
		}
	}
	visitedB[node] = true;
	return false;
}

// TODO: check the other 3 ways of combining the dags work, 
// also check the other constraints on the permutations...
bool treesDontMakeCycles(edgeTuple spanningTreeQuad) {
	memset(DAG, 0, sizeof(DAG));
	memset(DAGComb, 0, sizeof(DAGComb));

	// construct all the DAGS
	for (int j=0; j<4; j++) {
		constructDAG(j, spanningTreeQuad[j]);
		//printDAG(j);
	}

	bool loopExists = false;
	// this loops over which tree gets flipped
	for (int i=0; i<4; i++) {

		// combine all other DAGS, not reversed
		for (int j=0; j<4; j++) {
			if (j==i) continue;
			for (int k1=0; k1<n; k1++) {
				for (int k2=0; k2<n; k2++) {
					if (DAG[j][k1][k2]) {
						DAGComb[0][k1][k2] = 1;
					}
				}
			}
		}

		// make sure the reversed one is in there too
		for (int k1=0; k1<n; k1++) {
			for (int k2=0; k2<n; k2++) {
				if (DAG[i][k1][k2]) {
					DAGComb[i][k2][k1] = 1;
				}
			}
		}

		// check for cycles!	
		memset(visitedW, 0, sizeof(visited));
		memset(visitedB, 0, sizeof(visited));			
		edges edgeSet = spanningTreeQuad[0];
		int start = edgeSet.root;			
		loopExists |= hasCycles(edgeSet, start);
	}

	if (loopExists) {
		//cout << "found a cycle" << endl;
		//printDAGComb(0);
		return false;
	}
	return true;
}


int main() {
	vector<int> specialNodeCliqueEdges;

	memset(nodes2edges, -1, sizeof(nodes2edges));
	int k = 0;
	for (int i=0; i<n; i++) {
		for (int j=0; j<i; j++) {
			nodes2edges[i][j] = k;
			if (i >= n-4 && j >= n-4) specialNodeCliqueEdges.push_back(k);
			k++;
		}
	}
	for (int i=0; i<n; i++) {
		for (int j=i+1; j<n; j++) {
			nodes2edges[i][j] = nodes2edges[j][i];
		}
	}

	// compute all spanning trees 
	// but only on n - 3 nodes becuase the other 3 notes are 
	// the other 3 external/whateveritscalled/support? verticies
	vector<edges> spanningTrees = getAllSpanningTrees(n);
	cout << "there are " << spanningTrees.size() << " spanning trees" << endl;

	// compute all quadruples of spanning trees
	vector<edgeTuple> spanningTreeQuads;
	// no idea how much space this needs ...
	spanningTreeQuads.reserve(1000000);

	for (int i=0; i<spanningTrees.size(); i++) {

		for (int j=0; j<i; j++) {
			if ((spanningTrees[i].e & spanningTrees[j].e) != edges(0).e) continue;
			if (spanningTrees[i].root == spanningTrees[j].root) continue;

			for (int k=0; k<j; k++) { // this might be 2 slow
				bitset<m> intersection = (spanningTrees[i].e & spanningTrees[k].e) | (spanningTrees[j].e & spanningTrees[k].e);
				if (intersection != edges(0).e) 
					continue;
				if (spanningTrees[i].root == spanningTrees[k].root || spanningTrees[j].root == spanningTrees[k].root)
					continue;

				for (int l=0; l<k; l++) {
					bitset<m> intersectionall = (spanningTrees[i].e & spanningTrees[l].e) | (spanningTrees[j].e & spanningTrees[l].e)
											| (spanningTrees[k].e & spanningTrees[l].e);
					if (intersectionall != edges(0).e) 
						continue;
					if (spanningTrees[i].root == spanningTrees[l].root || spanningTrees[j].root == spanningTrees[l].root || spanningTrees[k].root == spanningTrees[l].root) 
						continue;

					edgeTuple newEdgesQuad(spanningTrees[i], spanningTrees[j], spanningTrees[k], spanningTrees[l]);
					spanningTreeQuads.push_back(newEdgesQuad);
					// memset(DAG, 0, sizeof(DAG));
					// constructDAG(0, newEdgesQuad.unionEdges());
					// printDAG(0);

				}				
			}
		}
	}
	
	cout << "there are " << spanningTreeQuads.size() << " quadruples of spanning trees" << endl;


	// get a map from each edges set of the graph to all the quadrupes that it can decompose to
	// use this to get a map from each edge set to to the number of working decompositions it has

	map<edges, vector<edgeTuple> > edges2quadruples;
	for (int i=0; i<spanningTreeQuads.size(); i++) {
		if (edges2quadruples.count(spanningTreeQuads[i].unionEdges())) {
			edges2quadruples[spanningTreeQuads[i].unionEdges()].push_back(spanningTreeQuads[i]);
		} else {
			edges2quadruples[spanningTreeQuads[i].unionEdges()] = vector<edgeTuple>(1,spanningTreeQuads[i]);
		}
	}

	map<edges, pair<int,int> > edges2decomps; // first = num 'working' decomps, 2nd = num not working

	for (int i=0; i<spanningTreeQuads.size(); i++) {
		edges e = spanningTreeQuads[i].unionEdges();

		if (!edges2decomps.count(e)) {
			edges2decomps[e] = make_pair(0,0);
		}

		if (treesDontMakeCycles(spanningTreeQuads[i])) {
			edges2decomps[e].first++;
		} else { 
			edges2decomps[e].second++;
		}
	}


	cout << "a = {";
	map<edges, pair<int,int> >::iterator it = edges2decomps.begin();
	while (it != edges2decomps.end()) {
		if (it->second.first == 0) {
			memset(DAG, 0, sizeof(DAG));
			constructDAG(0, it->first);
			printDAG(0);
		}
		//cout << (it->second.first) << " " << (it->second.second) << endl;
		it++;
	}
	cout << "}" << endl;

	return 0;
}