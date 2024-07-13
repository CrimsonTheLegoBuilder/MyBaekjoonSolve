#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <set>
typedef long long ll;
typedef long double ld;
//typedef double ld;

int N, Q;
struct Pos {
	int x, y;
	inline Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	inline bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	inline bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	inline bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline Pos operator * (const int& n) const { return { x * n, y * n }; }
	inline ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	inline ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	inline Pos operator - () const { return { -x, -y }; }
	inline ld mag() const { return hypotl(x, y); }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
typedef std::set<Pos> DynaPoly;
inline ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
inline ll area(const std::vector<Pos>& H, int lo = 1) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt);
	}
	return ret * lo;
}
inline Polygon half_monotone_chain(Polygon& C, int lo = 1) {
	Polygon H;
	if (lo > 0) std::sort(C.begin(), C.end());
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

	inline HalfHull(int lo = 1) : is_low(lo) { a = 0; r = 0; }
	inline HalfHull(Polygon& H, int lo) : is_low(lo) {
		for (const Pos& p : H) hull.insert(p);
		a = area(H, is_low);
		r = 0;
		int sz = H.size();
		//assert(sz > 1);
		for (int i = 0; i < sz - 1; i++) r += (H[i] - H[i + 1]).mag();
	}

	inline bool able_to_go_left(const Pos& p) {
		std::set<Pos>::iterator it = hull.find(p);
		if (it == hull.begin()) return 0;
		if (--it == hull.begin()) return 0;
		return 1;
	}
	inline Pos update_left(const Pos& p0, std::set<Pos>& S) {
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
		if (it == hull.begin()) return p0;
		--it;
		return *it;
	}

	inline bool able_to_go_right(const Pos& p) {
		std::set<Pos>::iterator it = hull.find(p);
		if (++it == hull.end()) return 0;
		if (++it == hull.end()) return 0;
		return 1;
	}
	inline Pos update_right(const Pos& p0, std::set<Pos>& S) {
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
		if (it == hull.end()) return p0;
		return *it;
	}

	inline bool inner_check(const Pos& p) {
		std::set<Pos>::iterator it = hull.upper_bound(p);

		if (it == hull.end()) return 0;//right most
		if (it == hull.begin()) return 0;//left most

		std::set<Pos>::iterator nxt = it;
		std::set<Pos>::iterator prev = --it;

		return ccw(*prev, *nxt, p) * is_low >= 0;
	}

	inline void insert(Pos p) {
		//assert(hull.size() >= 2);
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

			if (p != L) r -= (L - *p0).mag();
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

			if (p != R) r -= (R - *p1).mag();
			a += cross(p, *p1, R) * -is_low;
		}
		return;
	}
};
struct DynamicHull {
	HalfHull U, L;
	inline DynamicHull(Polygon& C) {
		Polygon LH = half_monotone_chain(C);
		Polygon UH = half_monotone_chain(C, -1);
		L = HalfHull(LH, 1);
		U = HalfHull(UH, -1);
	}
	inline void insert(const Pos& p) { L.insert(p); U.insert(p); return; }
	inline ld round() const { return L.r + U.r; }
	inline ll area() const { return L.a + U.a; }
};
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	std::cin >> N; //assert(N >= 3);
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
