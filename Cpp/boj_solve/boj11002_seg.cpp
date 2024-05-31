#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef double ld;
typedef std::vector<ld> vld;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
int N, M, T, Q;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }

//freopen("../../../input_data/triathlon_tests/triath.20", "r", stdin);
//freopen("../../../input_data/triathlon_tests/triathlon_out.txt", "w", stdout);
struct Pos {
	int x, y;
	int hi, i;
	Pos(int X = 0, int Y = 0, int HI = 0, int I = 0) : x(X), y(Y), hi(HI), i(I) {}
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
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
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
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
//bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	ll ret = cross(d1, d2, d3, d4);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0;
}
int collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return !ccw(d1, d2, d3) && !ccw(d1, d2, d4);
}
void get_round_memo(Polygon& H, vld& memo) {
	int sz = H.size();
	memo.resize(sz + 1, 0);
	memo[0] = .0;
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		memo[i + 1] = (cur - nxt).mag() + memo[i];//memo[sz] == convex hull's round
	}
	return;
}
std::vector<Pos> upper_monotone_chain(std::vector<Pos>& C) {
	Polygon H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) { for (const Pos& p : C) H.push_back(p); }
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && ccw(H[H.size() - 2], H.back(), C[i]) >= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
	}
	return H;
}
struct Seg {
	Pos s, e;
	int i;
	Seg(Pos s = Pos(), Pos e = Pos(), int I = 0) : s(s), e(e), i(I) {}
	Pos S() const { return e - s; }
	inline bool operator < (const Seg& p) const { return s == p.s ? e < p.e : s < p.s; }
	inline ll operator / (const Seg& p) const { return S() / p.S(); }
	inline int ccw(const Pos& p) const { return sign((e - s) / (p - e)); }
};
Seg upper_tangent_bi_search(const Polygon& L, const Polygon& R, const int& I) {
	if (!L.size() || !R.size()) return Seg(Pos(-1, -1), Pos(-1, -1), -INF);
	if (L.size() == 1 && R.size() == 1) return Seg(L[0], R[1], I);
	int szl = L.size(), szr = R.size();
	int l = szl - 1, r = 0;
	auto tangent_check = [&](const int& i, const int& j) -> bool {
		bool f1 = 0, f2 = 0, f3 = 0, f4 = 0;
		if (i > 0) f1 = ccw(L[i], R[j], L[i - 1]) <= 0;
		else f1 = 1;
		if (i < szl - 1) f2 = ccw(L[i], R[j], L[i + 1]) <= 0;
		else f2 = 1;
		if (j > 0) f3 = ccw(L[i], R[j], R[i - 1]) <= 0;
		else f3 = 1;
		if (j < szr - 1) f4 = ccw(L[i], R[j], R[i + 1]) <= 0;
		else f4 = 1;
		return f1 && f2 && f3 && f4;
		};
	bool f = 0;
	while (!tangent_check(l, r)) {
		int s, e, m;
		if (!f) {//r search
			s = r, e = szr - 1;
			while (s < e) {
				m = s + e >> 1;
				if (L[l] == R[m]) { e = m; continue; }
				if (ccw(L[l], R[m], R[m + 1]) > 0) s = m + 1;
				else e = m;
			}
			if (s > 0 && !ccw(L[l], R[s], R[s - 1])) s--;
			r = s;
		}
		else if (f) {//l search
			s = 0, e = l;
			while (s < e) {
				m = s + e >> 1;
				if (L[m] == R[r]) { s = m + 1; continue; }
				if (ccw(L[m], R[r], L[m + 1]) > 0) s = m + 1;
				else e = m;
			}
			l = s;
			if (s < szl - 1 && !ccw(L[l], R[s], R[s + 1])) s++;
		}
		f ^= 1;
	}
	if (r > 0 && !ccw(L[l], R[r], R[r - 1])) r--;
	if (l < szl - 1 && !ccw(L[l], R[r], L[l + 1])) l++;
	return Seg(L[l], R[r], I);
}
struct HullNode {
	Pos l, r;
	Polygon hull;
	vld memo;
	HullNode(Pos l = Pos(0, 0), Pos r = Pos(0, 0), Polygon h = {}) : l(l), r(r) {
		hull = h;
		get_round_memo(hull, memo);
	}
	HullNode operator + (const HullNode& R) const {
		Polygon C, H;
		for (const Pos& p : hull) C.push_back(p);
		for (const Pos& p : R.hull) C.push_back(p);
		H = upper_monotone_chain(C);
		return HullNode(H[0], H.back(), H);
	}
} hull_tree[LEN << 2];//semi-hull index tree (shit)
struct Bridge {
	int i;
	Pos s, e;
	Bridge(int I = 0, const HullNode& HN) : i(I) {
		s = HN.l;
		e = HN.r;
	}
	bool operator < (const Bridge& p) const { return s == p.s ? e < p.e : s < p.s; }
};
typedef std::vector<Bridge> BBB;