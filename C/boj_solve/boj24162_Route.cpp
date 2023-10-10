#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
typedef long long ll;
const int LEN = 101;
const ll MAX = 100'000'000'000'000'000;
int G[LEN][LEN];
ll W[LEN];

struct Pos {
	ll x, y;
	bool operator < (const Pos& p) const {
		if (x == p.x) { return y < p.y; }
		return x < p.x;
	}
}pos[LEN];
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x);
}

struct Toll {
	ll w;
	int n, d;
	Pos pre;
	bool operator < (const Toll& t) {
		if (w == t.w) return n < t.n;
		return w < t.w;
	}
};
std::priority_queue<Toll> H;

void dijkstra(int v, int N) {
	memset(W, MAX, sizeof W);
	W[v] = 0;
	H.push({ 0, v, 0, pos[v] });
	while (!H.empty()) {
		Toll p = H.top(); H.pop();
		if (W[p.n] < p.w) continue;
		for (int w = 0; w < N; w++) {
			if (G[p.n][w] > -1) {
				ll ct = p.w + G[p.n][w];
				if (W[w] > ct) {
					if (p.d < 2) {
						H.push({ ct, w, p.d + 1, pos[w] });
					}
				}
				else {

				}
			}
		}
	}
}