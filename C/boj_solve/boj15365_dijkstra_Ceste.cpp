#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
typedef long long ll;
const ll INF = 1e17;
const int LEN = 2e3 + 1;
int N, M;
ll C[LEN]{ -1 };

struct Pos {
	ll x, y;
	Pos(ll X, ll Y) : x(X), y(Y) {}
	Pos() : x(0), y(0) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ll operator ! () const { return x * y; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
} P[LEN], pivot = { -1, -1 };
const Pos O = { 0, 0 }, MAX = { INF, INF };
std::vector<Pos> H[LEN];
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !cross(d1, d2, d3) && dot(d1, d3, d2) > 0;
}
struct Info {
	int i;
	ll t, c, tc;
	bool operator < (const Info& info) const { return tc > info.tc; }
	Pos pos() const { return { t, c }; }
};
std::priority_queue<Info> Q;
std::vector<Info> G[LEN];
bool inner_check_bi_search(const int& i, const Pos& TC) {
	int sz = H[i].size();
	if (sz == 1) {
		Pos& p0 = H[i][0];
		if (p0 == TC) return 0;
		else if (p0.x <= TC.x && p0.x <= TC.y) return 1;
		else return 0;
	}
	Pos& L = H[i][0], R = H[i][sz - 1];
	if (L == TC || R == TC) return 0;
	else if (L.x <= TC.x && L.y <= TC.y) return 1;
	else if (R.x <= TC.x && R.x <= TC.y) return 1;
	if (L.x > TC.x || R.y > TC.y) return 0;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(pivot, H[i][m], TC) < 0) s = m;
		else e = m;
	}
	if (on_seg(H[i][s], H[i][e], TC)) return 1;
	return cross(H[i][s], H[i][e], TC) > 0;
}
void update(const int& i, const Pos& TC) {
	int sz = H[i].size();
	if (sz == 1) {
		H[i].push_back(TC);
		std::sort(H[i].begin(), H[i].end());
		Pos& p1 = H[i][1];
		if (p1.x >= TC.x && p1.y >= TC.y) H[i].pop_back();
		return;
	}
	else {
		int s = 0, e = sz - 1;
		if (TC.x <= H[i][0].x || cross(H[i][0], H[i][1], TC) <= 0) s = 0;
		else {
			for (int j = 0; j < sz - 1; j++) {
				Pos& cur = H[i][j], nxt = H[i][j + 1];
				ll ccw = cross(TC, cur, nxt);
				if (ccw <= 0) {
					s = j; break;
				}
			}
		}
		if (TC.y <= H[i][sz - 1].y || cross(H[i][sz - 1], H[i][sz - 2], TC) >= 0) e = sz - 1;
		else {
			for (int k = sz - 1; k > 0; k--) {
				Pos& cur = H[i][k], nxt = H[i][k - 1];
				ll ccw = cross(TC, cur, nxt);
				if (ccw >= 0) {
					e = k; break;
				}
			}
		}
		if (H[i][s] == TC || H[i][e] == TC) return;
		std::vector<Pos> h;
		for (int l = 0; l <= s; l++) if (H[i][l].x < TC.x && H[i][l].y > TC.y) h.push_back(H[i][l]);
		h.push_back(TC);
		for (int l = e; l <= sz; l++) if (H[i][l].x > TC.x && H[i][l].y < TC.y) h.push_back(H[i][l]);
		while (H[i].size() > 1 && H[i][H[i].size()].y >= H[i][H[i].size() - 1].y)
			H[i].pop_back();
		H[i] = h;
	}
	return;
}
void dijkstra() {
	for (int i = 1; i <= N; i++) H[i].push_back(MAX);
	for (int i = 1; i <= N; i++) C[i] = INF;
	H[1] = { { 0, 0 } };
	C[1] = 0;
	Q.push({ 0, 0, 0, 0 });
	while (Q.size()) {
		Info v = Q.top(); Q.pop();
		Pos cur = v.pos();
		int f = inner_check_bi_search(v.i, cur);
		if (f) continue;

		for (const Info& w : G[v.i]) {
			Pos nxt = w.pos();
			Pos cost = cur + nxt;
			if (!inner_check_bi_search(w.i, cost)) {
				update(w.i, cost);
				ll weight = !cost;
				Q.push({ w.i, cost.x, cost.y, weight });
				C[w.i] = std::min(C[w.i], weight);
			}
		}
	}
	for (int i = 1; i <= N; i++) if (C[i] == INF) C[i] = -1;
	return;
}
void answer() { for (int i = 2; i <= N; i++) std::cout << C[i] << "\n"; }
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen(".in", "r", stdin);
	//freopen(".out", "w", stdout);
	std::cin >> N >> M;
	for (int i = 1; i <= N; i++) {
		int s, e; ll t, c;
		std::cin >> s >> e >> t >> c;
		G[s].push_back({ e, t, c, 0 });
		G[e].push_back({ s, t, c, 0 });
	}
	return;
}
void solve() { init(); dijkstra(); answer(); return; }
int main() { solve(); return 0; }//boj15365
