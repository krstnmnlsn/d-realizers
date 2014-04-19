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
// THESE WILL BE ALL THE ORDERS!! 


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

bitset<k*k>	 constructIntersection(vector<int> p1, vector<int> p2, vector<int> p3, vector<int> p4) {
	// directed bitset, edge (x to y) represented as x*k + y
	bitset<k*k> p1set, p2set, p3set, p4set, intersection;

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

	for (int i=0; i<p4.size(); i++) {
		for (int j=0; j<i; j++) {
			p4set[(p4[j]-1)*k + (p4[i]-1)] = 1;
		}
	}

	intersection = (p1set & p2set) & (p3set & p4set);
	
	return intersection;
}


bool isCandidate(int x, int y, vvi quadPermutes) {
	//cout << "checking is isCandidate " << endl;
	for (int z=1; z<=quadPermutes[0].size(); z++) {
		if (z == x || z == y) continue;
		bool interfers = true;
		//cout << "trying z =" << z << endl;
		for (int i=0; i<quadPermutes.size(); i++) {
			//cout << "z at " << find(quadPermutes[i].begin(), quadPermutes[i].end(),z) - quadPermutes[i].begin() << " vs " << 
			//find(quadPermutes[i].begin(), quadPermutes[i].end(),x) - quadPermutes[i].begin() << " " << find(quadPermutes[i].begin(), quadPermutes[i].end(),y) - quadPermutes[i].begin() << endl;
			if ((find(quadPermutes[i].begin(), quadPermutes[i].end(),z) - find(quadPermutes[i].begin(), quadPermutes[i].end(),x) > 0) && 
				find(quadPermutes[i].begin(), quadPermutes[i].end(),z) - find(quadPermutes[i].begin(), quadPermutes[i].end(),y) > 0)
				{
					interfers = false;
				}
		}
		if (interfers) {
			//cout << "z=" << z << " broke it" << endl;
			return false;
		}
	}
	//cout << "is a candidate " << endl;
	return true;
}

bool isOneOfD(int x, int y, vvi quadPermutes) {
	int numTimesYWins = 0;
	int numTimesXWins = 0;

	for (int i=0; i<quadPermutes.size(); i++) {
		if (find(quadPermutes[i].begin(), quadPermutes[i].end(),y) > find(quadPermutes[i].begin(), quadPermutes[i].end(),x))
			numTimesYWins++;
		else
			numTimesXWins++;
	}

	if (numTimesXWins == 1 || numTimesYWins == 1) return true;
	return false;
}


int main() {


	vvi permutes = genAllPermutes(k);
	//cout << permutes.size() << endl;

	vvvi quadPermutes;


	for (int i=0; i<permutes.size(); i++) {
		for (int j=0; j<permutes.size(); j++) {
			for (int i1=0; i1<permutes.size(); i1++) {
				for (int j2=0; j2<permutes.size(); j2++) {

					vector<int> p1, p2, p3, p4;
					p1 = permutes[i];
					p2 = permutes[j];
					p3 = permutes[i1];
					p4 = permutes[j2];

					bitset<k*k> intersect = constructIntersection(p1, p2, p3, p4);

					if (intersect.any()) continue;

					vvi newQuad;
					newQuad.push_back(p1);
					newQuad.push_back(p2);
					newQuad.push_back(p3);
					newQuad.push_back(p4);
					quadPermutes.push_back(newQuad);
				}
			}
		}
	}


	cout << quadPermutes.size() << endl;

	// convert to a vvvi...
	vvvi quadPermuteVec = quadPermutes;
	// smvi::iterator quad = quadPermutes.begin();
	// while (quad != quadPermutes.end()) {
	// 	vvi newquadVect;
	// 	mvi::iterator perm = (*quad).begin();
	// 	while (perm != (*quad).end()) {
	// 		newquadVect.push_back(*perm);
	// 		perm++;
	// 	}
	// 	quadPermuteVec.push_back(newquadVect);
	// 	quad++;
	// }


	for (int i=0; i<quadPermuteVec.size(); i++) {
		for (int j=0; j<quadPermuteVec[0].size(); j++) {
			vector<int> suspensions;
			for (int j2=4; j2<=7; j2++) {
				if (j2 == j+4) continue;
				suspensions.push_back(j2);
			}

			suspensions.insert(suspensions.end(), quadPermuteVec[i][j].begin(), quadPermuteVec[i][j].end());
			suspensions.push_back(j+4);
			quadPermuteVec[i][j] = suspensions;

		}
	}

	// for (int i=0; i<quadPermuteVec.size(); i++) {
	// 	for (int j=0; j<quadPermuteVec[0].size(); j++) {
	// 		for (int j2=0; j2<quadPermuteVec[0][0].size(); j2++) {
	// 			cout << quadPermuteVec[i][j][j2] << " ";
	// 		}
	// 		cout << endl;
	// 	}
	// 	cout << "=====================" << endl;
	// }

	// now go from d-realizer to graph...

	vvvi graphs;
	for (int i=0; i<quadPermuteVec.size(); i++) {
		int n = k+4;
		vvi graph(n, vector<int>(n,0));
		for (int i2=1; i2<=n; i2++) {
			for (int i3=1; i3<=n; i3++) {
				//cout << isCandidate(i2,i3, quadPermuteVec[i]) << " " << isOneOfD(i2, i3, quadPermuteVec[i]) << endl;
				if (isCandidate(i2,i3, quadPermuteVec[i]) && isOneOfD(i2, i3, quadPermuteVec[i])) {
					graph[i2-1][i3-1] = 1;
					graph[i3-1][i2-1] = 1;
				}
			}
		}

		graphs.push_back(graph);
	}

	for (int i2=0; i2<graphs.size(); i2++) {
		cout << "{";
		for (int i=0; i<k+4; i++) {
			cout << "{";
			for (int j=0; j<k+4; j++) {
				cout << graphs[i2][i][j];
				if (j < k+4-1) cout << ",";
			}
			cout << "}";
			if (i <k+4-1) cout << ",";
		}
		cout << "}," << endl;
	}
	return 0;
}