#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <set>
#include <unordered_set>
typedef long long ll;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::set<int> Sint;
typedef std::unordered_set<int> USint;
const int LEN = 1e5 + 5;

#define ADD 1
#define DEL -1

int N, w, h;
int P[LEN];//disjoint set
int find(int i) { return P[i] < 0 ? i : P[i] = find(P[i]); }
bool join(int i, int j) {
	i = find(i), j = find(j);
	if (i == j) return 0;
	if (P[i] < P[j]) P[i] += P[j], P[j] = i;
	else P[j] += P[i], P[i] = j;
	return 1;
}
int CX[LEN], CY[LEN], xi, yi;//compressed
int idx_bi_search(int A[], int sz, const int& x) {
	int s = 0, e = sz - 1, m;
	while (s <= e) {
		m = s + e >> 1;
		if (A[m] == x) return m;
		else if (A[m] > x) e = m - 1;
		else s = m + 1;
	}
	return -1;
}
struct H_event {
	int l, r, y, i;
	H_event(int l_ = 0, int r_ = 0, int y_ = 0, int i_ = 0) : l(l_), r(r_), y(y_), i(i_) {}
	bool operator < (const H_event& h) const { return y == h.y ? l < h.l : y < h.y; }
} H[LEN]; int hp;
struct V_event {
	int x, y, ev, i;
	V_event(int x_ = 0, int y_ = 0, int ev_ = 0, int i_ = 0) : x(x_), y(y_), ev(ev_), i(i_) {}
	bool operator < (const V_event& v) const { return y == v.y ? x < v.x : v < v.y; }
} V[LEN << 1]; int vp;
int max_y[LEN];
USint inxs[LEN << 4];//seg_tree
//Sint inxs[LEN << 4];//seg_tree
int seg_v[LEN << 4];//seg_tree
int seg_p[LEN << 4];//seg_tree
int sum_horizontal(int l, int r, int idx, int s = 0, int e = xi - 1, int i = 1) {
	if (r < s || e < l) return 0;
	if (l <= s && e <= r) {
		if (seg_v[i] != -1) join(idx, seg_v[i]);//find new intersection
		for (const int& v : inxs[i]) {
			join(idx, v);
			if (seg_v[i] == -1 || max_y[v] > max_y[seg_v[i]]) seg_v[i] = v;
		}
		inxs[i].clear();
		return seg_p[i];
	}
	int m = s + e >> 1;
	return sum_horizontal(l, r, idx, s, m, i << 1) + sum_horizontal(l, r, idx, m + 1, r, i << 1 | 1);
}
void update_vertical(int x, int ev, int idx, int s = 0, int e = xi - 1, int i = 1) {
	if (x < s || e < x) return;
	if (ev == ADD) inxs[i].insert(idx);
	else if (ev == DEL) {
		inxs[i].erase(idx);
		if (seg_v[i] == idx) seg_v[i] = -1;
	}
	if (s == e) { seg_p[i] += ev; return; }
	int m = s + e >> 1;
	update_vertical(x, ev, idx, s, m, i << 1);
	update_vertical(x, ev, idx, m + 1, e, i << 1 | 1);
	seg_p[i] = seg_p[i << 1] + seg_p[i << 1 | 1];
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> h >> w >> N;
	Vint X = { 0, w };
	Vint Y = { 0, h, h + 1 };
	hp = 0; vp = 0;
	for (int i = 0, a, b, c, d; i < N; i++) {
		std::cin >> a >> b >> c >> d;
		if (b == d) {
			H[hp++] = H_event(a, c, b, i);
		}
		else if (a == c) {
			V[vp++] = V_event(a, b, ADD, i);
			V[vp++] = V_event(a, d + 1, DEL, i);
			max_y[i] = d;
			d++;
		}
		X.push_back(a); X.push_back(c);
		Y.push_back(b); Y.push_back(d);
	}
	H[hp++] = H_event(0, w, 0, N);
	H[hp++] = H_event(0, w, h, N);
	V[vp++] = V_event(0, 0, ADD, N);
	V[vp++] = V_event(w, 0, ADD, N);
	V[vp++] = V_event(0, h + 1, DEL, N);
	V[vp++] = V_event(w, h + 1, DEL, N);

	std::sort(X.begin(), X.end());
	X.erase(unique(X.begin(), X.end()), X.end());
	std::sort(Y.begin(), Y.end());
	Y.erase(unique(Y.begin(), Y.end()), Y.end());
	int tmp;
	tmp = -1; for (const int& x : X) if (x != tmp) tmp = CX[xi++] = x;
	tmp = -1; for (const int& y : Y) if (y != tmp) tmp = CY[yi++] = y;
	std::sort(H, H + hp);
	std::sort(V, V + vp);
	memset(P, -1, sizeof P);
	memset(seg_v, -1, sizeof seg_v);
	memset(seg_p, 0, sizeof seg_p);

	int e = 0, v = 0, chi = 0;
	for (int y = 0, i = 0, j = 0; y < yi; y++) {
		while (i < vp && idx_bi_search(CY, yi, V[i].y) == y) {
			v++;
			e += V[i].ev == DEL;
			update_vertical(idx_bi_search(CX, xi, V[i].x), V[i].ev, V[i].i);
			i++;
		}
		while (j < hp && idx_bi_search(CY, yi, H[j].y) == y) {
			int l_ = idx_bi_search(CX, xi, H[j].l);
			int r_ = idx_bi_search(CX, xi, H[j].r);
			int cnt = sum_horizontal(l_, r_, H[j].i);
			e += cnt * 2 + 1;
			v += cnt + 2;
			j++;
		}
	}
	for (int i = 0; i <= N; i++) if (i == find(i)) chi++;
	std::cout << chi - v + e << "\n";//X = v - e + f
	return;
}
int main() { solve(); return 0; }//boj10049