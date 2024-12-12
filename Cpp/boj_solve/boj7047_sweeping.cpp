#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include <vector>
#include <set>
#include <map>
typedef long long ll;
typedef long long int128;
//typedef __int128 int128;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 1e3 + 5;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }

int N, M;
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
} u, v; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
Polygon H[LEN];
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll tq = cross(d1, d2, d3); return tq < 0 ? -1 : !!tq; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll tq = cross(d1, d2, d3, d4); return tq < 0 ? -1 : !!tq; }
bool on_seg_strong(const Pos& p1, const Pos& p2, const Pos& q) { return !ccw(p1, p2, q) && dot(p1, q, p2) >= 0; }
ll area(const Polygon& H) {
	ll a = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return a;
}
void norm(Polygon& H) {
	ll A = area(H); assert(A);
	if (A < 0) std::reverse(H.begin(), H.end());
	auto s = std::min_element(H.begin(), H.end());
	std::rotate(H.begin(), s, H.end());
	return;
}
int128 abs_(int128 x) { if (x < 0) x *= -1; return x; }
int128 gcd(int128 a, int128 b) { while (b) { int128 tmp = a % b; a = b; b = tmp; } return a; }
int sign(const int128& x) { return x < 0 ? -1 : x > 0 ? 1 : 0; }
struct Frac {
	int128 x, den;
	bool operator < (const Frac& o) const {
		int s1 = sign(x) * sign(den);
		int s2 = sign(o.x) * sign(o.den);
		if (s1 != s2) return s1 < s2;
		if (!x) { assert(!o.x); return 0; }
		int128 div1 = abs_(x) / abs_(den);
		int128 div2 = abs_(o.x) / abs_(o.den);
		int128 mod1 = abs_(x) % abs_(den);
		int128 mod2 = abs_(o.x) % abs_(o.den);
		if (div1 == div2) {
			int128 n1 = mod1 * o.den;
			int128 n2 = mod2 * den;
			if (n1 == n2) return 0;
			return s1 > 0 ? n1 < n2 : n1 > n2;
		}
		return s1 > 0 ? div1 < div2 : div1 > div2;
	}
	bool operator == (const Frac& o) const {
		return x == o.x && den == o.den;
	}
};
std::vector<Frac> S;
Frac intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ll a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	int128 x = (int128)p1.x * a2 + (int128)p2.x * a1;
	int128 det = (int128)a1 + a2;
	int128 d = gcd(abs_(x), abs_(det));
	assert(det && d);
	if (x == 0) det = 1;
	else if (sign(x) * sign(det) < 0 && 0 < x) x *= -1, det *= -1;
	else if (sign(x) * sign(det) > 0) x = abs_(x), det = abs_(det);
	return { x, det };
}
int bi_search(const std::vector<Frac>& S, const Frac& f) {
	int sz = S.size();
	int s = 0, e = sz - 1, m;
	while (s <= e) {
		m = s + e >> 1;
		if (S[m] == f) return m;
		else if (S[m] < f) s = m + 1;
		else e = m - 1;
	}
	return s;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N >> u;
	u *= -1;
	v = u;
	if (v.x < 0) v *= -1;
	else if (!v.x) {
		if (v.y > 0) v = -~v;
		else v = ~v;
	}
	Frac inx;
	for (int n = 0; n < N; n++) {
		std::cin >> M;
		Polygon P(M);
		for (Pos& p : P) std::cin >> p;
		if (u.x < 0) {
			for (Pos& p : P) p *= -1;
		}
		else if (!u.x) {
			if (u.y > 0) for (Pos& p : P) p = -~p;
			else for (Pos& p : P) p = ~p;
		}
		H[n] = P;
		for (int i = 0; i < M; i++) {
			const Pos& p0 = P[(i + 1 + M) % M], & p1 = P[i], & p2 = P[(i - 1 + M) % M];
			assert(!on_seg_strong(p0, p1, O));
			int det0 = ccw(O, v, p0);
			int det1 = ccw(O, v, p1);
			int det2 = ccw(O, v, p2);
			int dir = ccw(p0, p1, p2);
			//assert(dir);
			if (det1 * det2 < 0) {
				inx = intersection(O, v, p1, p2);
				if (sign(inx.x) * sign(inx.den) <= 0) continue;
				S.push_back(inx);
			}
			else if (!det1) {
				inx = { p1.x, 1 };
				if (inx.x <= 0) continue;
				if (det0 == 0 && det2 == 0) continue;
				if (det0 * det2 > 0) {
					if (dir > 0) {
						S.push_back(inx);
					}
					continue;
				}
				if (!det0 && dir < 0) continue;
				if (!det2 && dir < 0) continue;
				S.push_back(inx);
			}
		}
	}
	std::sort(S.begin(), S.end());
	S.erase(unique(S.begin(), S.end()), S.end());
	int sz = S.size();
	Polygon R, V;
	for (int n = 0; n < N; n++) {
		R.clear();
		const Polygon& P = H[n];
		M = P.size();
		for (int i = 0; i < M; i++) {
			const Pos& p0 = P[(i + 1 + M) % M], & p1 = P[i], & p2 = P[(i - 1 + M) % M];
			assert(!on_seg_strong(p0, p1, O));
			int det0 = ccw(O, v, p0);
			int det1 = ccw(O, v, p1);
			int det2 = ccw(O, v, p2);
			int dir = ccw(p0, p1, p2);
			//assert(dir);
			if (det1 * det2 < 0) {
				inx = intersection(O, v, p1, p2);
				if (sign(inx.x) * sign(inx.den) <= 0) continue;
				Pos p = Pos(bi_search(S, inx), 0);
				if (ccw(O, v, p1, p2) < 0) p.y = 0;
				else p.y = 1;
				R.push_back(p);
			}
			else if (!det1) {
				inx = { p1.x, 1 };
				if (inx.x <= 0) continue;
				if (det0 == 0 && det2 == 0) continue;
				if (det0 * det2 > 0) {
					if (dir > 0) {
						Pos p = Pos(bi_search(S, inx), 0);
						p.y = 0;
						R.push_back(p);
						p.y = 1;
						R.push_back(p);
					}
					continue;
				}
				Pos p = Pos(bi_search(S, inx), 0);
				if (det0 > 0 || det2 < 0) p.y = 0;
				if (det0 < 0 || det2 > 0) p.y = 1;
				if (!det0 && dir < 0) continue;
				if (!det2 && dir < 0) continue;
				R.push_back(p);
			}
		}
		sz = R.size();
		std::sort(R.begin(), R.end());
		for (int i = 0; i < sz; i += 2) {
			Pos f1 = R[i];
			Pos f2 = R[i + 1];
			assert(f1.y == 0 && f2.y == 1);
			if (f2.x < 0) continue;
			V.push_back(Pos(f1.x, f2.x));
		}
	}
	std::sort(V.begin(), V.end());
	int hi = -1, cnt = 0;
	sz = V.size();
	for (int i = 0; i < sz; i++) {
		if (hi < V[i].x) cnt++, hi = V[i].y;
		else hi = std::max(hi, V[i].y);
	}
	std::cout << cnt << "\n";
	return;
}
int main() { solve(); return 0; }//boj7047