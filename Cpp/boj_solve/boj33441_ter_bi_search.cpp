#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
typedef std::vector<int> Vint;
const ll INF = 9e18;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }

#define LO 1
#define HI -1

int N, T, Q, QQ;
struct Pos {
	int x, y;
	int i;
	Pos(int x_ = 0, int y_ = 0, int i_ = 0) : x(x_), y(y_), i(i_) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const int& n) { x /= n; y /= n; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll xy() const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
std::vector<Pos> graham_scan(std::vector<Pos>& C) {
	std::vector<Pos> H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
Polygon half_monotone_chane(Polygon& C, const int& d = LO) {
	if (d == LO) std::sort(C.begin(), C.end());
	if (d == HI) std::reverse(C.begin(), C.end());
	Polygon H;
	for (const Pos& p : C) {
		while (H.size() > 1 && ccw(H[H.size() - 2], H.back(), p) <= 0) H.pop_back();
		H.push_back(p);
	}
	return H;
}
Pos ternary_search(const Polygon& H, Pos v, const int& d) {
	if (v.x * d < 0) v *= d;
	int s = 0, e = H.size() - 1;
	while (e - s > 2) {
		int m1 = (s + s + e) / 3;
		int m2 = (s + e + e) / 3;
		ll tq1 = v / H[m1];
		ll tq2 = v / H[m2];
		if (tq1 > tq2) s = m1;
		else e = m2;
	}
	ll tq = INF; Pos p = H[s]; int x = 0;
	for (int i = s; i <= e; i++) {
		if (v / H[i] < tq) {
			tq = v / H[i], p = H[i];
			x = i;
		}
	}
	p.i = x;
	return p;
}
int bi_search(const Polygon& H, int s, int e, Pos p, Pos v) {
	if (cross(p, v, H[s], H[e]) < 0) std::swap(p, v);
	while (s < e) {
		int m = s + e + 1 >> 1;
		ll tq = cross(p, v, H[m]);
		if (tq == 0) return m;
		if (tq > 0) e = m - 1;
		else s = m;
	}
	if (!cross(p, v, H[s])) return s;
	if (!cross(p, v, H[e])) return e;
	return -1;
}
int query(const Polygon& P, const Polygon& L, const Polygon& U) {
	int q, sz = P.size();
	Pos p1, p2, u, v;
	std::cin >> q >> p1;
	int cnt = 0;
	Polygon V;
	if (q == 1) {
		for (int i = 0; i < sz; i++) {
			const Pos& p = P[i];
			v = p1 - p;
			Pos lp = ternary_search(L, v, LO);
			Pos up = ternary_search(U, v, HI);
			Polygon I;
			int l1 = bi_search(L, 0, lp.i, p, p1);
			if (l1 >= 0) {
				if (l1 > 0 && p != L[l1 - 1] && !ccw(p, p1, L[l1 - 1])) {
					V.push_back(p);
					V.push_back(L[l1 - 1]);
				}
				if (l1 < L.size() - 1 && p != L[l1 + 1] && !ccw(p, p1, L[l1 + 1])) {
					V.push_back(p);
					V.push_back(L[l1 + 1]);
				}
				I.push_back(L[l1]);
			}
			int l2 = bi_search(L, lp.i, L.size() - 1, p, p1);
			if (l2 >= 0) {
				if (l2 > 0 && p != L[l2 - 1] && !ccw(p, p1, L[l2 - 1])) {
					V.push_back(p);
					V.push_back(L[l2 - 1]);
				}
				if (l2 < L.size() - 1 && p != L[l2 + 1] && !ccw(p, p1, L[l2 + 1])) {
					V.push_back(p);
					V.push_back(L[l2 + 1]);
				}
				I.push_back(L[l2]);
			}
			int u1 = bi_search(U, 0, up.i, p, p1);
			if (u1 >= 0) {
				if (u1 > 0 && p != U[u1 - 1] && !ccw(p, p1, U[u1 - 1])) {
					V.push_back(p);
					V.push_back(U[u1 - 1]);
				}
				if (u1 < U.size() - 1 && p != U[u1 + 1] && !ccw(p, p1, U[u1 + 1])) {
					V.push_back(p);
					V.push_back(U[u1 + 1]);
				}
				I.push_back(U[u1]);
			}
			int u2 = bi_search(U, up.i, U.size() - 1, p, p1);
			if (u2 >= 0) {
				if (u2 > 0 && p != U[u2 - 1] && !ccw(p, p1, U[u2 - 1])) {
					V.push_back(p);
					V.push_back(U[u2 - 1]);
				}
				if (u2 < U.size() - 1 && p != U[u2 + 1] && !ccw(p, p1, U[u2 + 1])) {
					V.push_back(p);
					V.push_back(U[u2 + 1]);
				}
				I.push_back(U[u2]);
			}
			std::sort(I.begin(), I.end());
			I.erase(unique(I.begin(), I.end()), I.end());
			if (I.size() == 2) {
				//std::cout << "I[0]:: " << I[0] << " I[1]:: " << I[1] << "\n";
				V.push_back(I[0]);
				V.push_back(I[1]);
				//cnt++;
			}
		}
		std::sort(V.begin(), V.end());
		V.erase(unique(V.begin(), V.end()), V.end());
		//std::cout << "V::\n";
		//for (Pos& p : V) std::cout << p << "\n";
		//std::cout << "V::\n";
		cnt = V.size(); assert(cnt % 2 == 0);
		std::cout << (cnt >> 1) << "\n";
		return 1;
	}
	else if (q == 2) {
		std::cin >> p2;
		v = p2 - p1;
		Pos lp = ternary_search(L, v, LO);
		Pos up = ternary_search(U, v, HI);
		Polygon I;
		int l1 = bi_search(L, 0, lp.i, p1, p2);
		if (l1 >= 0) {
			V.push_back(L[l1]);
			if (l1 > 0 && !ccw(p1, p2, L[l1 - 1])) V.push_back(L[l1 - 1]);
			if (l1 < L.size() - 1 && !ccw(p1, p2, L[l1 + 1])) V.push_back(L[l1 + 1]);
		}
		int l2 = bi_search(L, lp.i, L.size() - 1, p1, p2);
		if (l2 >= 0) {
			V.push_back(L[l2]);
			if (l2 > 0 && !ccw(p1, p2, L[l2 - 1])) V.push_back(L[l2 - 1]);
			if (l2 < L.size() - 1 && !ccw(p1, p2, L[l2 + 1])) V.push_back(L[l2 + 1]);
		}
		int u1 = bi_search(U, 0, up.i, p1, p2);
		if (u1 >= 0) {
			V.push_back(U[u1]);
			if (u1 > 0 && !ccw(p1, p2, U[u1 - 1])) V.push_back(U[u1 - 1]);
			if (u1 < U.size() - 1 && !ccw(p1, p2, U[u1 + 1])) V.push_back(U[u1 + 1]);
		}
		int u2 = bi_search(U, up.i, U.size() - 1, p1, p2);
		if (u2 >= 0) {
			V.push_back(U[u2]);
			if (u2 > 0 && !ccw(p1, p2, U[u2 - 1])) V.push_back(U[u2 - 1]);
			if (u2 < U.size() - 1 && !ccw(p1, p2, U[u2 + 1])) V.push_back(U[u2 + 1]);
		}
		std::sort(V.begin(), V.end());
		V.erase(unique(V.begin(), V.end()), V.end());
		cnt = V.size();
		//if (QQ == 1 && N == 1000) std::cout << "FUCK\n";
		std::cout << cnt << "\n";
		return 0;
	}
	else { assert(0); }
	return -1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> Q; QQ = Q;
	Polygon C(N); for (Pos& p : C) std::cin >> p;
	Polygon P = graham_scan(C);
	Polygon L = half_monotone_chane(C, LO);
	Polygon U = half_monotone_chane(C, HI);
	int q1 = 0, q;
	while (Q--) {
		q = query(P, L, U);
		q1 += q == 1;
	}
	assert(q1 < 105);
	return;
}
int main() { solve(); return 0; }//boj33441

/*

8 7
0 0
1 -2
2 -3
3 -2
4 0
3 2
2 3
1 2
2 2 0 2 1
2 2 1 2 0
2 3 -4 4 -5 
2 0 -2 5 -2
2 0 0 4 0
2 0 -1 0 -2
2 1 -1 1 -25


*/