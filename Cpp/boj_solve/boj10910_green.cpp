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
const ld TOL = 1e-8;
const ld PI = acos(-1);
const int LEN = 25;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(ll x) { return x * x; }
inline ld sq(ld x) { return x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}
inline bool cmpld(const ld& p, const ld& q) { return sign(p - q) < 0; }
inline bool eqld(const ld& p, const ld& q) { return zero(p - q); }

//#define DEBUG

#define START 1
#define CROSS 2
#define END 3

#define HI 0
#define LO 1

int N, M, T, Q;
int I[LEN][LEN][2];
bool IN[LEN][LEN];
ld ANS;
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	//bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	//bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
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
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
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
	Station(int x0 = 0, int y0 = 0, int m0 = 0, int l0 = 0, int u0 = 0)
		: x(x0), y(y0), M(m0), L(l0), U(u0) {
		r.clear(); s.clear(); w.clear();
	}
	Pos p() const { return Pos(x, y); }
} S[LEN];
bool intersection(const int& ai, const int& aj, const int& bi, const int& bj, Polygon& inx) {
	if (S[ai].p() == S[bi].p()) return 0;
	ll x = S[ai].x - S[bi].x;
	ll y = S[ai].y - S[bi].y;
	ll d = x * x + y * y;
	ll ro = S[ai].r[aj] + S[bi].r[bj];
	ll ri = S[ai].r[aj] - S[bi].r[bj];
	if (d > sq(ro) || d < sq(ri)) return 0;
	inx = intersection(S[ai].p(), S[ai].r[aj], S[bi].p(), S[bi].r[bj]);
	return 1;
}
struct Arc {
	int i, j, d;
	ld x, y;
	bool operator < (const Arc& a) const { return y < a.y; }
} A[LEN * LEN * LEN * LEN];
ld get_y(const Arc& s, const ld& x) {
	Pos p = S[s.i].p();
	ld r = S[s.i].r[s.j];
	if ((p.x - r) < x && x < (p.x + r)) {
		ld dy = sqrt(r * r - sq(p.x - x));
		return p.y + dy * (s.d == HI ? 1 : -1);
	}
	return p.y;
}
ld green(const Arc& a, const ld& sx, const ld& ex) {
	ld r = S[a.i].r[a.j];
	ld sy = get_y(a, sx);
	ld ey = get_y(a, ex);
	int f = a.d == HI ? 1 : -1;

	Pos c = S[a.i].p();
	Pos s = Pos(sx, sy);
	Pos e = Pos(ex, ey);

	ld t = norm(std::abs(rad(e - c, s - c)));
	ld fan = r * r * t * .5 - std::abs(cross(c, e, s)) * .5;
	ld rec = (ex - sx) * (sy + ey) * .5;
	return rec + fan * f;
}
struct Event {
	int t, i, j, d;
	ld x;
	bool operator < (const Event& e) const { return zero(x - e.x) ? t < e.t : x < e.x; }
};
std::vector<Event> VE;
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
void sweep(const int& k, const ld& x) {
	int sz;

	Arc hi = A[k + 1];
	Arc lo = A[k];

	ld ha = green(hi, A[k].x, x);
	ld la = green(lo, A[k].x, x);

	ld a = ha - la;
	if (zero(a)) return;

	ld mx = (x + A[k].x) * .5;
	ld my = (get_y(hi, mx) + get_y(lo, mx)) * .5;
	Pos m = Pos(mx, my);

	for (int i = 0; i < N; i++) {
		Pos v = S[i].p() - m;
		for (int j = 0; j < S[i].M; j++) {
			ll r = S[i].r[j];
			IN[i][j] = sign(r * r - v.Euc()) >= 0;
		}
	}

	std::vector<Prob> P;
	Prob p;
	for (int i = 0; i < N; i++) {
		prob[i] = 1.;
		int all = S[i].U - S[i].L + 1;
		int L = S[i].L;
		int U = S[i].U;
		std::vector<Pow> V;
		for (int j = 0; j < S[i].M; j++) if (IN[i][j]) V.push_back({ S[i].s[j], S[i].w[j] });
		std::sort(V.begin(), V.end());
		sz = V.size();
		for (int j = 0; j < sz; j++) {
			int s = V[j].s;
			int w = V[j].w;
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
Vld X;
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N;
	ANS = 0;
	Event E, EE;
	for (int i = 0; i < N; i++) {
		E.i = i;
		std::cin >> S[i].x >> S[i].y >> S[i].M >> S[i].L >> S[i].U;
		M = S[i].M;
		S[i].r.resize(M);
		S[i].s.resize(M);
		S[i].w.resize(M);
		for (int j = 0; j < M; j++) {
			E.j = j;
			std::cin >> S[i].r[j] >> S[i].s[j] >> S[i].w[j];

			E.t = START;
			E.x = S[i].x - S[i].r[j];
			X.push_back(E.x);
			VE.push_back(E);

			E.t = END;
			E.x = S[i].x + S[i].r[j];
			X.push_back(E.x);
			VE.push_back(E);

			E.t = EE.t = CROSS;
			for (int k = 0; k < i; k++) {
				if (S[i].p() == S[k].p()) continue;//no || infinity intersections
				EE.i = k;
				for (int m = 0; m < S[k].M; m++) {
					EE.j = m;
					Polygon inx;
					if (intersection(i, j, k, m, inx)) {
						for (const Pos& p : inx) {
							E.x = EE.x = p.x;
							X.push_back(E.x);

							//if (p.y != S[i].y) {
							if (sign(p.y - S[i].y)) {
								//E.d = S[i].y <= p.y ? HI : LO;
								E.d = sign(p.y - S[i].y) >= 0 ? HI : LO;
								VE.push_back(E);
							}
							//if (p.y != S[k].y) {
							if (sign(p.y - S[k].y)) {
								//E.d = S[k].y <= p.y ? HI : LO;
								EE.d = sign(p.y - S[k].y) >= 0 ? HI : LO;
								VE.push_back(EE);
							}
						}
					}
				}
			}
		}
	}
	std::sort(VE.begin(), VE.end());
	std::sort(X.begin(), X.end());
	//X.erase(unique(X.begin(), X.end()), X.end());
	//std::sort(X.begin(), X.end(), cmpld);
	X.erase(unique(X.begin(), X.end(), eqld), X.end());
	return;
}
void solve() {
	init();
	int xsz = X.size();
	int esz = VE.size();
	int i = 0;
	Arc a;
	assert(xsz <= 400 * 400);
	for (int Q = 0; Q < xsz - 1; Q++) {//O(400 * 400)
		bool o = 0;
		for (; i < esz; i++) {
			const Event& E = VE[i];
			//if (X[Q] != E.x) break;
			if (!zero(X[Q] - E.x)) break;
			if (E.t == START) {
				a.i = E.i;
				a.j = E.j;
				a.d = E.d;
				a.x = E.x;
				ld mx = (E.x + X[Q + 1]) * .5;
				a.d = LO;
				a.y = get_y(a, mx);
				A[T++] = a;
				a.d = HI;
				a.y = get_y(a, mx);
				A[T++] = a;
			}
			else if (E.t == CROSS) {
				a = A[I[E.i][E.j][E.d]];
				a.x = E.x;
				ld mx = (E.x + X[Q + 1]) * .5;
				a.y = get_y(a, mx);
				A[I[E.i][E.j][E.d]] = a;
			}
			else if (E.t == END) {
				A[I[E.i][E.j][HI]].y = INF;
				A[I[E.i][E.j][LO]].y = INF;
				o = 1;
			}
		}

		//for (int k = 0; k < T; k++) {
		//	ld mx = (A[k].x + X[Q + 1]) * .5;
		//	A[k].y = get_y(A[k], mx);
		//}

		assert(T <= 800);
		std::sort(A, A + T);//O(400 * 400 * 800 * log(800))
		
		if (o) {
			int cnt = 0;
			for (int j = T - 1; j >= 0; j--) {
				if (A[j].y < 1e9) break;
				cnt++;
			}
			assert(~cnt & 1); assert(~T & 1); assert(T >= cnt);
			T -= cnt;
		}

		for (int k = 0; k < T; k++) a = A[k], I[a.i][a.j][a.d] = k;
		
		for (int k = 0; k < T - 1; k++) sweep(k, X[Q + 1]);//O(400 * 400 * 800 * 400)

		if (Q == xsz - 2) break;
		for (int k = 0; k < T; k++) {
			A[k].x = X[Q + 1];
			ld mx = (A[k].x + X[Q + 2]) * .5;
			A[k].y = get_y(A[k], mx);
		}
	}
	std::cout << ANS << "\n";
	return;
}
int main() { solve(); return 0; }//boj10910 Random Signal