#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-12;
const ld PI = acos(-1);
const int LEN = 20;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
inline ll sq(ll x) { return x * x; }
inline ld sq(ld x) { return x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}

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
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
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
	Pos rot(ld the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
//Vld intersection(const Pos& ca, const ld& ra, const Pos& cb, const ld& rb) {
Polygon intersection(const Pos& ca, const ld& ra, const Pos& cb, const ld& rb) {
	Pos vec = cb - ca;
	ld distance = vec.mag();
	ld rd = vec.rad();
	if (vec.Euc() > sq(ra + rb) + TOL) return {};
	if (vec.Euc() < sq(ra - rb) - TOL) return {};
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
struct Pii {
	int s, e;
	Pii(int s_ = 0, int e_ = 0) : s(s_), e(e_) {}
};
struct Pow {
	int s, w;
	Pow(int s_, int w_) : s(s_), w(w_) {}
	bool operator < (const Pow& p) const { return s > p.s; }
};
struct Station {
	int x, y;
	int M, L, U;
	Vint r, s, w;
	Station(int x0, int y0, int m0, int l0, int u0) : x(x0), y(y0), M(m0), L(l0), U(u0) { r.clear(); s.clear(); w.clear(); }
	Pos p() const { return Pos(x, y); }
} S[LEN];
struct Event {
	int t;//type
	ld x;
	int ai, aj, ad;
	int bi, bj, bd;
	//Event(int t_) : t(t_) { x = 0; ai = aj = ad = bi = bj = bd = 0; }
	bool operator < (const Event& e) const {
		if (zero(x - e.x)) {
			return t == e.t ? S[ai].r[aj] > S[e.ai].r[e.aj] : t < e.t;
		}
		return x < e.x;
	}
} E;
struct Signal {
	int i, j, d;
	ld x;
	bool operator < (const Signal& s) const { return i < s.i; }
} SG[LEN * LEN * LEN];
struct Prob {
	ld p;
	int i, s;
	bool operator < (const Prob& o) const { return s > o.s; }
};
ld prob[LEN];
bool intersection(const int& ai, const int& aj, const int& bi, const int& bj, Polygon& inx) {
	if (S[ai].p() == S[bi].p()) return 0;
	ll x = S[ai].x - S[bi].x;
	ll y = S[ai].y - S[bi].y;
	ll d = x * x + y * y;
	ll ro = S[ai].r[aj] + S[bi].r[bj];
	ll ri = S[ai].r[aj] - S[bi].r[bj];
	if (d >= sq(ro) || d <= sq(ri)) return 0;
	inx = intersection(S[ai].p(), S[ai].r[aj], S[bi].p(), S[ bi].r[bj]);
	assert(inx.size() == 2);
	return 1;
}
ld get_y(const Signal& s, const ld& x) {
	Pos p = S[s.i].p();
	ld r = S[s.i].r[s.j];
	if ((p.x - r) < x && x < (p.x + r)) {
		ld dy = sqrt(r * r - sq(p.x - x));
		return p.y + dy * (s.d == HI ? 1 : -1);
	}
	return p.y;
}
ld green(const Signal& s, const ld& sx, const ld& ex) {
	ld r = S[s.i].r[s.j];
	ld sy = get_y(s, sx);
	ld ey = get_y(s, ex);
	Pos c = S[s.i].p();

	ld st = norm((c - Pos(sx, sy)).rad());
	ld et = norm((c - Pos(ex, ey)).rad());
	ld t = norm((st - et) * (s.d == HI ? 1 : -1));

	ld fan = r * r * (t * .5 - cos(t * .5) * sin(t * .5));
	ld rec = (ex - sx) * (sy + ey) * .5;
	return fan * (s.d == HI ? 1 : -1) + rec;
}
std::vector<Event> VE;
void sweep(const int& k, const ld& x) {
	if (k < 0 || T < k) return;
	if (zero(SG[k].x - x)) return;
	int sz;

	Signal hi = SG[k + 1];
	Signal lo = SG[k];

	ld ha = green(hi, SG[k].x, x);
	ld la = green(lo, SG[k].x, x);

	ld A = ha - la;
	if (zero(A)) return;

	ld mx = (x + SG[k].x) * .5;
	ld my = (get_y(hi, mx) + get_y(lo, mx)) * .5;
	Pos m = Pos(mx, my);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < S[i].M; j++) {
			ld r = S[i].r[j];
			Pos v = S[i].p() - m;
			IN[i][j] = sign(r * r - v.Euc()) >= 0;
		}
	}

	ld per = 1.;
	std::vector<Prob> P;
	Prob p;
	for (int i = 0; i < N; i++) {
		prob[i] = 1.;
		std::vector<Pow> V;
		int all = S[i].U - S[i].L + 1;
		int L = S[i].L;
		int U = S[i].U;
		for (int j = 0; j < S[i].M; j++) 
			if (IN[i][j]) V.push_back(Pow(S[i].s[j], S[i].w[j]));
		std::sort(V.begin(), V.end());
		sz = V.size();
		for (int j = 0; j < sz; j++) {
			int s = V[j].s;
			int w = V[j].w;
			if (w < L) w = L;
			int diff = U - w + 1;
			if (diff > 0) {
				p.p = (ld)diff / all;
				p.i = i;
				U = w - 1;
				p.s = s;
				P.push_back(p);
			}
		}
	}
	std::sort(P.begin(), P.end());
	ld total = 0;
	sz = P.size();
	for (int i = 0; i < sz; i++) {
		p = P[i];
		ld rp = per * p.p / prob[p.i];
		total += rp * p.s;

		per = per / prob[p.i];
		prob[p.i] -= p.p;
		per = per * prob[p.i];
	}
	SG[k].x = x;
	ANS += total * A;
	return;
}
void sweep_signal(const int& k, const ld& x) {
	sweep(k, x);
	if (0 <= k && k < T) SG[k].x = x;
	return;
}
void solve() {
	T = 0;
	int sz = VE.size();
	for (int i = 0; i < sz; i++) {
		E = VE[i];
		switch (E.t) {

		case START:
			int k = 0;
			Signal s;
			for (k = 0; k < T; k++) {
				s = SG[k];
				ld y = get_y(s, E.x);
				if (y > S[E.ai].y || (zero(y - S[E.ai].y) && SG[k].d == HI)) break;
			}

			sweep_signal(k - 1, E.x);
			for (int j = T + 1; j > k + 1; j--) SG[j] = SG[j - 2];
			
			s.i = E.ai; s.j = E.aj;
			s.x = E.x;

			s.d = LO; SG[k] = s;
			s.d = HI; SG[k + 1] = s;

			T += 2;
			break;

		case END:
			int ui = I[E.ai][E.aj][HI];
			int di = I[E.ai][E.aj][LO];

			sweep_signal(ui - 1, E.x);
			sweep_signal(ui, E.x);
			sweep_signal(di - 1, E.x);
			sweep_signal(di, E.x);

			int T_ = T;
			T = 0;
			for (int j = 0; j < T_; j++) {
				if (j == ui || j == di) continue;
				SG[T] = SG[j];
				T++;
			}
			break;

		case CROSS:
			int PLUS = 0;
			Vint VI;
			ld nxt = E.x;
			for (int j = i + 1; j < sz; j++) {
				const Event& NE = VE[j];
				if (!zero(E.x - NE.x)) { nxt = NE.x; break; }
				if (NE.t != CROSS) continue;
				PLUS++;
			}
			
			for (int j = 0; j < T - 1; j++) {
				if (zero(get_y(SG[j], E.x) - get_y(SG[j + 1], E.x))) {
					VI.push_back(j);
					VI.push_back(j + 1);
				}
			}
			std::sort(VI.begin(), VI.end());
			VI.erase(unique(VI.begin(), VI.end()), VI.end());

			sz = VI.size();
			int ss = -1, ee = -1;
			std::vector<Pii> rev;
			for (int j = 0; j <= sz; j++) {
				int k = -1;
				if (j < sz) {
					k = VI[j];
					sweep_signal(k - 1, E.x);
					sweep_signal(k, E.x);
				}
				if (ss == -1) ss = ee = k;
				else {
					if (k != -1 && zero(get_y(SG[ss], E.x) - get_y(SG[k], E.x))) ee = k;
					else {
						if (ss != -1) rev.push_back(Pii(ss, ee));
						ss = ee = k;
					}
				}
			}

			ld mx = (E.x + nxt) * .5;
			int szr = rev.size();
			for (int j = 0; j < szr; j++) {
				int s = rev[j].s;
				int e = rev[j].e;
				std::vector<std::pair<ld, Signal>> VS;
				for (int k = s; k < e; k++) {
					SG[k].x = E.x;
					VS.push_back(std::make_pair(get_y(SG[k], mx), SG[k]));
				}
				std::sort(VS.begin(), VS.end());
				for (int k = s; k <= e; k++) SG[k] = VS[k].second;
			}

			i += PLUS;
			break;
		}

		for (int j = 0; j < T; j++) {
			const Signal& s = SG[j];
			I[s.i][s.j][s.d] = j;
		}
	}
	return;
}
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	ANS = 0;
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		E.ai = i;
		std::cin >> S[i].x >> S[i].y >> S[i].M >> S[i].L >> S[i].U;
		M = S[i].M;
		S[i].r.resize(M);
		S[i].s.resize(M);
		S[i].w.resize(M);
		for (int j = 0; j < M; j++) {
			E.aj = j;
			std::cin >> S[i].r[j] >> S[i].s[j] >> S[i].w[j];

			E.x = S[i].x - S[i].r[j];
			E.t = START;
			VE.push_back(E);

			E.x = S[i].x + S[i].r[j];
			E.t = END;
			VE.push_back(E);

			for (int k = 0; k < i; k++) {
				if (S[i].p() == S[k].p()) continue;
				for (int m = 0; m < S[k].M; m++) {
					Polygon inx;
					if (intersection(i, j, k, m, inx)) {
						for (const Pos& p : inx) {
							E.x = p.x;
							E.t = CROSS;
							VE.push_back(E);
						}
					}
				}
			}
		}
	}
	std::sort(VE.begin(), VE.end());
	return;
}
int main() { init(); solve(); std::cout << ANS << "\n"; return 0; }