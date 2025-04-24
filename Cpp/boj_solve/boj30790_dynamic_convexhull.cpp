#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <set>
typedef long long ll;
//typedef long double ld;
typedef double ld;

int N, Q;
struct Pos {
	int x, y;
	inline Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	inline bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	inline bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	inline bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	//inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	//inline Pos operator * (const int& n) const { return { x * n, y * n }; }
	//inline ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	inline ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	inline Pos operator - () const { return { -x, -y }; }
	inline ld mag() const { return hypotl(x, y); }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
inline ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
struct HalfHull {
	std::set<Pos> hull;
	int is_low;//low == 1, hi == -1
	ll a;
	ld r;

	inline HalfHull(int lo = 1) : is_low(lo) { a = 0; r = 0; }

	inline bool able_to_go_left(const Pos& p) {
		std::set<Pos>::iterator it = hull.find(p);
		if (it == hull.begin()) return 0;
		if (--it == hull.begin()) return 0;
		return 1;
	}
	inline Pos update_left(const Pos& p0, Polygon& DEL) {
		std::set<Pos>::iterator it = hull.find(p0);
		std::set<Pos>::iterator p1 = it, p2 = it;
		while (able_to_go_left(p0)) {
			it = hull.find(p0);
			p1 = --it;
			p2 = --it;

			if (ccw(p0, *p1, *p2) * is_low >= 0) {
				DEL.push_back(*p1);
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
	inline Pos update_right(const Pos& p0, Polygon& DEL) {
		std::set<Pos>::iterator it = hull.find(p0);
		std::set<Pos>::iterator p1 = it, p2 = it;
		while (able_to_go_right(p0)) {
			it = hull.find(p0);
			p1 = ++it;
			p2 = ++it;

			if (ccw(p0, *p1, *p2) * is_low <= 0) {
				DEL.push_back(*p1);
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

	inline void insert(const Pos& p) {
		if (hull.empty()) { hull.insert(p); return; }
		if (inner_check(p)) return;
		hull.insert(p);

		Polygon D;
		//Since the maximum number of points to be removed is N, 
		//the time complexity is O(N log N) even after collection and sorting.

		Pos L = update_left(p, D);
		Pos R = update_right(p, D);
		r += (p - L).mag();
		r += (p - R).mag();

		if (D.empty()) {
			if (p != L && p != R) r -= (L - R).mag();
			a += cross(p, L, R) * -is_low;
			return;
		}

		std::sort(D.begin(), D.end());

		if (p != L) r -= (L - D[0]).mag();
		a += cross(p, L, D[0]) * -is_low;

		int sz = D.size();
		for (int i = 0; i < sz - 1; i++) {
			Pos& p1 = D[i], & p2 = D[i + 1];
			r -= (p1 - p2).mag();
			a += cross(p, p1, p2) * -is_low;
		}

		if (p != R) r -= (D.back() - R).mag();
		a += cross(p, D.back(), R) * -is_low;

		return;
	}
};
struct DynamicHull {
	HalfHull L, U;
	inline DynamicHull() { L = HalfHull(1); U = HalfHull(-1); }
	inline void insert(const Pos& p) { L.insert(p); U.insert(p); return; }
	inline ld round() const { return L.r + U.r; }
	inline ll area() const { return L.a + U.a; }
};
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	DynamicHull H = DynamicHull();
	Pos p;
	std::cin >> N;
	while (N--) { std::cin >> p; H.insert(p); }
	std::cin >> Q;
	while (Q--) {
		std::cin >> p;
		H.insert(p);
		ll A = H.area();
		std::cout << H.round() << " ";
		std::cout << (A >> 1) << "." << ((A & 1) * 5) << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj30790 expansion construction