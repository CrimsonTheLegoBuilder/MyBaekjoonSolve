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
const int LEN = 1500;
const ld TOL = 1e-9;
inline ll nCm(const int& n, const int& m) {
	if (n < m) return 0;
	if (m == 1) return n;
	if (m == 2) return (ll)n * ((ll)n - 1) / 2;
	if (m == 3) return (ll)n * ((ll)n - 1) * ((ll)n - 2) / 6;
	if (m == 4) return (ll)n * ((ll)n - 1) * ((ll)n - 2) * ((ll)n - 3) / 24;
	return -1;
}

int N;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} pivot;
const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
bool cmpr(const Pos& p, const Pos& q) {
	bool f1 = O < p;
	bool f2 = O < q;
	if (f1 != f2) return f1;
	ll CCW = p / q;
	return !CCW ? p.Euc() < q.Euc() : CCW > 0;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	std::cin >> N;
	Polygon C(N), D;
	ll total = 0;
	for (Pos& p : C) std::cin >> p;
	for (int i = 0; i < N; i++) {
		Polygon().swap(D);
		pivot = C[i];
		for (int j = 0; j < N; j++) 
			if (i != j) D.push_back(C[j] - pivot);
		std::sort(D.begin(), D.end(), cmpr);
		int sz = N - 1;
		for (int j = 0, k = 1; j < sz; j++) {
			while (D[j] / D[k] > 0) k = (k + 1) % sz;
			int r = k - j;
			if (r <= 0) r += sz;
			r--;
			total += nCm(r, 2);
		}
	}
	total -= 2 * nCm(N, 4);
	ld ans = total / (ld)nCm(N, 3) + 3;
	std::cout << ans << "\n";
	return;
}
int main() { solve(); return 0; }//boj4010 signaling
