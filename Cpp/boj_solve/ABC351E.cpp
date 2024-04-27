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
	int Che() const { return std::max(std::abs(x), std::abs(y)); }
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

Polygon tmp;
std::vector<ll> ODD[2], EVEN[2];
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	tmp.resize(N);
	for (Pos& p : tmp) {
		std::cin >> p;
		if (p.Man() & 1) ODD[0].push_back(p.x + p.y), ODD[1].push_back(p.x - p.y);
		else EVEN[0].push_back(p.x + p.y), EVEN[1].push_back(p.x - p.y);
	}
	ll ret = 0;
	for (int i = 0; i < 2; i++) {
		std::sort(ODD[i].begin(), ODD[i].end());
		int sz = ODD[i].size();
		for (int j = 0; j < sz; j++) ret += ODD[i][j] * (2 * j + 1 - sz);
	}
	for (int i = 0; i < 2; i++) {
		std::sort(EVEN[i].begin(), EVEN[i].end());
		int sz = EVEN[i].size();
		for (int j = 0; j < sz; j++) ret += EVEN[i][j] * (2 * j + 1 - sz);
	}
	std::cout << (ret / 2) << "\n";
	return;
}
int main() { solve(); return 0; }
