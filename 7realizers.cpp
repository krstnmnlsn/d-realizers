#include <vector>
#include <iostream>
#include <stack>
#include <set>
#include <algorithm>
#include <bitset>
#include <cmath>
#include <queue>

using namespace std;

typedef vector<vector<int> > vvi;
typedef vector<vvi> vvvi;
typedef multiset<vector<int> > mvi;
typedef set<mvi> smvi;
typedef pair<int, int> pii;

// find a bunch of 4-realizers, on 7 nodes, and their graphs 
// THESE WILL NOT BE ALL THE ORDERS!! 


vvi genAllPermutes(int k) {
	vvi permutes;
	stack<vector<int> > unfinished;
	for (int i=1; i<=k; i++) unfinished.push(vector<int>(1,i));
	while (!unfinished.empty()) {
		vector<int> partialPermute = unfinished.top();
		unfinished.pop();

		bool numsUsedAlready[k+1];
		for (int i=1; i<=k; i++) {
			numsUsedAlready[i] = 0;
		}
		for (int i=0; i<partialPermute.size(); i++) {
			numsUsedAlready[partialPermute[i]]=1;
		}

		for (int i=1; i<=k; i++) {
			if (!numsUsedAlready[i]) {
				vector<int> extendedPermute = partialPermute;
				extendedPermute.push_back(i);
				if (extendedPermute.size() == k) {
					permutes.push_back(extendedPermute);
				} else {
					unfinished.push(extendedPermute);
				}
			}
		}
	}
	return permutes;
}


const int k=3;

bitset<k*k>	 constructIntersection(vector<int> p1, vector<int> p2, vector<int> p3) {
	// directed bitset, edge (x to y) represented as x*k + y
	bitset<k*k> p1set, p2set, p3set, intersection;

	for (int i=0; i<p1.size(); i++) {
		for (int j=0; j<i; j++) {
			p1set[(p1[j]-1)*k + (p1[i]-1)] = 1;
		}
	}

	for (int i=0; i<p2.size(); i++) {
		for (int j=0; j<i; j++) {
			p2set[(p2[j]-1)*k + (p2[i]-1)] = 1;
		}
	}

	for (int i=0; i<p3.size(); i++) {
		for (int j=0; j<i; j++) {
			p3set[(p3[j]-1)*k + (p3[i]-1)] = 1;
		}
	}

	//cout << p1set << '\n' << p2set << '\n' << p3set << endl;
	//intersection = p1set & (p2set & p3set);
	//cout << intersection << endl;

	return intersection;
}

void flipIntersect(bitset<k*k>& intersect) {
	bitset<k*k> reversed;
	for (int i=0; i<intersect.size(); i++) {
		if (intersect[i] == 1) {
			int first = floor(i/k);
			int second = i % k;
			reversed[second*k + first] = 1;
		}
	}

	intersect = reversed;
}

int findARoot(bitset<k*k> intersect, bitset<k> v) {
	for (int i=0; i<k; i++) {
		if (v[i]) continue;
		bool isRoot = true;
		for (int j=0; j<k; j++) {
			if (intersect[j*k + i]) isRoot = false;
		}
		if (isRoot) return i;
	}
	return -1;
}

// BFS through the DAG (defined by the intersect) indexing vertices as we see them 
// the resulting ordering of verticies is our total order
// note that the intersect must be acyclic as the subset of a (of 3 really) total orders
vector<int> constructPerm(bitset<k*k> intersect) {
	queue<int> q;
	bitset<k> v;
	vector<int> labelling(k,0);
	int start = findARoot(intersect, v);
	q.push(start);
	int s = 1;

	while ((~v).any()) {
		while (!q.empty()) {
			int cur = q.front();
			q.pop();
			if (v[cur]) continue;
			v[cur] = 1;
			labelling[cur] = s;
			s++;

			for (int i=0; i<k; i++) {
				if (intersect[cur*k + i]) {
					q.push(i);
				}
			}

		}
		start = findARoot(intersect, v);
		q.push(start);
	}

	return labelling;
}

int main() {


	vvi permutes = genAllPermutes(k);
	//cout << permutes.size() << endl;

	smvi quadPermutes;


	for (int i=0; i<permutes.size(); i++) {
		// cout << "i=" << i << " of " << permutes.size() << endl;
		for (int j=0; j<permutes.size(); j++) {
			if (j==i) continue;
			vector<int> p1, p2, p3, p4;
			p1 = permutes[i];
			p2 = permutes[j];
			int t = (i+j)%permutes.size();
			if (t == i || t == j) continue;
			p3 = permutes[t];

			bitset<k*k> intersect = constructIntersection(p1, p2, p3);

			// cout << i*permutes.size()+j << ": have 3 perms:" << endl;
			// for (int i=0; i<p1.size(); i++) {cout << p1[i] << " ";} cout << endl;
			// for (int i=0; i<p2.size(); i++) {cout << p2[i] << " ";} cout << endl;
			// for (int i=0; i<p3.size(); i++) {cout << p3[i] << " ";} cout << endl;
			
			flipIntersect(intersect);
			p4 = constructPerm(intersect);

			// cout << "and now 4th:" << endl;
			// for (int i=0; i<p4.size(); i++) {cout << p4[i] << " ";} cout << endl;
			// cout << endl;

			if (p4.size() == 0) continue;

			mvi newQuad;
			newQuad.insert(p1);
			newQuad.insert(p2);
			newQuad.insert(p3);
			newQuad.insert(p4);
			quadPermutes.insert(newQuad);
		}
	}

	cout << quadPermutes.size() << endl;

	smvi::iterator quad = quadPermutes.begin();
	while (quad != quadPermutes.end()) {
		mvi::iterator perm = (*quad).begin();
		while (perm != (*quad).end()) {
			for (int i=0; i<(*perm).size(); i++) {
				cout << (*perm)[i] << " ";
			}
			cout << endl;
			perm++;
		}
		cout << "======================" << endl;
		quad++;
	}


	return 0;
}