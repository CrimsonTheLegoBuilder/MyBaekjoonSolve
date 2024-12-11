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
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
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
	int f;
	bool operator < (const Frac& o) const {
		int s1 = sign(x) * sign(den);
		int s2 = sign(o.x) * sign(o.den);
		if (s1 != s2) return s1 < s2;
		if (!x) {
			assert(!o.x);
			if (f != o.f) return f < o.f;
			return 0;
		}
		int128 div1 = abs_(x) / abs_(den);
		int128 div2 = abs_(o.x) / abs_(o.den);
		int128 mod1 = abs_(x) % abs_(den);
		int128 mod2 = abs_(o.x) % abs_(o.den);
		if (div1 == div2) {
			int128 n1 = mod1 * o.den;
			int128 n2 = mod2 * den;
			if (n1 == n2) {
				if (f != o.f) return f < o.f;
				return 0;
			}
			return s1 > 0 ? n1 < n2 : n1 > n2;
		}
		return s1 > 0 ? div1 < div2 : div1 > div2;
	}
	bool operator == (const Frac& o) const {
		return x == o.x && den == o.den;
		int s1 = sign(x) * sign(den);
		int s2 = sign(o.x) * sign(o.den);
		if (s1 != s2) return 0;
		if (!x) { assert(!o.x); return 1; }
		int128 div1 = abs_(x) / abs_(den);
		int128 div2 = abs_(o.x) / abs_(o.den);
		int128 mod1 = abs_(x) % abs_(den);
		int128 mod2 = abs_(o.x) % abs_(o.den);
		if (div1 != div2) return 0;
		int128 n1 = mod1 * o.den;
		int128 n2 = mod2 * den;
		if (n1 != n2) return 0;
		return 1;
	}
};
std::vector<Frac> tmp, S;
std::map<Frac, int> Mfrac;
std::vector<std::pair<Frac, Frac>> V;
Frac intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ll a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	int128 x = (int128)p1.x * a2 + (int128)p2.x * a1;
	int128 det = (int128)a1 + a2;
	int128 d = gcd(abs_(x), abs_(det));
	assert(det && d);
	if (x == 0) det = 1;
	else if (sign(x) * sign(det) < 0 && 0 < x) x *= -1, det *= -1;
	else if (sign(x) * sign(det) > 0) x = abs_(x), det = abs_(det);
	//std::cout << "den:: " << det << "\n";
	return { x, det, 0 };
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
	for (int _ = 0; _ < N; _++) {
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
		//norm(P);
		tmp.clear();
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
				if (ccw(O, v, p1, p2) < 0) inx.f = 0;
				else inx.f = 1;
				//std::cout << "p1:: " << p1 << " p2:: " << p2 << "\n";
				//std::cout << "inx:: " << inx.x << " " << inx.f << "\n";
				tmp.push_back(inx);
			}
			else if (!det1) {
				inx = { p1.x, 1, 0 };
				if (inx.x <= 0) continue;
				if (det0 == 0 && det2 == 0) continue;
				if (det0 * det2 > 0) {
					if (dir > 0) {
						inx.f = 0; tmp.push_back(inx);
						inx.f = 1; tmp.push_back(inx);
					}
					continue;
				}
				if (det0 > 0 || det2 < 0) inx.f = 0;
				if (det0 < 0 || det2 > 0) inx.f = 1;
				if (!det0 && dir < 0) continue;
				if (!det2 && dir < 0) continue;
				tmp.push_back(inx);
			}
		}
		std::sort(tmp.begin(), tmp.end());
		int sz = tmp.size();
		assert(sz % 2 == 0);
		//for (int i = 0; i < sz; i += 2) {
		//	Frac f1 = tmp[i];
		//	Frac f2 = tmp[i + 1];
		//	std::cout << "f1:: " << f1.x << " " << f1.den << " " << f1.x * 1. / f1.den << " " << f1.f << "\n";
		//	std::cout << "f2:: " << f2.x << " " << f2.den << " " << f2.x * 1. / f2.den << " " << f2.f << "\n";
		//	//std::cout << "f1:: " << " " << f1.x * 1. / f1.den << " " << f1.f << "\n";
		//	//std::cout << "f2:: " << " " << f2.x * 1. / f2.den << " " << f2.f << "\n";
		//}
		for (int i = 0; i < sz; i += 2) {
			Frac f1 = tmp[i];
			Frac f2 = tmp[i + 1];
			assert(f1.f == 0 && f2.f == 1);
			if (f2.x < 0) continue;
			f1.f = 0;
			f2.f = 0;
			V.push_back({ f1, f2 });
			S.push_back(f1);
			S.push_back(f2);
		}
	}
	std::sort(S.begin(), S.end());
	S.erase(unique(S.begin(), S.end()), S.end());
	int sz = S.size();
	for (int i = 0; i < sz; i++) Mfrac[S[i]] = i;
	Polygon R;
	sz = V.size();
	for (int i = 0; i < sz; i++) {
		int s = Mfrac[V[i].first];
		int e = Mfrac[V[i].second];
		R.push_back(Pos(s, e));
	}
	std::sort(R.begin(), R.end());
	int hi = -1, cnt = 0;
	sz = R.size();
	for (int i = 0; i < sz; i++) {
		if (hi < R[i].x) cnt++, hi = R[i].y;
		else hi = std::max(hi, R[i].y);
	}
	std::cout << cnt << "\n";
	return;
}
int main() { solve(); return 0; }//boj7047