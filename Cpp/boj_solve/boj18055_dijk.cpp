#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 101;
const ld TOL = 1e-7;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& u, const ld& v) { return zero(u - v); }
inline int sq(const int& x) { return x * x; }

int N, M, H, W, R, S;
char B[LEN][LEN];
struct Pos {
	int x, y;
	int c;
	Pos(int X = 0, int Y = 0, int C = 0) : x(X), y(Y), c(C) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	int Euc() const { return sq(x) + sq(y); }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
struct Info {
	int i, c;
	ld d;
	Info(int i_ = 0, int c_ = 0, ld d_ = 0) : i(i_), c(c_), d(d_) {}
	bool operator < (const Info& w) const { return eq(d, w.d) ? c > w.c : d > w.d; }
};
std::vector<Info> G[1 << 10];
std::priority_queue<Info> Q;
ld C[1 << 10][1 << 13];
void dijkstra(const int& s, const Pos& v) {
	Q.push(Info(s, v.c, 0));
	while (!Q.empty()) {
		Info p = Q.top(); Q.pop();
		if (p.i == 0) continue;//fin
		if (p.d > C[p.i][p.c]) continue;
		for (const Info& w : G[p.i]) {
			int c = p.c + w.c;
			ld d = p.d + w.d;
			if (c > M) continue;
			if (C[w.i][c] > d) {
				C[w.i][c] = d;
				for (int j = c + 1; j <= M; j++) {
					if (C[w.i][j] < d) break;
					C[w.i][j] = d;
				}
				Q.push(Info(w.i, c, d));
			}
		}
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> H >> W >> R >> S;
	R = sq(R);
	Polygon P;
	for (int i = 0; i < H; i++) {
		std::cin >> B[i];
		for (int j = 0; j < W; j++) {
			if (B[i][j] != '.') P.push_back(Pos(i, j, B[i][j] - '0'));
		}
	}
	N = P.size();
	M = std::min(N * 9, S);
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			const Pos& p = P[i], & q = P[j];
			int D = (p - q).Euc();
			if (D <= R) {
				ld d = (p - q).mag();
				G[i].push_back(Info(j, q.c, d));
				G[j].push_back(Info(i, p.c, d));
			}
		}
	}
	for (int i = 0; i < N; i++)
		for (int j = 0; j <= M; j++)
			C[i][j] = INF;
	for (int i = 0; i <= N; i++) std::sort(G[i].rbegin(), G[i].rend());
	for (int j = 0; j <= M; j++) C[N - 1][j] = P[N - 1].c;
	dijkstra(N - 1, P[N - 1]);
	ld ret = INF;
	for (int j = 0; j <= M; j++) ret = std::min(ret, C[0][j]);
	if (ret > 1e16) std::cout << "impossible\n";
	else std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }//boj18055