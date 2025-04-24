#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 25;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ll sq(const ll& x) { return x * x; }
inline ld sq(const ld& x) { return x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}

int N;
struct Pos {
	ld x, y;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos rot(const ld& the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
struct Circle {
	Pos c;
	int r;
	Circle(Pos c_ = Pos(), int r_ = 0) : c(c_), r(r_) {}
	bool operator == (const Circle& q) const { return c == q.c && r == q.r; }
	bool operator >= (const Pos& p) const { return sign(r - (c - p).mag()) >= 0; }
	Pos p(const ld& t) const { return c + Pos(r, 0).rot(t); }
	ld rad(const Pos& p) const { return (p - c).rad(); }
	ld area(const ld& lo, const ld& hi) const { return (hi - lo) * r * r * .5; }
	ld green(const ld& lo, const ld& hi) const {
		Pos s = Pos(cos(lo), sin(lo)), e = Pos(cos(hi), sin(hi));
		ld fan = area(lo, hi);
		Pos m = c + (s + e) * r * (ld).5;
		ld tz = (cos(lo) - cos(hi)) * m.y * r;
		return fan + tz - (s / e) * r * r * (ld).5;
	}
};
Vld intersections(const Circle& a, const Circle& b) {
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;
	ll ra = a.r, rb = b.r;
	ld distance = vec.mag();
	ld rd = vec.rad();
	if (vec.Euc() > sq(ra + rb) + TOL) return {};
	if (vec.Euc() < sq(ra - rb) - TOL) return {};
	ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance * ra);
	if (X < -1) X = -1;
	if (X > 1) X = 1;
	ld h = acos(X);
	Vld ret = {};
	ret.push_back(norm(rd - h));
	if (zero(h)) return ret;
	ret.push_back(norm(rd + h));
	return ret;
}
struct Station {
	int x, y;
	int M, L, U;
	int r[20], s[20], w[20];
	bool F[20];
	Station(int x0 = 0, int y0 = 0, int m0 = 0, int l0 = 0, int u0 = 0)
		: x(x0), y(y0), M(m0), L(l0), U(u0) {
		for (int i = 0; i < 20; i++) r[i] = 0, s[i] = 0, w[i] = 0, F[i] = 0;
	}
	Pos p() const { return Pos(x, y); }
	Circle c(const int& i) const { return Circle(p(), r[i]); }
} S[LEN];
struct Pow {
	int s, w;
	Pow(int s_ = 0, int w_ = 0) : s(s_), w(w_) {}
	bool operator < (const Pow& p) const { return s == p.s ? w > p.w : s > p.s; }
};
struct Prob {
	ld p;
	int i, s;
	bool operator < (const Prob& o) const { return s > o.s; }
};
ld expect(const int& i, const int& j, const Pos& mid) {//from ekzm0204 (oj.uz)
	ld POS[LEN], NEG[LEN];
	memset(POS, 0, sizeof POS);
	memset(NEG, 0, sizeof NEG);
	int sz;
	std::vector<Prob> P, M;
	Prob p, m;
	Circle cij = S[i].c(j);
	for (int k = 0; k < N; k++) {
		POS[k] = 1; NEG[k] = 1;
		int all = S[k].U - S[k].L + 1;
		int pl = S[k].L;
		int pu = S[k].U;
		int nl = S[k].L;
		int nu = S[k].U;
		std::vector<Pow> PP, MM;
		for (int l = 0; l < S[k].M; l++) {
			Circle ckl = S[k].c(l);
			if (ckl >= mid) PP.push_back(Pow(S[k].s[l], S[k].w[l]));
			if (ckl == cij) continue;
			if (ckl >= mid) MM.push_back(Pow(S[k].s[l], S[k].w[l]));
		}
		std::sort(PP.begin(), PP.end());
		sz = PP.size();
		for (int l = 0; l < sz; l++) {
			int s = PP[l].s;
			int w = PP[l].w;
			if (w < pl) w = pl;
			int diff = pu - w + 1;
			if (diff > 0) {
				pu = w - 1;
				p.p = (ld)diff / all;
				p.i = k;
				p.s = s;
				P.push_back(p);
			}
		}
		std::sort(MM.begin(), MM.end());
		sz = MM.size();
		for (int l = 0; l < sz; l++) {
			int s = MM[l].s;
			int w = MM[l].w;
			if (w < nl) w = nl;
			int diff = nu - w + 1;
			if (diff > 0) {
				nu = w - 1;
				m.p = (ld)diff / all;
				m.i = k;
				m.s = s;
				M.push_back(m);
			}
		}
	}
	ld total = 0, per;
	if (P.size()) {
		per = 1.;
		std::sort(P.begin(), P.end());
		sz = P.size();
		for (int k = 0; k < sz; k++) {
			p = P[k];
			total += p.s * per * p.p / POS[p.i];
			per = per / POS[p.i];
			POS[p.i] -= p.p;
			per = per * POS[p.i];
		}
	}
	if (M.size()) {
		per = 1.;
		std::sort(M.begin(), M.end());
		sz = M.size();
		for (int k = 0; k < sz; k++) {
			m = M[k];
			total -= m.s * per * m.p / NEG[m.i];
			per = per / NEG[m.i];
			NEG[m.i] -= m.p;
			per = per * NEG[m.i];
		}
	}
	return total;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	ld A = 0;
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> S[i].x >> S[i].y >> S[i].M >> S[i].L >> S[i].U;
		for (int j = 0; j < S[i].M; j++) std::cin >> S[i].r[j] >> S[i].s[j] >> S[i].w[j];
		for (int j = 0; j < S[i].M; j++) {
			for (int k = j + 1; k < S[i].M; k++) {
				if (S[i].c(j) == S[i].c(k)) S[i].F[k] = 1;
			}
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < S[i].M; j++) {
			if (S[i].F[j]) continue;
			Vld V = { 0 };
			Circle cij = S[i].c(j);
			for (int k = 0; k < N; k++) {
				if (k == i) continue;
				for (int l = 0; l < S[k].M; l++) {
					Circle ckl = S[k].c(l);
					if (i < k && cij == ckl) S[k].F[l] = 1;
					ll d1 = sq((ll)S[i].x - S[k].x) + sq((ll)S[i].y - S[k].y);
					if (!d1) continue;
					ll d2 = sq((ll)S[i].r[j] + S[k].r[l]);
					ll d3 = sq((ll)S[i].r[j] - S[k].r[l]);
					if (d1 > d2 || d1 < d3) continue;
					Vld inxs = intersections(cij, ckl);
					for (const ld& x : inxs) V.push_back(x);
				}
			}
			std::sort(V.begin(), V.end());
			V.erase(unique(V.begin(), V.end(), eq), V.end());
			V.push_back(PI * 2);
			int sz = V.size();
			for (int k = 0; k < sz - 1; k++) {
				const ld& s = V[k], e = V[k + 1];
				if (eq(s, e)) continue;
				ld a = cij.green(s, e);
				ld m = (s + e) * .5;
				Pos mid = cij.p(m);
				A += expect(i, j, mid) * a;
			}
		}
	}
	std::cout << A << "\n";
	return;
}
int main() { solve(); return 0; }//boj10910 Random signals hint from kcm1700

/*

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 25;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ll sq(const ll& x) { return x * x; }
inline ld sq(const ld& x) { return x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}

int N;
struct Pos {
	ld x, y;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos rot(const ld& the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
struct Circle {
	Pos c;
	int r;
	Circle(Pos c_ = Pos(), int r_ = 0) : c(c_), r(r_) {}
	bool operator == (const Circle& q) const { return c == q.c && r == q.r; }
	bool operator >= (const Pos& p) const { return sign(r - (c - p).mag()) >= 0; }
	Pos p(const ld& t) const { return c + Pos(r, 0).rot(t); }
	ld rad(const Pos& p) const { return (p - c).rad(); }
	ld area(const ld& lo, const ld& hi) const { return (hi - lo) * r * r * .5; }
	ld green(const ld& lo, const ld& hi) const {
		Pos s = Pos(cos(lo), sin(lo)), e = Pos(cos(hi), sin(hi));
		ld fan = area(lo, hi);
		Pos m = c + (s + e) * r * (ld).5;
		ld tz = (cos(lo) - cos(hi)) * m.y * r;
		return fan + tz - (s / e) * r * r * (ld).5;
	}
};
Vld intersections(const Circle& a, const Circle& b) {
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;
	ll ra = a.r, rb = b.r;
	ld distance = vec.mag();
	ld rd = vec.rad();
	if (vec.Euc() > sq(ra + rb) + TOL) return {};
	if (vec.Euc() < sq(ra - rb) - TOL) return {};
	ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance * ra);
	if (X < -1) X = -1;
	if (X > 1) X = 1;
	ld h = acos(X);
	Vld ret = {};
	ret.push_back(norm(rd - h));
	if (zero(h)) return ret;
	ret.push_back(norm(rd + h));
	return ret;
}
struct Station {
	int x, y;
	int M, L, U;
	int r[20], s[20], w[20];
	Vld A[20];
	bool F[20];
	Station(int x0 = 0, int y0 = 0, int m0 = 0, int l0 = 0, int u0 = 0)
		: x(x0), y(y0), M(m0), L(l0), U(u0) {
		for (int i = 0; i < 20; i++) r[i] = 0, s[i] = 0, w[i] = 0, F[i] = 0, A[i].clear();
	}
	Pos p() const { return Pos(x, y); }
	Circle c(const int& i) const { return Circle(p(), r[i]); }
} S[LEN];
struct Pow {
	int s, w;
	Pow(int s_ = 0, int w_ = 0) : s(s_), w(w_) {}
	bool operator < (const Pow& p) const { return s == p.s ? w > p.w : s > p.s; }
};
struct Prob {
	ld p;
	int i, s;
	bool operator < (const Prob& o) const { return s > o.s; }
};
ld expect(const int& i, const int& j, const Pos& mid) {//from ekzm0204 (oj.uz)
	ld POS[LEN], NEG[LEN];
	memset(POS, 0, sizeof POS);
	memset(NEG, 0, sizeof NEG);
	int sz;
	std::vector<Prob> P, M;
	Prob p, m;
	Circle cij = S[i].c(j);
	for (int k = 0; k < N; k++) {
		POS[k] = 1; NEG[k] = 1;
		int all = S[k].U - S[k].L + 1;
		int pl = S[k].L;
		int pu = S[k].U;
		int nl = S[k].L;
		int nu = S[k].U;
		std::vector<Pow> PP, MM;
		for (int l = 0; l < S[k].M; l++) {
			Circle ckl = S[k].c(l);
			if (ckl >= mid) PP.push_back(Pow(S[k].s[l], S[k].w[l]));
			if (ckl == cij) continue;
			if (ckl >= mid) MM.push_back(Pow(S[k].s[l], S[k].w[l]));
		}
		std::sort(PP.begin(), PP.end());
		sz = PP.size();
		for (int l = 0; l < sz; l++) {
			int s = PP[l].s;
			int w = PP[l].w;
			if (w < pl) w = pl;
			int diff = pu - w + 1;
			if (diff > 0) {
				pu = w - 1;
				p.p = (ld)diff / all;
				p.i = k;
				p.s = s;
				P.push_back(p);
			}
		}
		std::sort(MM.begin(), MM.end());
		sz = MM.size();
		for (int l = 0; l < sz; l++) {
			int s = MM[l].s;
			int w = MM[l].w;
			if (w < nl) w = nl;
			int diff = nu - w + 1;
			if (diff > 0) {
				nu = w - 1;
				m.p = (ld)diff / all;
				m.i = k;
				m.s = s;
				M.push_back(m);
			}
		}
	}
	ld total = 0, per;
	if (P.size()) {
		per = 1.;
		std::sort(P.begin(), P.end());
		sz = P.size();
		for (int k = 0; k < sz; k++) {
			p = P[k];
			total += p.s * per * p.p / POS[p.i];
			per = per / POS[p.i];
			POS[p.i] -= p.p;
			per = per * POS[p.i];
		}
	}
	if (M.size()) {
		per = 1.;
		std::sort(M.begin(), M.end());
		sz = M.size();
		for (int k = 0; k < sz; k++) {
			m = M[k];
			total -= m.s * per * m.p / NEG[m.i];
			per = per / NEG[m.i];
			NEG[m.i] -= m.p;
			per = per * NEG[m.i];
		}
	}
	return total;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	ld A = 0;
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> S[i].x >> S[i].y >> S[i].M >> S[i].L >> S[i].U;
		for (int j = 0; j < S[i].M; j++) std::cin >> S[i].r[j] >> S[i].s[j] >> S[i].w[j];
		for (int j = 0; j < S[i].M; j++) {
			for (int k = j + 1; k < S[i].M; k++) {
				if (S[i].c(j) == S[i].c(k)) S[i].F[k] = 1;
			}
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < S[i].M; j++) {
			if (S[i].F[j]) continue;
			Vld& V = S[i].A[j];
			V = { 0 };
			Circle cij = S[i].c(j);
			for (int k = 0; k < N; k++) {
				if (k == i) continue;
				for (int l = 0; l < S[k].M; l++) {
					Circle ckl = S[k].c(l);
					if (i < k && cij == ckl) { S[k].F[l] = 1; continue; }
					ll d1 = sq((ll)S[i].x - S[k].x) + sq((ll)S[i].y - S[k].y);
					if (!d1) continue;
					ll d2 = sq((ll)S[i].r[j] + S[k].r[l]);
					ll d3 = sq((ll)S[i].r[j] - S[k].r[l]);
					if (d1 > d2 || d1 < d3) continue;
					Vld inxs = intersections(cij, ckl);
					for (const ld& x : inxs) V.push_back(x);
				}
			}
			std::sort(V.begin(), V.end());
			V.erase(unique(V.begin(), V.end(), eq), V.end());
			V.push_back(PI * 2);
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < S[i].M; j++) {
			if (S[i].F[j]) continue;
			Circle cij = S[i].c(j);
			const Vld& V = S[i].A[j];
			int sz = V.size();
			for (int k = 0; k < sz - 1; k++) {
				const ld& s = V[k], e = V[k + 1];
				if (eq(s, e)) continue;
				ld a = cij.green(s, e);
				ld m = (s + e) * .5;
				Pos mid = cij.p(m);
				A += expect(i, j, mid) * a;
			}
		}
	}
	std::cout << A << "\n";
	return;
}
int main() { solve(); return 0; }//boj10910 hint from kcm1700


*/
