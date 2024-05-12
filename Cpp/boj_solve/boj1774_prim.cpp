#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <numeric>
#include <unordered_map>
typedef long long ll;
typedef double db;
typedef long double ld;
const db INF = 1e18;
const db TOL = 1e-7;
const db PI = acos(-1);
const int LEN = 1005;
int N, M, T, Q;
inline bool zero(const db& x) { return std::abs(x) < TOL; }
inline int sign(const db& x) { return x < -TOL ? -1 : x > TOL; }
inline db sqr(const db& x) { return x * x; }
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

ll V[LEN];
int P[LEN];
struct Info {
    int u, v;
    db c;
    Info(int u = 0, int v = 0, db c = 0) : u(u), v(v), c(c) {}
    bool operator < (const Info& x) const { return c > x.c; }
};
int find(int v) {
    if (P[v] < 0) return v;
    int p = find(P[v]);
    return P[v] = p;
}
int join(int u, int v) {
    int i, j;
    i = find(u);
    j = find(v);
    if (i == j) return 0;
    if (P[i] < P[j]) {
        P[i] += P[j];
        P[j] = i;
    }
    else {
        P[j] += P[i];
        P[i] = j;
    }
    return 1;
}
void kruskal(std::vector<Info>& info) {
	std::sort(info.begin(), info.end());
	for (const Info& i : info) {
		int s = i.u, e = i.v;
		if (find(s) == find(e)) break;
		join(i.u, i.v);
	}
	return;
}
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
void solve() {
    std::cin.tie(0)->sync_with_stdio(0);
    std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	std::cin >> N >> M;

    return;
}
int main() { solve(); return 0; }//boj1774