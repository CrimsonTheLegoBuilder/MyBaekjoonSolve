#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <set>
typedef long long ll;
typedef double ld;
const ll INF = 1e17;
const int LEN = 1e6 + 5;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }

//#define TEST
//#define ASSERT
#ifdef ASSERT
#include <cassert>
#include <unordered_set>
#endif

int N, M, T, Q;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	//bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator < (const Pos& p) const { return x < p.x; }
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
ll area(std::vector<Pos>& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt);
	}
	return ret;
}
//condition that must be observed : all coord's x or y with no same value
//https://github.com/jonathanirvings/inc-2017/blob/master/polygonal/solution.cpp
//Solution by William Gozali
class DynamicHull {
private:
	const int OFFSET = 1e6 + 1;//constrain all coordinates from 1 to 1'000'001
	bool is_low;
	std::set<Pos> hull;
	std::vector<int> idx_;//fenwick tree

	inline Pos transform(const Pos& p) {
		if (is_low) return p + Pos(1, 1);//make non zero
		else return -p + Pos(OFFSET, OFFSET);//for make upper hull && make non zero
	}
	inline Pos restore(const Pos& t) {
		if (is_low) return t - Pos(1, 1);
		else return -t + Pos(OFFSET, OFFSET);
	}

	inline int search(int i) {
		int ret = 0;
		for (; i; i -= i & -i) ret += idx_[i];
		return ret;
	}
	inline void update_idx(int i, int n) {
		for (; i <= OFFSET; i += i & -i) idx_[i] += n;
		return;
	}

	bool able_to_go_left(const Pos& p) {
		std::set<Pos>::iterator it = hull.find(p);
		if (it == hull.begin()) return 0;
		it--;
		if (it == hull.begin()) return 0;
		return 1;
	}
	void update_left(const Pos& p0) {
		while (able_to_go_left(p0)) {
			std::set<Pos>::iterator it = hull.find(p0);
			std::set<Pos>::iterator p1 = --it;
			std::set<Pos>::iterator p2 = --it;

			if (ccw(p0, *p1, *p2) >= 0) {
				update_idx(p1->x, -1);
				hull.erase(p1);
			}
			else break;
		}
	}

	bool able_to_go_right(const Pos& p) {
		std::set<Pos>::iterator it = hull.find(p);
		it++;
		if (it == hull.end()) return 0;
		it++;
		if (it == hull.end()) return 0;
		return 1;
	}
	void update_right(const Pos& p0) {
		while (able_to_go_right(p0)) {
			std::set<Pos>::iterator it = hull.find(p0);
			std::set<Pos>::iterator p1 = ++it;
			std::set<Pos>::iterator p2 = ++it;

			if (ccw(p0, *p1, *p2) <= 0) {
				update_idx(p1->x, -1);
				hull.erase(p1);
			}
			else break;
		}
	}

	bool inner_check(const Pos& p) {
		std::set<Pos>::iterator it = hull.upper_bound(p);

		if (it == hull.end()) return 0;//right most
		if (it == hull.begin()) return 0;//left most

		std::set<Pos>::iterator nxt = it;
		std::set<Pos>::iterator prev = --it;

		return ccw(*prev, *nxt, p) >= 0;
	}

	inline bool exist(const Pos& p) { return hull.count(transform(p)); }

	inline int len(const Pos& p) { return search(transform(p).x); }

	inline int size() { return hull.size(); }

	ll area() {
		if (hull.size() <= 2) return 0;
		ll ret = 0;
		std::set<Pos>::iterator p0 = hull.begin();
		std::set<Pos>::iterator it = p0;
		while (1) {
			std::set<Pos>::iterator p1 = it;
			std::set<Pos>::iterator p2 = ++it;
			if (p2 == hull.end()) break;
			ret += cross(*p0, *p1, *p2);
		}
		return ret;
	}

public:
	DynamicHull(bool lo) : is_low(lo) { idx_.resize(OFFSET + 5, 0); };

	inline void insert(Pos p) {
		p = transform(p);

		if (hull.size() <= 1) {
			hull.insert(p);
			update_idx(p.x, 1);
		}
		else if (!inner_check(p)) {
			hull.insert(p);
			update_idx(p.x, 1);
			update_left(p);
			update_right(p);
		}
		return;
	}

	inline friend int idx(DynamicHull& LH, DynamicHull& UH, const Pos& p) {
		if (LH.exist(p)) return LH.len(p) - 1;
		else if (UH.exist(p)) return LH.size() + UH.len(p) - 2;
		else return -1;
	}

	inline friend int total_len(DynamicHull& LH, DynamicHull& UH) {
		return LH.size() + UH.size() - 2;
	}

	inline friend ll area(DynamicHull& LH, DynamicHull& UH) {
		return LH.area() + UH.area();
	}
};

#ifdef ASSERT
std::unordered_set<int> x_coord;
std::unordered_set<int> y_coord;
int t = 0;
#endif

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
#ifdef TEST
	freopen("../../../input_data/polygonal/polygonal_6.in", "r", stdin);
	freopen("../../../input_data/polygonal/polygonal_out.txt", "w", stdout);
	std::vector<bool> ret;
	std::vector<Pos> V;
#endif
	std::cin >> Q;

	DynamicHull LH(1);
	DynamicHull UH(0);

	while (Q--) {
		std::cin >> N;
		if (!N) {
			Pos p;
			std::cin >> p;
			LH.insert(p);
			UH.insert(p);
#ifdef ASSERT
			t++;
			x_coord.insert(p.x);
			y_coord.insert(p.y);
			assert(t == x_coord.size());
			assert(t == y_coord.size());
#endif
#ifdef TEST
			V.push_back(p);
#endif
		}
		else {
#ifdef ASSERT
			assert(t >= 3);
#endif
			Pos p, q;
			std::cin >> p >> q;
			int a, b;
			a = idx(LH, UH, p);
			b = idx(LH, UH, q);
			int diff = b - a;
			if (diff < 0) diff += total_len(LH, UH);
#ifndef TEST
			if (diff * 2 > total_len(LH, UH)) std::cout << "CCW\n";
			else std::cout << "CW\n";
#else
			ret.push_back(diff * 2 > total_len(LH, UH));
#endif
		}
	}
#ifdef TEST
	freopen("../../../input_data/polygonal/polygonal_6.out", "r", stdin);
	int sz = ret.size();
	T = sz;
	for (int i = 0; i < sz; i++) {
		std::string ans;
		std::cin >> ans;
		bool f = ans != "CCW";
		T -= f != ret[i];
	}
	std::cout << sz << " " << T << "\n";
	std::cout << area(LH, UH) << "\n";
	auto H = graham_scan(V);
	std::cout << area(H) << "\n";
#endif
	return;
}
int main() { solve(); return 0; }// INC 2017 C boj21037 refer to jonathanirvings
