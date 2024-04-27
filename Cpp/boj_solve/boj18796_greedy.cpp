#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
typedef double ld;
const ll INF = 1e17;
const int LEN = 5e5 + 1;
const ld TOL = 1e-7;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
int sign(const ll& x) { return x < 0 ? -1 : !!x; }

struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	inline ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
inline ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return ret < 0 ? -1 : !!ret;
}

int N, M, Q, T;
ll A[LEN], B[LEN];
ll diff_A[LEN], diff_B[LEN];
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	N++; M++;
	for (int i = 0; i < N; i++) std::cin >> A[i];
	for (int j = 0; j < M; j++) std::cin >> B[j];
	for (int i = 0; i < N - 1; i++) diff_A[i] = A[i + 1] - A[i];
	for (int j = 0; j < M - 1; j++) diff_B[j] = B[j + 1] - B[j];
	ll ret = A[0] * ((ll)M - 1) + B[0] * ((ll)N - 1);
	return;
}
int main() { solve(); return 0; }
