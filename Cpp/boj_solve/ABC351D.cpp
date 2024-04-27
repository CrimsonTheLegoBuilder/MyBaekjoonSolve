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
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
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
int H, W;

std::queue<Pos> Q;
int bfs(const Pos& s = Pos(0, 0), int c = 1) {
	std::vector<Pos> tmp;
	auto vaild = [&](const Pos& p) -> bool {
		return 0 <= p.x && p.x < H && 0 <= p.y && p.y < W
			&& MAP[p.x][p.y] != '#'
			&& !V[p.x][p.y];
		};
	Q.push(s);
	tmp.push_back(s);
	while (Q.size()) {
		Pos v = Q.front(); Q.pop();
		for (Pos& i : dir) {
			Pos w = v + i;
			if (vaild(w)) {
				tmp.push_back(w);
				if (MAP[w.x][w.y] == '.') V[w.x][w.y] = c, Q.push(w);
			}
		}
	}
	std::sort(tmp.begin(), tmp.end());
	tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
	return tmp.size();
}
void init() {
	auto bad = [&](const Pos& p) -> bool {
		return 0 <= p.x && p.x < H && 0 <= p.y && p.y < W
			&& MAP[p.x][p.y] == '#';
		};
	auto valid = [&](const Pos& p) -> bool {
		return 0 <= p.x && p.x < H && 0 <= p.y && p.y < W
			&& MAP[p.x][p.y] != '#';
		};
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> H >> W;
	for (int i = 0; i < H; i++) std::cin >> MAP[i];
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			Pos cur = Pos(i, j);
			if (bad(cur)) {
				for (const Pos& d : dir) {
					Pos nxt = cur + d;
					if (valid(nxt))
						MAP[nxt.x][nxt.y] = 'X';
				}
			}
		}
	}
	return;
}
void solve() {
	init();
	memset(V, 0, sizeof V);
	int cnt = 1;
	int ret = 1;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			Pos cur = Pos(i, j);
			if (MAP[i][j] == '.' && !V[i][j])
				ret = std::max(ret, bfs(cur, cnt++));
		}
	}
	std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }
