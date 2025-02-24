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
inline int gcd(int a, int b) { while (b) { int tmp = a % b; a = b; b = tmp; } return a; }

#define LO 1
#define HI -1

#define LEFT x
#define RIGHT y

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
const Pos INVAL = Pos(-1, -1);
const Pos OUTER = Pos(-1, -1);
const Pos ONSEG = Pos(-2, -2);
const Pos ONPT = Pos(-3, -3);
typedef std::vector<Pos> Polygon;
Pos norm(const Pos& p) { int g = gcd(std::abs(p.x), std::abs(p.y)); return p / g; }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
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
Pos ternary_search(const Polygon& H, Pos v, Pos p1, Pos p2, const int& d) {
	if (v.x * d < 0) std::swap(p1, p2);
	int s = 0, e = H.size() - 1;
	while (e - s > 2) {
		int m1 = (s + s + e) / 3;
		int m2 = (s + e + e) / 3;
		ll tq1 = cross(p1, p2, H[m1]);
		ll tq2 = cross(p1, p2, H[m2]);
		if (tq1 > tq2) s = m1;
		else e = m2;
	}
	ll tq = INF; Pos p = H[s]; int x = 0;
	for (int i = s; i <= e; i++) {
		if (cross(p1, p2, H[i]) < tq) {
			tq = cross(p1, p2, H[i]), p = H[i];
			x = i;
		}
	}
	p.i = x;
	return p;
}
int bi_search(const Polygon& H, int s, int e, Pos p, Pos v) {
	if (cross(p, v, H[s], H[e]) < 0) std::swap(p, v);
	while (s < e) {
		int m = s + e >> 1;
		ll tq = cross(p, v, H[m]);
		if (tq == 0) return m;
		if (tq > 0) e = m - 1;
		else s = m + 1;
	}
	if (!cross(p, v, H[s])) return s;
	if (!cross(p, v, H[e])) return e;
	return -1;
}
Pos inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	if (!sz) return Pos(-1, -1);
	if (sz == 1) return p == H[0] ? Pos(0, 0) : Pos(-1, -1);
	if (sz == 2) {
		int i1 = -1, i2 = -1;
		if (H[0] == p) i1 = 0;
		if (H[1] == p) i2 = 1;
		return Pos(i1, i2);
	}
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return Pos(-1, -1);
	//if (H[0] == p) return Pos(sz - 1, 1);
	if (H[0] == p) return Pos(-3, -3);
	//if (H[1] == p) return Pos(0, 2 % sz);
	if (H[1] == p) return Pos(-3, -3);
	//if (H[sz - 1] == p) return Pos(sz - 2, 0);
	if (H[sz - 1] == p) return Pos(-3, -3);
	//if (on_seg_weak(H[0], H[1], p)) return Pos(0, 1);
	if (on_seg_weak(H[0], H[1], p)) return Pos(-2, -2);
	//if (on_seg_weak(H[0], H[sz - 1], p)) return Pos(sz - 1, 0);
	if (on_seg_weak(H[0], H[sz - 1], p)) return Pos(-2, -2);
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) >= 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) < 0) return Pos(-1, -1);
	//if (H[s] == p) return Pos((s - 1 + sz) % sz, e);
	if (H[s] == p) return Pos(-3, -3);
	//if (H[e] == p) return Pos(s, (e + 1) % sz);
	if (H[e] == p) return Pos(-3, -3);
	//if (on_seg_weak(H[s], H[e], p)) return Pos(s, e);
	if (on_seg_weak(H[s], H[e], p)) return Pos(-2, -2);
	return Pos(sz + 1, sz + 1);
}
Pos find_tangent_bi_search(const Polygon& H, const Pos& p) {
	int sz = H.size();
	Pos IN = Pos(sz + 1, sz + 1);
	Pos F = inner_check_bi_search(H, p);
	//std::cout << "inner_bi:: " << F << "\n";
	if (F == IN) return INVAL;
	if (F != OUTER) return F;
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
			const Pos& p1 = p, & cur = H[m], & nxt = H[(m + 1) % sz];
			int CCW = ccw(p1, cur, nxt);
			if (!f) CCW *= -1;//normailze
			if (CCW > 0) s = m + 1;
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
			const Pos& p1 = p, & cur = H[m], & nxt = H[(m + 1) % sz];
			int CCW = ccw(p1, cur, nxt);
			if (!f) CCW *= -1;//normailze
			if (CCW > 0) s1 = m + 1;
			else e1 = m;
		}
		i1 = s1;
		if (!ccw(p, H[i1], H[(i1 + 1) % sz]) && dot(p, H[(i1 + 1) % sz], H[i1]) > 0) i1 = (i1 + 1) % sz;
		//search upper hull
		int s2 = e, e2 = sz - 1;
		while (s2 < e2) {
			m = s2 + e2 >> 1;
			const Pos& p1 = p, & cur = H[m], & nxt = H[(m + 1) % sz];
			int CCW = ccw(p1, cur, nxt);
			if (!f) CCW *= -1;//normailze
			if (CCW < 0) s2 = m + 1;
			else e2 = m;
		}
		i2 = s2;
		if (!ccw(p, H[i2], H[(i2 + 1) % sz]) && dot(p, H[(i2 + 1) % sz], H[i2]) > 0) i2 = (i2 + 1) % sz;
	}
	if (ccw(p, H[i1], H[i2]) > 0) std::swap(i1, i2);
	return Pos(i1, i2);
}
int query(const Polygon& P, const Polygon& L, const Polygon& U) {
	int q, sz = P.size();
	Pos p1, p2, u, v;
	std::cin >> q >> p1;
	int cnt = 0;
	if (q == 1) {
		Pos t = find_tangent_bi_search(P, p1);
		if (t == INVAL) {
			for (int i = 0, j = 1; i < sz; i++) {
				while (ccw(P[i], p1, P[j]) < 0) j = (j + 1) % sz;
				if (!ccw(P[i], p1, P[j])) cnt++;
			}
		}
		else if (t == ONSEG) { cnt += 2; }
		else if (t == ONPT) { cnt += 4; }
		else {
			int l = t.LEFT, r = t.RIGHT;
			assert(l != r);
			int i = l, j = (l - 1 + sz) % sz;
			if (!ccw(p1, P[r], P[(r + 1) % sz])) cnt += 2;
			while (1) {
				if (!ccw(p1, P[i], P[j])) {
					cnt += 2;
					i = (i + 1) % sz;
					j = (j - 1 + sz) % sz;
				}
				else if (ccw(p1, P[i], P[j]) < 0) i = (i + 1) % sz;
				else if (ccw(p1, P[i], P[j]) > 0) j = (j - 1 + sz) % sz;
				if (i == r) break;
			}
		}
		std::cout << (cnt >> 1) << "\n";
		return 1;
	}
	else if (q == 2) {
		Polygon V;
		std::cin >> p2;
		v = p2 - p1;
		Pos lp = ternary_search(L, v, p1, p2, LO);
		Pos up = ternary_search(U, v, p1, p2, HI);
		Polygon I;
		int l1 = bi_search(L, 0, lp.i, p1, p2);
		if (l1 >= 0) {
			if (l1 > 0 && !ccw(p1, p2, L[l1 - 1]) || l1 < L.size() - 1 && !ccw(p1, p2, L[l1 + 1])) {
				std::cout << "2\n"; return 0;
			}
			V.push_back(L[l1]);
		}
		int l2 = bi_search(L, lp.i, L.size() - 1, p1, p2);
		if (l2 >= 0) {
			if (l2 > 0 && !ccw(p1, p2, L[l2 - 1]) || l2 < L.size() - 1 && !ccw(p1, p2, L[l2 + 1])) {
				std::cout << "2\n"; return 0;
			}
			V.push_back(L[l2]);
		}
		int u1 = bi_search(U, 0, up.i, p1, p2);
		if (u1 >= 0) {
			if (u1 > 0 && !ccw(p1, p2, U[u1 - 1]) || u1 < U.size() - 1 && !ccw(p1, p2, U[u1 + 1])) {
				std::cout << "2\n"; return 0;
			}
			V.push_back(U[u1]);
		}
		int u2 = bi_search(U, up.i, U.size() - 1, p1, p2);
		if (u2 >= 0) {
			if (u2 > 0 && !ccw(p1, p2, U[u2 - 1]) || u2 < U.size() - 1 && !ccw(p1, p2, U[u2 + 1])) {
				std::cout << "2\n"; return 0;
			}
			V.push_back(U[u2]);
		}
		std::sort(V.begin(), V.end());
		V.erase(unique(V.begin(), V.end()), V.end());
		cnt = V.size();
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
	while (Q--) { q = query(P, L, U); q1 += q == 1; }
	assert(q1 < 105);
	return;
}
int main() { solve(); return 0; }//boj33441
