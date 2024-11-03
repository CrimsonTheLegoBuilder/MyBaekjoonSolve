#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const int LEN = 105;
const ld MAX = 1e17;
const ld TOL = 1e-8;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

ld C[LEN];
int N, M, K, T;
struct Info {
	int i;
	ld c;
	Info(int i_ = 0, ld c_ = 0) : i(i_), c(c_) {}
	bool operator < (const Info& w) const { return c > w.c; }
};
std::vector<Info> G[LEN];
std::priority_queue<Info> Q;
ld dijkstra(int s, int e = 1) {
	for (int i = 0; i < N + 2; i++) C[i] = MAX;
	C[s] = 0;
	Q.push({ s, 0 });
	while (!Q.empty()) {
		Info p = Q.top(); Q.pop();
		if (p.c > C[p.i]) continue;
		for (const Info& w : G[p.i]) {
			ld cost = p.c + w.c;
			if (C[w.i] > cost) {
				C[w.i] = cost;
				Q.push({ w.i, cost });
			}
		}
	}
	return C[e];
}
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	ld dist, c;
	Pos s, e;
	std::cin >> s >> e >> N;
	Polygon P(N);
	for (Pos& p : P) std::cin >> p;
	G[0].push_back(Info(1, (s - e).mag() / 5));
	for (int i = 0; i < N; i++) {
		const Pos& I = P[i];
		G[0].push_back(Info(i + 2, (s - I).mag() / 5));
		dist = (e - I).mag();
		if (sign(dist - 50) >= 0) c = 2 + (dist - 50) / 5;
		else c = std::min(dist / 5, 2 + (50 - dist) / 5);
		G[i + 2].push_back(Info(1, c));
		for (int j = i + 1; j < N; j++) {
			const Pos& J = P[j];
			dist = (I - J).mag();
			if (sign(dist - 50) >= 0) c = 2 + (dist - 50) / 5;
			else c = std::min(dist / 5, 2 + (50 - dist) / 5);
			G[i + 2].push_back(Info(j + 2, c));
			G[j + 2].push_back(Info(i + 2, c));
		}
	}
	std::cout << dijkstra(0, 1) << "\n";
	return;
}
int main() { solve(); return 0; }//boj10473
