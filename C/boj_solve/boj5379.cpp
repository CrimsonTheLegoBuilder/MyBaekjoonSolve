#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <cmath>
typedef long long ll;
typedef long double ld;
const int LEN = 51;
const ll MAXi = 1e17;
const ld MAX = 1e17;
int T, N, M;

struct Pos {
	ll x, y;
	int i;
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	ld mag() { return hypot(x, y); }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
};
std::vector<Pos> tmp;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ld dist(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	ld D = (d1 - d2).mag();
	return ret / D;
}
std::vector<Pos> island[LEN];
struct Info {
	int i;
	ld c;
	bool operator < (const Info& x) const {
		return c > x.c;
	}
};
std::priority_queue<Info> Q;
std::vector<Info> G[LEN];
ld C[LEN];
ld dijkstra(int s, int e) {
	for (int i = 1; i <= N; i++) C[i] = MAX;
	Q.push({ s, 0 });
	C[s] = 0;
	while (Q.size()) {
		Info p = Q.top(); Q.pop();
		if (p.c > C[p.i]) continue;
		for (const Info& w : G[p.i]) {
			ld cost = w.c + p.c;
			if (C[w.i] > cost) {
				C[w.i] = cost;
				Q.push({ w.i, cost });
			}
		}
	}
	return C[e];
}
