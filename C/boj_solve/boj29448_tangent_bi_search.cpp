#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
typedef long double ld;
const int LEN = 1e5 + 1;
//const ld TOL = 1e-7;
int N, M, K;
ld memo[LEN]{ 0 };

//bool z(ld x) { return std::abs(x) < TOL; }
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
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
	ld mag() { return hypot(x, y); }
} Fence[LEN], fan[LEN], O = { 0, 0 };
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
bool intersect(const Pos& s1, const Pos& s2, const Pos& p1, const Pos& p2) {
	bool f1 = ccw(s1, s2, p1) * ccw(s2, s1, p2) > 0;
	bool f2 = ccw(p1, p2, s1) * ccw(p2, p1, s2) > 0;
	bool f3 = on_seg(s1, s2, p1) || on_seg(s1, s2, p2) ||
			  on_seg(p1, p2, s1) || on_seg(p1, p2, s2);
	return (f1 && f2) || f3;
}
void get_round() {
	memo[0] = 0;
	for (int i = 0; i < N; i++) {
		Pos cur = Fence[i], nxt = Fence[(i + 1) % N];
		memo[i + 1] = (cur - nxt).mag() + memo[i];
	}
	return;
}
int inner_check_bi_search(Pos H[], const int& sz, const Pos& p) {
	if (sz < 3 || cross(H[0], H[1], p) <= 0 || cross(H[0], H[sz - 1], p) >= 0) return 0;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	return cross(H[s], H[e], p) > 0;
}
Pos find_tangent_bi_search(Pos H[], const int& sz, const Pos& p) {
	int i1{ 0 }, i2{ 0 };
	int ccw1 = ccw(p, H[0], H[1]), ccwN = ccw(p, H[0], H[sz - 1]);
	if (ccw1 * ccwN >= 0) {
		i1 = 0;
		if (!ccw1 && dot(p, H[1], H[0]) > 0) i1 = 1;
		if (!ccwN && dot(p, H[sz - 1], H[0]) > 0) i1 = sz - 1;
		int s = 0, e = sz - 1, m;
		if (!ccw1) s += 1;
		if (!ccwN) e -= 1;
		bool f = ccw(p, H[s], H[s + 1]) >= 0;
		while (s < e) {
			m = s + e >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);//normalize
			if (ccw(p1, cur, nxt) > 0) s = m + 1;
			else e = m;
		}
		i2 = s;
		if (!ccw(p, H[i2], H[(i2 + 1) % sz]) && dot(p, H[(i2 + 1) % sz], H[i2]) > 0) i2 = (i2 + 1) % sz;
	}
	else {
		//divide hull
		int s = 0, e = sz - 1, k, m;
		bool f = ccw1 > 0 && ccwN < 0;//if H[k] is between H[0] && p
		while (s + 1 < e) {
			k = s + e >> 1;
			int CCW = ccw(H[0], H[k], p);
			if (!f) CCW *= -1;//normailze
			if (CCW > 0) s = k;
			else e = k;
		}

		//search lower hull
		int s1 = 0, e1 = s;
		while (s1 < e1) {
			m = s1 + e1 >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);//normalize
			if (ccw(p1, cur, nxt) > 0) s1 = m + 1;
			else e1 = m;
		}
		i1 = s1;
		if (!ccw(p, H[i1], H[(i1 + 1) % sz]) && dot(p, H[(i1 + 1) % sz], H[i1]) > 0) i1 = (i1 + 1) % sz;

		//search upper hull
		int s2 = e, e2 = sz - 1;
		while (s2 < e2) {
			m = s2 + e2 >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);//normalize
			if (ccw(p1, cur, nxt) < 0) s2 = m + 1;
			else e2 = m;
		}
		i2 = s2;
		if (!ccw(p, H[i2], H[(i2 + 1) % sz]) && dot(p, H[(i2 + 1) % sz], H[i2]) > 0) i2 = (i2 + 1) % sz;
	}

	if (i2 < i1) std::swap(i2, i1);//normalize
	return { i2, i1 };
}
void query(const int& j) {
	Pos& F = fan[j], gate = Fence[K - 1];
	if (inner_check_bi_search(Fence, N, F)) { std::cout << "0.0000000\n"; return; }
	Pos tangent = find_tangent_bi_search(Fence, N, F);
	int i1 = tangent.x, i2 = tangent.y;
	if (!intersect(F, gate, Fence[i1], Fence[i2])) std::cout << (F - gate).mag() << "\n";
	else {
		ld arc{ 0 }, Rr{ 0 }, Rl{ 0 };
		Pos& vr = Fence[i1], vl = Fence[i2];

		//r-round
		int r1 = i1, r2 = K - 1;
		if (r2 < r1) std::swap(r1, r2);
		arc = memo[r2] - memo[r1];
		arc = std::min(arc, memo[N] - arc);
		Rr = arc + (F - vr).mag();

		//l-round
		int l1 = i2, l2 = K - 1;
		if (l2 < l1) std::swap(l1, l2);
		arc = memo[l2] - memo[l1];
		arc = std::min(arc, memo[N] - arc);
		Rl = arc + (F - vl).mag();
		std::cout << std::min(Rr, Rl) << "\n";
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N >> K;
	for (int i = 0; i < N; i++) std::cin >> Fence[i].x >> Fence[i].y;
	std::cin >> M;
	for (int j = 0; j < M; j++) std::cin >> fan[j].x >> fan[j].y;
	get_round();
	for (int j = 0; j < M; j++) query(j);
	return;
}
int main() { solve(); return 0; }//boj29448

/*

9 3
-4 -2
-2 -4
0 -5
2 -4
4 -2
4 2
2 4
-2 4
-4 2
7
0 4
0 5
4 0
-4 0
-4 -2
5 0
-5 0

*/