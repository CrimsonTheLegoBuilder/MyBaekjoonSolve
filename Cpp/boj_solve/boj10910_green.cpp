#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <map>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-8;
const ld PI = acos(-1);
const int LEN = 25;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool cmpld(const ld& p, const ld& q) { return sign(p - q) < 0; }
inline bool eqld(const ld& p, const ld& q) { return zero(p - q); }
inline ll sq(int x) { return (ll)x * x; }
inline ll sq(ll x) { return x * x; }
inline ld sq(ld x) { return x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}

//#define DEBUG

#define HI 0
#define LO 1

int N, M, T, Q;
bool IN[25][25];
ld ANS;
Vld R[400];
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	//bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos rot(ld the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
Polygon KEY;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
Polygon intersection(const Pos& ca, const ld& ra, const Pos& cb, const ld& rb) {
	Pos vec = cb - ca;
	ld distance = vec.mag();
	ld rd = vec.rad();
	ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance * ra);
	if (X < -1) X = -1;
	if (X > 1) X = 1;
	ld h = acos(X);
	Polygon ret;
	ret.push_back(ca + Pos(ra, 0).rot(norm(rd + h)));
	if (zero(h)) return ret;
	ret.push_back(ca + Pos(ra, 0).rot(norm(rd - h)));
	return ret;
}
struct Station {
	int x, y;
	int M, L, U;
	Vint r, s, w;
	Station(int x0 = 0, int y0 = 0, int m0 = 0, int l0 = 0, int u0 = 0) : x(x0), y(y0), M(m0), L(l0), U(u0) {}
	Pos p() const { return Pos(x, y); }
} S[25];
struct Disk {
	int x, y, r;
	Disk(int x_ = 0, int y_ = 0, int r_ = 0) : x(x_), y(y_), r(r_) {}
	bool operator < (const Disk& o) const { return x == o.x ? y == o.y ? r < o.r : y < o.y : x < o.x; }
	bool operator == (const Disk& o) const { return x == o.x && y == o.y && r == o.r; }
	Pos c() const { return Pos(x, y); }
	friend int intersect(const Disk& p, const Disk& q) {
		if (p.x == q.x && p.y == q.y) return 0;
		ll d = sq(p.x - q.x) + sq(p.y - q.y);
		ll ro = sq(p.r + q.r);
		ll ri = sq(p.r - q.r);
		if (d > sq(ro) || d < sq(ri)) return 0;
		if (d == sq(ro) || d == sq(ri)) return 1;
		return 2;
	}
};
bool intersection(const Disk& a, const Disk& b, Polygon& inx) {
	inx.clear();
	int cnt = intersect(a, b);
	if (!cnt) return 0;
	inx = intersection(a.c(), a.r, b.c(), b.r);
	return cnt;
}
std::vector<Disk> D;
struct Arc {
	ld hi, lo;
	Pos c;
	int r;
	bool d;
} A[200'000]; int AP;
struct Tangent {
	Pos dir, pet;
	bool d;
};
std::map<Pos, std::vector<Tangent>> MAP;
ld get_y(const Arc& a, const ld& x) {
	Pos p = a.c;
	ld r = a.r;
	if ((p.x - r) < x && x < (p.x + r)) {
		ld dy = sqrt(r * r - sq(p.x - x));
		return p.y + dy * (a.d == HI ? 1 : -1);
	}
	return p.y;
}
ld green(const Arc& a, const ld& sx, const ld& ex) {
	ld r = a.r;
	ld sy = get_y(a, sx);
	ld ey = get_y(a, ex);
	int f = a.d == HI ? 1 : -1;
	Pos c = a.c;
	Pos s = Pos(sx, sy);
	Pos e = Pos(ex, ey);
	ld t = norm(std::abs(rad(e - c, s - c)));
	ld fan = r * r * t * .5 - std::abs(cross(c, e, s)) * .5;
	ld rec = (ex - sx) * (sy + ey) * .5;
	return rec + fan * f;
}
struct Prob {
	ld p;
	int i, s;
	bool operator < (const Prob& o) const { return s > o.s; }
};
ld prob[LEN];
struct Pow {
	int s, w;
	bool operator < (const Pow& p) const { return s == p.s ? w > p.w : s > p.s; }
};
int P[LEN * LEN + 10];//disjoint set
int find(int i) { return P[i] < 0 ? i : P[i] = find(P[i]); }
bool join(int i, int j) {
	i = find(i), j = find(j);
	if (i == j) return 0;
	if (P[i] < P[j]) P[i] += P[j], P[j] = i;
	else P[j] += P[i], P[i] = j;
	return 1;
}
int V[LEN * LEN * 10];
Vint GS[LEN * LEN * 10];
void sweep(const int& k, const ld& x) {
	int sz;
	ld a = 0;
	Pos m = Pos(0, 0);
	for (int i = 0; i < N; i++) {
		Pos v = S[i].p() - m;
		for (int j = 0; j < S[i].M; j++) {
			IN[i][j] = sign(sq(S[i].r[j]) - v.Euc()) >= 0;
		}
	}
	std::vector<Prob> P;
	Prob p;
	for (int i = 0; i < N; i++) {
		prob[i] = 1;
		int all = S[i].U - S[i].L + 1;
		int L = S[i].L, U = S[i].U;
		std::vector<Pow> V;
		for (int j = 0; j < S[i].M; j++) if (IN[i][j]) V.push_back({ S[i].s[j], S[i].w[j] });
		std::sort(V.begin(), V.end());
		sz = V.size();
		for (int j = 0; j < sz; j++) {
			int s = V[j].s, w = V[j].w;
			if (w < L) w = L;
			int diff = U - w + 1;
			if (diff > 0) {
				U = w - 1;
				p.p = (ld)diff / all;
				p.i = i;
				p.s = s;
				P.push_back(p);
			}
		}
	}
	if (P.empty()) return;
	std::sort(P.begin(), P.end());
	ld per = 1.;
	ld total = 0;
	sz = P.size();
	for (int i = 0; i < sz; i++) {
		p = P[i];
		total += p.s * per * p.p / prob[p.i];
		per = per / prob[p.i];
		prob[p.i] -= p.p;
		per = per * prob[p.i];
	}
	ANS += total * a;
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N;
	ANS = 0;
	for (int i = 0; i < N; i++) {
		std::cin >> S[i].x >> S[i].y >> S[i].M >> S[i].L >> S[i].U;
		M = S[i].M;
		S[i].r.resize(M);
		S[i].s.resize(M);
		S[i].w.resize(M);
		for (int j = 0; j < M; j++) {
			std::cin >> S[i].r[j] >> S[i].s[j] >> S[i].w[j];
			D.push_back(Disk(S[i].x, S[i].y, S[i].r[j]));
		}
	}
	std::sort(D.begin(), D.end());
	D.erase(unique(D.begin(), D.end()), D.end());
	Polygon inx;
	int sz = D.size();
	for (int i = 0; i < sz; i++) 
		for (int j = i + 1; j < sz; j++) 
			if (intersection(D[i], D[j], inx)) 
				for (const Pos& p : inx) {
					KEY.push_back(p);
					R[i].push_back(norm((D[i].c() - p).rad()));
					R[j].push_back(norm((D[j].c() - p).rad()));
				}
	std::sort(KEY.begin(), KEY.end());
	KEY.erase(unique(KEY.begin(), KEY.end()), KEY.end());

	for (int i = 0; i < sz; i++) {
		R[i].push_back(0);
		R[i].push_back(2 * PI);
		std::sort(R[i].begin(), R[i].end());
		R[i].erase(unique(R[i].begin(), R[i].end()), R[i].end());
		for (const ld& t : R[i]) {
			Pos key = D[i].c() + Pos(0, D[i].r).rot(t);

		}
	}
	std::cout << ANS << "\n";
	return;
}
int main() { solve(); return 0; }//boj10910 Random signals