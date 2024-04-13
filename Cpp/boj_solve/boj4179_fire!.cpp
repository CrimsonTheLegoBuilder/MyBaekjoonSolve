#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <random>
#include <array>
#include <tuple>
typedef long long ll;
typedef long double ld;
typedef std::pair<int, int> pi;
const ll INF = 1e17;
const int LIMIT = 1e9;
const int LEN = 1e3 + 5;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
int N, M;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
//ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
ll gcd(ll a, ll b) { while (b) { ll tmp = a % b; a = b; b = tmp; } return a; }

struct Pos {
	int x, y;
	ll c;
	Pos(int X = 0, int Y = 0, ll C = 0) : x(X), y(Y), c(C) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { -x, -y }; }
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
typedef std::vector<Pos> Polygon;
std::vector<Pos> dir = { Pos(1, 0), Pos(0, 1), Pos(-1, 0), Pos(0, -1) };
int V[LEN][LEN];
char MAP[LEN][LEN];
std::queue<Pos> Q;
void bfs(const Pos& s = Pos(0, 0), bool f = 0) {
	auto vaild = [&](const Pos& p) -> bool {
		return 0 <= p.x && p.x < N && 0 <= p.y && p.y < M 
			&& MAP[p.x][p.y] != '#'
			&& !V[p.x][p.y];
		};
	if (f) Q.push(s), V[s.x][s.y] = 1;
	while (Q.size()) {
		Pos v = Q.front(); Q.pop();
		for (Pos& i : dir) {
			Pos w = v + i;
			if (vaild(w)) Q.push(w), V[w.x][w.y] = V[v.x][v.y] + 1;
		}
	}
	return;
}
std::vector<Pos> tmp;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	for (int i = 0; i < N; i++) std::cin >> MAP[i];
	Pos s(0, 0);
	memset(V, 0, sizeof V);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++) {
			if (MAP[i][j] == 'F') Q.push(Pos(i, j)), V[i][j] = 1;
			if (MAP[i][j] == 'J') s = Pos(i, j);
		}
	if (!s.x || !s.y || s.x == N - 1 || s.y == M - 1) {
		std::cout << "1\n";
		while (Q.size()) Q.pop();
		return;
	}
	bfs();
	for (int i = 0; i < N; i++) {
		if (MAP[i][0] != '#')
			tmp.push_back(Pos(i, 0, V[i][0]));
		if (MAP[i][M - 1] != '#')
			tmp.push_back(Pos(i, M - 1, V[i][M - 1]));
	}
	for (int j = 1; j < M - 1; j++) {
		if (MAP[0][j] != '#')
			tmp.push_back(Pos(0, j, V[0][j]));
		if (MAP[N - 1][j] != '#')
			tmp.push_back(Pos(N - 1, j, V[N - 1][j]));
	}
	memset(V, 0, sizeof V);
	bfs(s, 1);
	int ret = LIMIT;
	for (Pos& p : tmp) {
		if (V[p.x][p.y] && (V[p.x][p.y] < p.c || !p.c))
			ret = std::min(ret, V[p.x][p.y]);
	}
	if (ret == LIMIT) std::cout << "IMPOSSIBLE\n";
	else std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }

/*

5 5
.....
####.
..J#F
####.
.....

3

5 5
.....
.F.F.
..J..
.F.F.
.....

IM

5 5
#####
#...#
#.J.#
#...#
#####

IM


7 7
....#.#
.F..#.#
...F#.#
#####.#
.....J.
#######
....F..

2

*/