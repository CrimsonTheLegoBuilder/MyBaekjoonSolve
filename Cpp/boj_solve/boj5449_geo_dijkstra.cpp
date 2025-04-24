#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <cmath>
typedef long long ll;
typedef long double ld;
const int LEN = 205;
const ll iMAX = 1e17;
const ld fMAX = 1e17;
int T, N;

struct Pos {
	int x, y;
	Pos operator - (const Pos& p) { return { x - p.x, y - p.y }; }
	ld mag() { return hypot(x, y); }
} S, E, pos[LEN];
int n;
struct Seg {
	Pos L, R;
} fence[100];
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
bool intersect(const Pos& s1, const Pos& s2, const Pos& t1, const Pos& t2) {
	return ccw(s1, s2, t1) * ccw(s2, s1, t2) > 0 && ccw(t1, t2, s1) * ccw(t2, t1, s2) > 0;
}
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
	for (int i = 0; i < n; i++) C[i] = fMAX;
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
void init() {
	n = 0;
	std::cin >> S.x >> S.y >> E.x >> E.y >> N;
	pos[n++] = S; pos[n++] = E;
	for (int i = 0; i < N; i++) {
		std::cin >> fence[i].L.x >> fence[i].L.y >> fence[i].R.x >> fence[i].R.y;
		pos[n++] = fence[i].L; pos[n++] = fence[i].R;
	}
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			Pos v = pos[i], w = pos[j];
			bool f = 1;
			for (int k = 0; k < N; k++) {
				Pos l = fence[k].L, r = fence[k].R;
				if (intersect(v, w, l, r)) { f = 0; break; }
			}
			if (f) {
				ld cost = (v - w).mag();
				G[i].push_back({ j, cost });
				G[j].push_back({ i, cost });
			}
		}
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	std::cin >> T;
	while (T--) {
		for (int i = 0; i < LEN; i++) G[i].clear();
		init();
		std::cout << dijkstra(0, 1) << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj5449

//ld cost = dist(v, w);
//ld dist(const Pos& d1, const Pos& d2) {
//	return hypot((d1.x - d2.x), (d1.y - d2.y));
//}
