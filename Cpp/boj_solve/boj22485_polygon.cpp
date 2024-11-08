#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
#include <map>
#include <set>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<bool> Vbool;
const ll INF = 1e17;
const int LEN = 105;
const ld TOL = 1e-25;
inline int sign(const ll& x) { return x < 0 ? -1 : x > 0; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
ll gcd(ll a, ll b) { while (b) { ll tmp = a % b; a = b; b = tmp; } return a; }

#define __FUCK__ ;
#define WHAT_THE_FUCK
//#define DEBUG

int N, M;
struct BigPos {//what the fuck??? holy shit fucking floating point accuracy error!!
	ll x, y, den;
	BigPos(ll X = 0, ll Y = 0, ll D = 1) : x(X), y(Y), den(D) {}
	bool operator == (const BigPos& p) const { return x == p.x && y == p.y; }
	bool operator != (const BigPos& p) const { return x != p.x || y != p.y; }
	bool operator < (const BigPos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const BigPos& p) const { return *this < p || *this == p; }
	BigPos operator + (const BigPos& p) const { return { x + p.x, y + p.y }; }
	BigPos operator - (const BigPos& p) const { return { x - p.x, y - p.y }; }
	BigPos operator * (const ll& n) const { return { x * n, y * n }; }
	BigPos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const BigPos& p) const { return x * p.x + y * p.y; }
	ll operator / (const BigPos& p) const { return x * p.y - y * p.x; }
	ll Euc() const { return x * x + y * y; }
	friend std::istream& operator >> (std::istream& is, BigPos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const BigPos& p) { os << "(" << p.x << ", " << p.y << ")"; return os; }
} C[LEN]; const BigPos BO = { 0, 0, 1 }; const BigPos BINVAL = { 0, 0, 0 };
ll cross(const BigPos& d1, const BigPos& d2, const BigPos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const BigPos& d1, const BigPos& d2, const BigPos& d3, const BigPos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const BigPos& d1, const BigPos& d2, const BigPos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const BigPos& d1, const BigPos& d2, const BigPos& d3) { ll ret = cross(d1, d2, d3); return sign(ret); }
int ccw(const BigPos& d1, const BigPos& d2, const BigPos& d3, const BigPos& d4) { ll ret = cross(d1, d2, d3, d4); return sign(ret); }
bool on_seg_strong(const BigPos& d1, const BigPos& d2, const BigPos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool collinear(const BigPos& d1, const BigPos& d2, const BigPos& d3, const BigPos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
bool intersect(const BigPos& s1, const BigPos& s2, const BigPos& d1, const BigPos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
BigPos intersection(const BigPos& p1, const BigPos& p2, const BigPos& q1, const BigPos& q2) {
	if (p1 == q1 || p1 == q2) return p1;
	if (p2 == q1 || p2 == q2) return p2;
	ll a1 = cross(q1, q2, p1);
	ll a2 = -cross(q1, q2, p2);
	ll x = p1.x * a2 + p2.x * a1;
	ll y = p1.y * a2 + p2.y * a1;
	ll den = a1 + a2;
	ll gcd_ = gcd(std::abs(x), std::abs(den));
	gcd_ = gcd(std::abs(y), gcd_);
	x /= gcd_; y /= gcd_; den /= gcd_;
	return BigPos(x, y, den);
}
struct BigSeg {
	BigPos a, b;
	bool operator < (const BigSeg& s) const {
		ll tq;
		if (tq = ccw(a, b, s.a, s.b)) return tq > 0;
		if (collinear(a, b, s.a, s.b)) {
			if (a == s.a) return b < s.b;
			return a < s.a;
		}
		return ccw(a, b, s.a) > 0;
	}
	friend std::istream& operator >> (std::istream& is, BigSeg& s) { is >> s.a >> s.b; return is; }
} B[LEN];
BigPos intersection(const BigSeg& s1, const BigSeg& s2) {
	if (!intersect(s1.a, s1.b, s2.a, s2.b)) return BINVAL;
	return intersection(s1.a, s1.b, s2.a, s2.b);
}
bool collinear(const BigSeg& s1, const BigSeg& s2) { return collinear(s1.a, s1.b, s2.a, s2.b); }
struct Pos {
	ld x, y;
	int i;
	bool rv;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) { i = -1, rv = 0; }
	//bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	//bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	//bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2l(y, x); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << "(" << p.x << ", " << p.y << ")"; return os; }
} p0, p1, key, vec; const Pos O = Pos(0, 0); const Pos INVAL = Pos(INF, INF);
typedef std::vector<Pos> Polygon;
Pos conv(const BigPos& b) {
	ld x = 1. * b.x / b.den;
	ld y = 1. * b.y / b.den;
	return Pos(x, y);
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
bool cmpr(const Pos& p, const Pos& q) {
	ld tp = p.rad();
	ld tq = q.rad();
	return tp == tq ? p.rv > q.rv : tp < tq;
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) > 0; }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	//return f1 && f2;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
int inner_check(const Polygon& H, const Pos& p) {//concave
	int sz = H.size(), cnt = 0;
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (cur.y == nxt.y) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y <= p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}
ld area(const Polygon& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
struct Seg {
	Pos a, b;
	int i;
	Seg(Pos A = Pos(), Pos B = Pos()) : a(A), b(B) { i = -1; }
	Pos inx(const Seg& o) const { return intersection(a, b, o.a, o.b); }
} seg[LEN], frag[LEN * LEN * 10];
Polygon INX[LEN];
void inx_sort(Polygon& INX, const Pos& a) {
	std::sort(INX.begin(), INX.end(), [&](const Pos& p, const Pos& q) -> bool {
		return (a - p).Euc() < (a - q).Euc();
		});
	INX.erase(unique(INX.begin(), INX.end()), INX.end());
}
int I, I0;
std::map<Pos, Polygon> map_pos;
ld A[LEN * LEN + 10];
Polygon cell[LEN * LEN + 10]; int ci;
int V[LEN * LEN * 10];
Vint GS[LEN * LEN * 10];
void dfs(const int& i, int v) {
	V[v] = 1;
	cell[i].push_back(frag[v].a);
	for (const int& w : GS[v]) {
		if (V[w]) continue;
		dfs(i, w);
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> C[i];
	for (int i = 0; i < N - 1; i++) {
		BigPos a = C[i], b = C[i + 1];
		if (b < a) std::swap(a, b);
		B[i] = { a, b };
	}
	N--;
	Vbool F(N, 1);
	std::sort(B, B + N);
	for (int i = 0; i < N - 1; i++) {
		if (collinear(B[i], B[i + 1])) {
			if (B[i].b < B[i + 1].a) continue;
			if (B[i].a == B[i + 1].a) F[i] = 0;
			else if (B[i + 1].a <= B[i].b) {
				F[i] = 0;
				B[i + 1].a = std::min(B[i].a, B[i + 1].a);
				B[i + 1].b = std::max(B[i].b, B[i + 1].b);
			}
		}
	}
	std::vector<BigSeg> tmp;
	for (int i = 0; i < N; i++) if (F[i]) tmp.push_back(B[i]);
	N = tmp.size();
	for (int i = 0; i < N; i++) B[i] = tmp[i];
	for (int i = 0; i < N; i++) {
		B[i].a.den = B[i].b.den = 1;
		seg[i].a = conv(B[i].a);
		seg[i].b = conv(B[i].b);
		seg[i].i = i;
		seg[i].a.i = i;
		seg[i].b.i = i;
	}
	Polygon INXS;
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			BigPos inx = intersection(B[i], B[j]);
			if (inx.den) {
#ifdef DEBUG
				std::cout << "FUUUUCK!!!\n";
				std::cout << "FUCK:: S[i].a:: " << B[i].a << " S[i].b:: " << B[i].b << "\n";
				std::cout << "FUCK:: S[j].a:: " << B[j].a << " S[j].b:: " << B[j].b << "\n";
				std::cout << "FUCK:: inx:: " << conv(inx) << "\n";
				std::cout << "FUUUUCK!!!\n";
#endif
				Pos X = conv(inx);
				INX[i].push_back(X);
				INX[j].push_back(X);
				INXS.push_back(X);
			}
		}
	}
	std::sort(INXS.begin(), INXS.end());
	INXS.erase(unique(INXS.begin(), INXS.end()), INXS.end());
	I = 0;
	for (int i = 0; i < N; i++) {
		inx_sort(INX[i], seg[i].a);
		Polygon& v = INX[i];
		int sz = v.size();
		for (int j = 0; j < sz - 1; j++) {
			frag[I] = Seg(v[j], v[j + 1]);
			frag[I].i = I;
			I++;
		}
	}
	I0 = I;
	for (int i = 0; i < N; i++) {
		Polygon& v = INX[i];
		int sz = v.size();
		for (int j = 0; j < sz - 1; j++) {
			frag[I] = Seg(v[j + 1], v[j]);
			frag[I].i = I;
			I++;
		}
	}
	for (int i = 0; i < I; i++) {
		key = frag[i].a;
		vec = frag[i].b - frag[i].a;
		vec.i = frag[i].i;
		map_pos[key].push_back(vec);

		key = frag[i].b;
		vec = frag[i].a - frag[i].b;
		vec.i = frag[i].i;
		vec.rv = 1;
		map_pos[key].push_back(vec);
	}
	for (const Pos& key : INXS) {
		Polygon& v = map_pos[key];
		std::sort(v.begin(), v.end(), cmpr);
		int sz = v.size();
#ifdef DEBUG
		std::cout << "DEBUG:: key:: " << key << "\n";
		std::cout << "DEBUG:: sz:: " << sz << "\n";
		for (int k = 0; k < sz; k++) {
			std::cout << "v[" << k << "]:: " << v[k] << " i:: " << v[k].i << " rv:: " << v[k].rv << "\n";
		}
		std::cout << "FUCK::\n";
#endif
		assert(!(sz & 1));
		for (int j = 0; j < sz; j += 2) {
			Pos cur = v[(j - 1 + sz) % sz], nxt = v[j];
			//assert(cur.rv != nxt.rv);
			GS[nxt.i].push_back(cur.i);
		}
	}
	memset(V, 0, sizeof V);
	ci = 0;
	for (int i = 0; i < I; i++) {
		if (!V[i]) {
			dfs(ci, i);
			A[ci] = area(cell[ci]);
#ifdef DEBUG
			std::cout << "FUCK:: i:: " << i << " sz:: " << cell[ci].size() << "\n";
			//for (const Pos& p : cell[ci]) std::cout << p << "\n";
			std::cout << "FUCK:: A[" << ci << "]:: " << A[ci] << "\n";
#endif
			if (0 == A[ci]) {
				cell[ci].clear();
				A[ci] = 0;
				ci--;
			}
			ci++;
		}
	}
	ld ret = 0;
	for (int i = 0; i < ci; i++) if (A[i] > 0) ret += A[i];
	std::cout << ret * .5 << "\n";
	return;
}
int main() { solve(); return 0; }//boj22485 Area Folding