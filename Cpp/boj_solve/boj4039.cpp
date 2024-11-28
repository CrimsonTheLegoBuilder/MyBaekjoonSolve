#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-9;
const ld PI = acosl(-1);
const int LEN = 5005;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }

int N, F;
struct Pos {
	int x, y;
	int c;
	Pos(int X = 0, int Y = 0, int C = 1) : x(X), y(Y), c(C) {}
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
	Pos& operator *= (const int& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const int& n) { x /= n; y /= n; return *this; }
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
} A; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
bool cmp(const Pos& p, const Pos& q) {
	bool f1 = O < p;
	bool f2 = O < q;
	if (f1 != f2) return f1;
	ll ccw = p / q;
	return ccw ? ccw > 0 : p.Euc() < q.Euc();
}
bool query(int t) {
	std::cin >> N >> A >> F;
	if (!N && A == O && !F) return 0;
	ld r = F / PI;
	Polygon P(N);
	for (Pos& p : P) std::cin >> p, p -= A;
	std::sort(P.begin(), P.end());
	std::vector<bool> V(N, 1);
	for (int i = 0, j; i < N - 1; i = j) {
		j = i + 1;
		while (j < N && P[i] / P[j]) V[j] = 0, j++;
		P[i].c += j - i - 1;
	}
	Polygon H;
	for (int i = 0; i < N; i++) if (V[i]) H.push_back(P[i]);
	N = H.size();
	int ret = 1000;
	for (int k = 0; k < N; k++) {
		int cnt = 0;
		for (int i = 0, j = 0; i < N; i = j) {
			while (j < N && norm(rad(H[i], H[j])) < r) j++;
			cnt++;
		}
		ret = std::min(ret, cnt);
		rotate(H.begin(), H.begin() + 1, H.end());
	}
	std::cout << "Case " << t << ": " << ret << "\n";
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int T = 1;
	while (query(T++));
	return;
}
int main() { solve(); return 0; }//boj4039