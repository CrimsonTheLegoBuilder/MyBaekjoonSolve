#include <iostream>
#include <algorithm>
#include <cmath>
#include <set>
#include <string.h>
typedef long long ll;
typedef long double ld;
const int LEN = 1'001;
const ld MAX = 1e15;
std::set<int> G[LEN];
int N, D1, D2, ID[LEN], V[LEN], P[LEN]{};

struct Pos {
	ld x, y, z;
}pos[LEN];
ld dist(const Pos& u, const Pos& v) {
	return sqrtl((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y) + (u.z - v.z) * (u.z - v.z));
}

int DFS(int v, int p = 0) {
	V[v] = V[p] + 1;
	P[v] = p;
	int bottom = v;
	for (const int w : G[v]) {
		if (!~V[w]) {
			int x = DFS(w, v);
			if (V[x] > V[bottom])
				bottom = x;
		}
	}
	return bottom;
}



int main() {
	int id;
	while (1) {
		for (int i = 0; i < LEN; i++) {
			G[i].clear();
		}
		std::cin >> N;
		if (!N) return 0;
		for (int i = 1; i <= N; i++) {
			std::cin >> id >> pos[i].x >> pos[i].y >> pos[i].z;
			ID[i] = id;
			ld cost = MAX;
			int w = 1;
			for (int j = 1; j < i; j++) {
				ld tmp = dist(pos[j], pos[i]);
				if (cost > tmp) {
					cost = tmp; w = j;
				}
			}
			G[i].insert(w); G[w].insert(i);
		}

		memset(V, -1, sizeof V);
		D1 = DFS(1);

		memset(V, -1, sizeof V);
		D2 = DFS(D1);

		int v = D2, M = 0, find[LEN]{};
		while (v != 0) {
			find[M++] = v;
			v = P[v];
		}

		int d1 = 0, d2 = 0;
		if (!(M % 2)) {
			int m = M >> 1;
			d1 = std::min({ ID[find[m]], ID[find[m - 1]] });
			d2 = std::max({ ID[find[m]], ID[find[m - 1]] });
			std::cout << d1 << " " << d2 << "\n";
		}
		else std::cout << ID[P[M >> 1]] << "\n";
	}
}


/*
#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
typedef long double ld;
const int LEN = 1'001;
const ld MAX = 1e15;
bool G[LEN][LEN];
int V[LEN], ID[LEN];
int N, D1, D2;
int P[LEN]{};

struct Pos {
	ld x, y, z;
}pos[LEN];
ld dist(const Pos& u, const Pos& v) {
	return sqrtl((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y) + (u.z - v.z) * (u.z - v.z));
}

int DFS(int v, int p = 0) {
	V[v] = V[p] + 1;
	//std::cout << V[p] << "V ";
	P[v] = p;
	int bottom = v;
	for (int w = 1; w <= N; w++) {
		if (G[v][w] && !~V[w]) {
			//std::cout << w << ' ';
			int x = DFS(w, v);
			if (V[x] > V[bottom])
				bottom = x;
		}
	}
	return bottom;
}



int main() {
	int id;
	while (1) {
		memset(G, 0, sizeof G);
		std::cin >> N;
		if (!N) return 0;
		for (int i = 1; i <= N; i++) {
			std::cin >> id >> pos[i].x >> pos[i].y >> pos[i].z;
			ID[i] = id;
			ld cost = MAX;
			int w = 1;
			for (int j = 1; j < i; j++) {
				ld tmp = dist(pos[j], pos[i]);
				if (cost > tmp) {
					cost = tmp; w = j;
				}
			}
			//std::cout << i << ' ' << w << '\n';
			G[i][w] = G[w][i] = 1;
		}
		//for (int i = 1; i <= N; ++i) {
		//	for (int j = 1; j <= N; ++j)
		//		std::cout << G[i][j];
		//	std::cout << '\n';
		//}

		memset(V, -1, sizeof V);
		D1 = DFS(1);
		//std::cout << D1 << "\n";

		memset(V, -1, sizeof V);
		D2 = DFS(D1);
		//std::cout << D1 << " " << D2 << "\n";

		int v = D2, M = 0, find[LEN]{};
		while (v != 0) {
			find[M++] = v;
			v = P[v];
		}
		//std::cout << D1 << " " << D2 << "\n";
		//for (const int v : find) {
		//	std::cout << v << " ";
		//}
		//std::cout << M << "\n";

		int d1 = 0, d2 = 0;
		if (!(M % 2)) {
			//std::cout << (M >> 1) << "\n";
			d1 = std::min(ID[P[M >> 1]], ID[P[(M >> 1) - 1]]);
			d2 = std::max(ID[P[M >> 1]], ID[P[(M >> 1) - 1]]);
			std::cout << d1 << " " << d2 << "\n";
		}
		else std::cout << ID[P[M >> 1]] << "\n";
	}
}
*/