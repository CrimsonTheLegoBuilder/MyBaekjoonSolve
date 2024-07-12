#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <random>
#include <array>
#include <tuple>
#include <set>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 3e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
bool zero(const ld& x) { return !sign(x); }
ll sq(int x) { return (ll)x * x; }
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
ll powmod(ll a, ll b) {
	ll res = 1; a %= MOD;
	assert(b >= 0);
	for (; b; b >>= 1) {
		if (b & 1) res = res * a % MOD;
		a = a * a % MOD;
	}
	return res;
}

int N, Q;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
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
typedef std::vector<Pos> Polygon;
typedef std::set<Pos> DynaPoly;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return ret < 0 ? -1 : !!ret; }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d1) / (d2 - d1).mag(); }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3) / (d2 - d1).mag(); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2, const bool f = 1) {
	//f : include end of seg, !f : ignore end of seg
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	if (!f) return f1 && f2;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
ll area(std::vector<Pos>& H, int lo = 1) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt);
	}
	return ret * lo;
}
Polygon half_monotone_chain(Polygon& C, int lo = 1) {
	std::vector<Pos> H;
	if (lo > 0) std::sort(C.begin(), C.end());
	//C.erase(unique(C.begin(), C.end()), C.end());
	if (C.size() <= 2) { for (const Pos& pos : C) H.push_back(pos); }
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) * lo <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
	}
	return H;
}
struct HalfHull {
	std::set<Pos> hull;
	int is_low;//low == 1, hi == -1
	ll a;
	ld r;

	HalfHull(int lo = 1) : is_low(lo) { a = 0; r = 0; }
	HalfHull(Polygon& H, int lo) : is_low(lo) {
		for (const Pos& p : H) hull.insert(p);
		a = area(H, is_low);
		r = 0;
		int sz = H.size();
		assert(sz > 1);
		for (int i = 0; i < sz - 1; i++) r += (H[i] - H[i + 1]).mag();
	}

	bool able_to_go_left(const Pos& p) {
		std::set<Pos>::iterator it = hull.find(p);
		if (it == hull.begin()) return 0;
		if (--it == hull.begin()) return 0;
		return 1;
	}
	Pos update_left(const Pos& p0, std::set<Pos>& S) {
		std::set<Pos>::iterator it = hull.find(p0);
		std::set<Pos>::iterator p1 = it, p2 = it;
		while (able_to_go_left(p0)) {
			it = hull.find(p0);
			p1 = --it;
			p2 = --it;

			if (ccw(p0, *p1, *p2) * is_low >= 0) {
				S.insert(*p1);
				hull.erase(p1);
			}
			else break;
		}
		it = hull.find(p0);
		if (it == S.begin()) return p0;
		--it;
		return *it;
	}

	bool able_to_go_right(const Pos& p) {
		std::set<Pos>::iterator it = hull.find(p);
		if (++it == hull.end()) return 0;
		if (++it == hull.end()) return 0;
		return 1;
	}
	Pos update_right(const Pos& p0, std::set<Pos>& S) {
		std::set<Pos>::iterator it = hull.find(p0);
		std::set<Pos>::iterator p1 = it, p2 = it;
		while (able_to_go_right(p0)) {
			it = hull.find(p0);
			p1 = ++it;
			p2 = ++it;

			if (ccw(p0, *p1, *p2) * is_low <= 0) {
				S.insert(*p1);
				hull.erase(p1);
			}
			else break;
		}
		it = hull.find(p0);
		++it;
		if (it == S.end()) return p0;
		return *it;
	}

	bool inner_check(const Pos& p) {
		std::set<Pos>::iterator it = hull.upper_bound(p);

		if (it == hull.end()) return 0;//right most
		if (it == hull.begin()) return 0;//left most

		std::set<Pos>::iterator nxt = it;
		std::set<Pos>::iterator prev = --it;

		return ccw(*prev, *nxt, p) * is_low >= 0;
	}

	inline bool exist(const Pos& p) { return hull.count(p); }

	void insert(Pos p) {
		assert(hull.size());
		if (!inner_check(p)) {
			std::set<Pos> S;
			//Since the maximum number of points to be removed is N, 
			//the time complexity is O(N log N) even after collection and sorting.
			hull.insert(p);

			Pos L, R;
			L = update_left(p, S);
			R = update_right(p, S);
			r += (p - L).mag();
			r += (p - R).mag();

			if (S.empty()) {
				if (p != L && p != R) r -= (L - R).mag();
				a += cross(p, L, R) * -is_low;
				return;
			}

			std::set<Pos>::iterator p0 = S.begin();

			r -= (L - *p0).mag();
			a += cross(p, L, *p0) * -is_low;

			std::set<Pos>::iterator it = p0;
			std::set<Pos>::iterator p1 = it, p2 = it;
			while (1) {
				p1 = it;
				p2 = ++it;
				if (p2 == S.end()) break;
				r -= (*p1 - *p2).mag();
				a += cross(p, *p1, *p2) * -is_low;
			}

			r -= (R - *p1).mag();
			a += cross(p, *p1, R) * -is_low;
		}
		return;
	}
};
struct DynamicHull {
	HalfHull U, L;
	DynamicHull(Polygon& C) {
		Polygon LH = half_monotone_chain(C);
		Polygon UH = half_monotone_chain(C, -1);
		L = HalfHull(LH, 1);
		U = HalfHull(UH, -1);
	}
	void insert(const Pos& p) { L.insert(p); U.insert(p); return; }
	ll round() const { return L.r + U.r; }
	ll area() const { return L.a + U.a; }
};
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	std::cin >> N;
	Polygon C(N);
	for (Pos& p : C) std::cin >> p;
	DynamicHull H = DynamicHull(C);
	std::cin >> Q;
	while (Q--) {
		Pos p;
		std::cin >> p;
		H.insert(p);
		ld R = H.round();
		ll A = H.area();
		std::cout << R << " ";
		std::cout << (A >> 1) << "." << ((A & 1) * 5) << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj30790 expansion construction
