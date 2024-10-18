#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
//typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& p, const ld& q) { return zero(p - q); }
inline ll sq(ll x) { return (ll)x * x; }

int N;
ll L;
struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ll operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	ll Euc() const { return x * x + y * y; }
	ll Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
struct E {
	ld x;
	int f;
	E(ld x_ = 0, int f_ = 0) : x(x_), f(f_) {}
	bool operator < (const E& e) const {
		return eq(x, e.x) ? f > e.f : sign(x - e.x) > 0;
	}
};
bool sweep(const Polygon& B, const ld& d) {
	std::vector<E> tmp;
	Pos s = Pos(0, 0), e = Pos(L, 0);
	Pos vec = e - s;
	tmp.push_back(E(0, 0));
	tmp.push_back(E(1, 0));
	for (int i = 0; i < N; i++) {
		Pos OM = s - B[i];
		ld a = vec * vec;
		ld b = 2 * (vec * OM);
		ld c = OM * OM - d * d;
		ld J = b * b - 4 * a * c;
		if (J < TOL) continue;
		else {
			ld ret1 = (-b + sqrt(J)) / (2 * a);
			ld ret2 = (-b - sqrt(J)) / (2 * a);
			ret1 = std::min(ret1, (ld)1.);
			ret2 = std::min(ret2, (ld)1.);
			if (ret2 < ret1) std::swap(ret1, ret2);
			tmp.push_back(E(ret1, -1));
			tmp.push_back(E(ret2, 1));
		}
	}
	int toggle = 0;
	bool f = 0;
	std::sort(tmp.begin(), tmp.end());
	for (const E& d : tmp) {
		toggle -= d.f;
		if (!d.f) f = !f;
		if (f && toggle < 0) return 1;
	}
	return 0;
}
bool F(const Polygon& B, const ld& d) {
	std::vector<E> tmp;
	Pos s = Pos(0, 0), e = Pos(L, 0);
	Pos vec = e - s;
	tmp.push_back(E(0, 0));
	tmp.push_back(E(L, 0));
	for (int i = 0; i < N; i++) {
		ll y = std::abs(B[i].y);
		ld dx = sqrt(d * d - y * y);
		ld x0 = std::max((ld)0, B[i].x - dx);
		ld x1 = std::min((ld)L, B[i].x + dx);
		tmp.push_back(E(x0, -1));
		tmp.push_back(E(x1, 1));
	}
	int t = 0;
	bool f = 0;
	std::sort(tmp.begin(), tmp.end());
	for (const E& d : tmp) {
		t -= d.f;
		if (!d.f) f = !f;
		if (f && t < 0) return 1;
	}
	return 0;
}
ld bi_search(const Polygon& B) {
	int cnt = 50;
	ld s = 0, e = L;
	for (int i = 0; i < N; i++) s = std::max(s, (ld)std::abs(B[i].x));
	while (cnt--) {
		ld m = (s + e) * .5;
		//if (sweep(B, m)) s = m;
		if (F(B, m)) s = m;
		else e = m;
	}
	return (s + e) * .5;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N >> L;;
	Polygon B(N);
	for (Pos& p : B) std::cin >> p;
	std::cout << bi_search(B) << "\n";
	return;
}
int main() { solve(); return 0; }//boj3346