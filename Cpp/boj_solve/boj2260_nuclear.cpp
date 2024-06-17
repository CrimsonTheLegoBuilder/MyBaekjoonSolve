#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>
typedef long long ll;
typedef double ld;
const int INF = 1e9;
const int LEN = 101;

int N, M, T, Q;
int G[LEN][LEN];
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} nu;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
struct Seg {
	Pos s, e;
	int cost;
	Seg(Pos S = Pos(), Pos E = Pos(), int COST = 0) : cost(COST) {
		s = S, e = E;
		if (on_seg_strong(s, nu, e)) std::swap(s, e);
		if (ccw(nu, s, e) < 0) std::swap(s, e);
	}
} seg[LEN];
bool check(const Seg& S, const Seg& E) {
	bool f1 = S.e == E.s;
	int f2 = ccw(S.s, S.e, E.e);
	return f1 && f2 >= 0;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> nu;
	for (int i = 0; i < N; i++) {
		Pos s, e; int cost;
		std::cin >> s >> e >> cost;
		assert(!on_seg_strong(s, e, nu));
		seg[i] = Seg(s, e, cost);
	}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			G[i][j] = check(seg[i], seg[j]) ? seg[i].cost : INF;

	for (int k = 0; k < N; k++)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				G[i][j] = std::min(G[i][j], G[i][k] + G[k][j]);

	int ret = INF;
	for (int i = 0; i < N; i++)
		for (int j = i + 1; j < N; j++)
			if (check(seg[i], seg[j]) || check(seg[j], seg[i]))
				ret = std::min(ret, G[i][j] + G[j][i]);

	std::cout << (ret > 1e8 ? -1 : ret) << "\n";
	return;
}
int main() { solve(); return 0; }//boj2260 nuclear bomb
/*
달디달고 달디단
bomb 양갱
bomb 양갱
bomb 양갱이야~~~
*/