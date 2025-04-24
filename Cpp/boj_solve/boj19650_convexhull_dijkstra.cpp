#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <queue>
typedef long long ll;
typedef double ld;
const int LEN = 102;
const ll MAXi = 1e17;
const ld MAXd = 1e17;
int N, x, y, u, v, M, T, t;
ld V[LEN];

struct Pos {
	int x, y;
	int i;
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
}pos[LEN];
std::vector<Pos> tmp;
struct Hull {
	std::vector<Pos> H, L, U;
}hulls[LEN];
struct Info {
	int i;
	ld c;
	bool operator < (const Info& x) const { return c > x.c; }
};
std::vector<Info> G[LEN];
std::priority_queue<Info> Q;
ld dijkstra(int v, int g) {
	for (int i = 0; i <= t; i++) V[i] = MAXd;
	Q.push({ v, 0 });
	V[v] = 0;
	while (Q.size()) {
		Info p = Q.top(); Q.pop();
		if (p.c > V[p.i]) continue;
		for (const Info& w : G[p.i]) {
			ld nxt = p.c + w.c;
			if (V[w.i] > nxt) {
				V[w.i] = nxt;
				Q.push({ w.i, nxt });
			}
		}
	}
	return V[g];
}
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (ll)(d2.x - d1.x) * (d3.y - d2.y) - (ll)(d2.y - d1.y) * (d3.x - d2.x);
}
ld dist(const Pos& d1, const Pos& d2) {
	return hypot((d1.x - d2.x), (d1.y - d2.y));
}
bool I(std::vector<Pos> H, const Pos& p) {
	int h = H.size() - 1;
	if (h < 2 || cross(H[0], H[1], p) <= 0 || cross(H[0], H[h], p) >= 0) return 0;
	int s = 0, e = h, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	return cross(H[s], H[e], p) > 0;
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	//std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
}
std::vector<Pos> lower_hull(std::vector<Pos>& C) {  //lower monotone chain
	std::vector<Pos> H;
	//std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	return H;
}
std::vector<Pos> upper_hull(std::vector<Pos>& C) {  //upper monotone_chain
	std::vector<Pos> H;
	//std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (int i = C.size() - 1; i >= 0; i--) H.push_back(C[i]);
		return H;
	}
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	return H;
}
Pos ternary_search(std::vector<Pos>& H, const Pos& p1, const Pos& p2, bool f = 0) {
	int s = 0, e = H.size() - 1, l, r;
	ll X = MAXi;
	Pos EX = H[0], d1 = p1, d2 = p2;
	if (!f && d2 < d1) std::swap(d1, d2); // lower hull
	if (f && d1 < d2) std::swap(d1, d2);  // upper hull
	while (e - s > 2) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		ll cl = cross(d1, d2, H[l]), cr = cross(d1, d2, H[r]);
		if (cl > cr) s = l;
		else e = r;
	}
	for (int i = s; i <= e; i++) {
		if (cross(d1, d2, H[i]) < X) {
			X = cross(d1, d2, H[i]);
			EX = H[i];
		}
	}
	return EX;
}
int C(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ccw = (ll)(d2.x - d1.x) * (d3.y - d2.y) - (ll)(d2.y - d1.y) * (d3.x - d2.x);
	return ccw > 0 ? 1 : ccw < 0 ? -1 : 0;
}
int D(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll dot = (ll)(d2.x - d1.x) * (d3.x - d2.x) + (ll)(d2.y - d1.y) * (d3.y - d2.y);
	return dot > 0 ? 1 : dot < 0 ? -1 : 0;
}
bool X(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return C(d1, d2, d3) * C(d2, d1, d4) > 0 && C(d3, d4, d1) * C(d4, d3, d2) > 0;
}
bool X(int i, const Pos& d1, const Pos& d2) {
	if (I(hulls[i].H, d1) || I(hulls[i].H, d2)) return 1;
	int h = hulls[i].H.size();
	if (h < 3) return 0;
	for (int j = 0; j < h; j++) {
		Pos cur = hulls[i].H[j], nxt = hulls[i].H[(j + 1) % h];
		if ((!C(cur, nxt, d1) && D(cur, d1, nxt) > 0) && C(cur, nxt, d2) > 0) return 1;
		if ((!C(cur, nxt, d2) && D(cur, d2, nxt) > 0) && C(cur, nxt, d1) > 0) return 1;
	}
	Pos pl = ternary_search(hulls[i].L, d1, d2);
	Pos pu = ternary_search(hulls[i].U, d1, d2, 1);
	return X(d1, d2, pl, pu);
}
bool hull_init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> x >> y >> u >> v >> N;
	t = -1;
	pos[++t] = { x, y };
	pos[++t] = { u, v };
	if (!N) return 0;
	for (int i = 0; i < N; i++) {
		std::cin >> M;
		tmp.resize(M);
		//std::cout << "resize DEBUG\n";
		for (int j = 0; j < M; j++) {
			std::cin >> tmp[j].x >> tmp[j].y;
			pos[++t] = { tmp[j].x, tmp[j].y };
			//std::cout << "push_back DEBUG\n";
		}
		std::sort(tmp.begin(), tmp.end());
		hulls[i].H = monotone_chain(tmp);
		hulls[i].L = lower_hull(tmp);
		hulls[i].U = upper_hull(tmp);
		//std::cout << "hull_init DEBUG\n";
	}
	return 1;
}
bool graph_init() {
	for (int i = 0; i < LEN; i++) G[i].clear();
	for (int i = 0; i <= t; i++) {
		for (int j = i + 1; j <= t; j++) {
			Pos s = pos[i], e = pos[j];
			bool f = 1;
			for (int k = 0; k < N; k++) {
				if (X(k, s, e)) { f = 0; break; }
			}
			if (f) {
				ld c = dist(s, e);
				G[i].push_back({ j, c });
				G[j].push_back({ i, c });
			}
		}
	}
	if (!G[0].size() || !G[1].size()) return 0;
	return 1;
}

int main() {
	if (!hull_init()) {
		std::cout << dist(pos[0], pos[1]) << "\n";
		return 0;
	}
	if (!graph_init()) {
		std::cout << "-1\n";
		return 0;
	}
	ld cost = dijkstra(0, 1);
	if (cost > 1e16) std::cout << "-1\n";
	else std::cout << cost << "\n";
	return 0;
}