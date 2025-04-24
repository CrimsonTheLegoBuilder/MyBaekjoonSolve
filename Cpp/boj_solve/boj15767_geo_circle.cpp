#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <unordered_map>
typedef long long ll;
typedef double ld;
typedef std::vector<int> Vint;
#define radius x
#define idx y
const int LEN = 3e5 + 5;
inline ll sq(const ll& x) { return x * x; }

int N;
int V[LEN];
struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
typedef std::vector<Pos> Polygon;
struct Circle {
	Pos c;
	ll r;
	Circle(Pos C = Pos(0, 0), ll R = 0) : c(C), r(R) {}
	bool meet(const Circle& q) const { return sq(r + q.r) >= (c - q.c).Euc(); }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
};
bool cmpri(const Pos& p, const Pos& q) { return p.radius == q.radius ? p.idx < q.idx : p.radius > q.radius; }
typedef std::vector<Circle> Disks;
typedef std::unordered_map<ll, Vint> Table;
Table idxs;
ll key(const Pos& p, const int& ex) { return ((p.x / (1ll << ex)) << 32) | (p.y / (1ll << ex)); }
void grid_init(Table& T, const Disks& C, const int& ex) {
	T.clear();
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		if (V[i]) continue;
		const Pos& p = C[i].c;
		T[key(p, ex)].push_back(i);
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N;
	Polygon R(N);
	Disks C(N);
	Pos w = Pos(1ll << 30, 1ll << 30);
	for (int i = 0; i < N; i++) {
		std::cin >> C[i];
		C[i].c += w;
		R[i] = Pos(C[i].r, i);
	}
	std::sort(R.begin(), R.end(), cmpri);
	memset(V, 0, sizeof V);
	int ex = 30;//exponent of the interval dividing the grid
	while ((1ll << ex) >= (R[0].radius) * 2) ex--;
	grid_init(idxs, C, ex);
	for (int i = 0; i < N; i++) {
		if (V[R[i].idx]) continue;
		if ((1ll << ex) >= (R[i].radius) * 2) {
			while ((1ll << ex) >= (R[i].radius) * 2) ex--;
			grid_init(idxs, C, ex);
		}
		int I = R[i].idx;
		Pos p = C[I].c;
		for (int x = -2; x <= 2; x++) {
			for (int y = -2; y <= 2; y++) {
				ll dx = (1ll << ex) * x, dy = (1ll << ex) * y;
				ll K = key(p + Pos(dx, dy), ex);
				if (!idxs.count(K)) continue;
				const Vint& vi = idxs[K];
				for (const int& v : vi) {
					if (V[v]) continue;
					if (C[I].meet(C[v])) V[v] = I + 1;
				}
			}
		}
	}
	for (int i = 0; i < N; i++) std::cout << V[i] << " ";
	return;
}
int main() { solve(); return 0; }//APIO 2018 B boj15767 Circle selection
//refer to jjang36524
//https://jjang36524.tistory.com/26
//https://codeforces.com/blog/entry/59650

/*

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <unordered_map>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
#define radius x
#define idx y
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 3e5 + 5;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline ll sq(const ll& x) { return x * x; }

int N, M, Q;
int V[LEN];
struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
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
	ll Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
struct Circle {
	Pos c;
	ll r;
	Circle(Pos C = Pos(0, 0), ll R = 0) : c(C), r(R) {}
	bool operator == (const Circle& q) const { return c == q.c && r == q.r; }
	bool operator != (const Circle& q) const { return !(*this == q); }
	bool operator < (const Circle& q) const { return r < q.r && (c - q.c).Euc() <= sq(q.r - r); }
	bool operator <= (const Circle& q) const { return *this < q || *this == q; }
	bool meet(const Circle& q) const { return sq(r + q.r) >= (c - q.c).Euc(); }
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	ld A() const { return 1. * r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
} INVAL = { { 0, 0 }, -1 };
bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
bool cmpri(const Pos& p, const Pos& q) { return p.radius == q.radius ? p.idx < q.idx : p.radius > q.radius; }
typedef std::vector<Circle> Disks;
typedef std::unordered_map<ll, Vint> Table;
Table idxs;
ll key(const Pos& p, const int& ex) { return ((p.x / (1ll << ex)) << 32) | (p.y / (1ll << ex)); }
void grid_init(Table& T, const Disks& C, const int& ex) {
	T.clear();
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		if (V[i]) continue;
		const Pos& p = C[i].c;
		T[key(p, ex)].push_back(i);
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N;
	Polygon R(N);
	Disks C(N);
	Pos w = Pos(1ll << 30, 1ll << 30);
	for (int i = 0; i < N; i++) {
		std::cin >> C[i];
		C[i].c += w;
		R[i] = Pos(C[i].r, i);
	}
	std::sort(R.begin(), R.end(), cmpri);
	memset(V, 0, sizeof V);
	int ex = 30;//exponent of the interval dividing the grid
	while ((1ll << ex) >= (R[0].radius) * 2) ex--;
	grid_init(idxs, C, ex);
	for (int i = 0; i < N; i++) {
		if (V[R[i].idx]) continue;
		if ((1ll << ex) >= (R[i].radius) * 2) {
			while ((1ll << ex) >= (R[i].radius) * 2) ex--;
			grid_init(idxs, C, ex);
		}
		int I = R[i].idx;
		Pos p = C[I].c;
		for (int x = -2; x <= 2; x++) {
			for (int y = -2; y <= 2; y++) {
				ll dx = (1ll << ex) * x, dy = (1ll << ex) * y;
				ll K = key(p + Pos(dx, dy), ex);
				if (!idxs.count(K)) continue;
				Vint& vi = idxs[K];
				for (const int& v : vi) {
					if (V[v]) continue;
					if (C[I].meet(C[v])) V[v] = I + 1;
				}
			}
		}
	}
	for (int i = 0; i < N; i++) std::cout << V[i] << " ";
	return;
}
int main() { solve(); return 0; }//APIO 2018 B boj15767 Circle selection
//refer to jjang36524
//https://jjang36524.tistory.com/26
//https://codeforces.com/blog/entry/59650

*/