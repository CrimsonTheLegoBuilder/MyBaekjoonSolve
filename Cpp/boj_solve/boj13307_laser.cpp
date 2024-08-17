#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <cassert>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ll INF = 1e18;
const int LEN = 1e3;
const ld TOL = 1e-9;
const ld PI = acos(-1);
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL ? 1 : 0; }
inline bool zero(const ld& x) { return !sign(x); }

int N, F;
struct Pos {
	int x, y;
	int i, c;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) { i = 0; c = 0; }
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} pivot;
const Pos O = { 0, 0 };
const Pos INVAL = Pos(-1e9, -1e9);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
bool cmpc(const Pos& p, const Pos& q) { return p.c > q.c; }
bool cmpr(const Pos& p, const Pos& q) {
	ll ret = cross(pivot, p, q);
	//if (!ret) return (pivot - p).Euc() < (pivot - q).Euc();
	assert(ret);
	return ret > 0;
}
Polygon graham_scan(Polygon& C) {
	Polygon H;
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
struct Pair {
	int j, k;
	Pair(int J = 0, int K = 0) : j(J), k(K) {}
	friend std::ostream& operator << (std::ostream& os, const Pair& p) { os << p.j << " " << p.k; return os; }
} ANS[LEN];
void solve(Polygon& P) {
	int sz = P.size();
	assert(!(sz % 3));
	if (!sz) return;
	if (sz == 3) {
		std::sort(P.begin(), P.end(), cmpc);
		assert(P[0].c == 2); assert(P[1].c == -1); assert(P[2].c == -1);
		ANS[P[0].i] = Pair(P[1].i, P[2].i);
		return;
	}
	Polygon H = graham_scan(P);
	Pos B = INVAL;//blue
	int cnt = 0;
	for (const Pos& p : H) if (p.c == 2) { B = p; break; }
	std::vector<Pos>().swap(H);
	if (B == INVAL) {//all Pos are red in the convex hull
		Polygon U, L;
		cnt = -1;
		for (int i = 1; i < sz; i++) {
			cnt += P[i].c;
			if (!cnt) {
				for (int j = 0; j <= i; j++) L.push_back(P[j]);
				for (int j = i + 1; j < sz; j++) U.push_back(P[j]);
				if (!U.size() || !L.size()) break;
				std::vector<Pos>().swap(P);
				solve(U); solve(L);
				return;
			}
		}
		U.clear();
		L.clear();
		cnt = -1;
		for (int i = sz - 1; i > 0; i--) {
			cnt += P[i].c;
			if (!cnt) {
				for (int j = 1; j < i; j++) L.push_back(P[j]);
				for (int j = i; j <= sz; j++) U.push_back(P[j % sz]);
				if (!U.size() || !L.size()) break;
				std::vector<Pos>().swap(P);
				solve(U); solve(L);
				return;
			}
		}
		F = 0;
		return;
	}
	//else if at least 1 blue is in the convex hull
	for (int i = 0; i < sz; i++) {
		if (P[i] == B) {
			std::swap(P[0], P[i]); break;
		}
	}
	pivot = P[0];
	std::sort(P.begin() + 1, P.end(), cmpr);
	Polygon U, M, L;
	Pair cur;
	int f = -1, pre = -1;
	for (int i = 1; i < sz; i++) {
		cnt += P[i].c;
		if (cnt == -1 && f == -1) {
			cur.j = P[i].i;
			pre = i; f--;
			continue;
		}
		if (cnt == -2 && f == -2) {
			cur.k = P[i].i;
			ANS[P[0].i] = cur;
			for (int j = 1; j < pre; j++) L.push_back(P[j]);
			for (int j = pre + 1; j < i; j++) M.push_back(P[j]);
			for (int j = i + 1; j < sz; j++) U.push_back(P[j]);
			std::vector<Pos>().swap(P);
			solve(L); solve(M); solve(U);
			return;
		}
	}
	F = 0;
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	Polygon P(N * 3);
	for (int i = 0; i < N; i++) {
		std::cin >> P[i];
		P[i].i = i;
		P[i].c = 2;
	}
	for (int i = N; i < N * 3; i++) {
		std::cin >> P[i];
		P[i].i = i - N + 1;
		P[i].c = -1;
	}
	F = 1;
	solve(P);
	if (!F) std::cout << "-1\n";
	else for (int i = 0; i < N; i++) std::cout << ANS[i] << "\n";
	return;
}
int main() { solve(); return 0; }//boj13307