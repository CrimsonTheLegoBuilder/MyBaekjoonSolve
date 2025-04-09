#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <iomanip>
#include <chrono>
typedef long long ll;
typedef long double ld;
//typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<size_t> Vidx;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
typedef std::vector<bool> Vbool;
const ld INF = 1e17;
const ld TOL = 1e-9;
const int LEN = 105;
const ld PI = acos(-1);
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ll sq(const ll& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
inline ld fit(const ld& x, const ld& lo = 0, const ld& hi = 1) { return std::min(hi, std::max(lo, x)); }

#define STRONG 0
#define WEAK 1

#define LO x
#define HI y

#define BLACK   0
#define RED     (1 << 0)
#define GREEN   (1 << 1)
#define BLUE    (1 << 2)
#define YELLOW  (RED | GREEN)
#define MAGENTA (RED | BLUE)
#define CYAN    (GREEN | BLUE)
#define WHITE   (RED | GREEN | BLUE)

int N, M, K, Q, I[1 << 3], C[LEN];
ld A[1 << 3];
struct Pos {
	ld x, y;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { -x, -y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	Pos rot(const ld& t) { return { x * cos(t) - y * sin(t), x * sin(t) + y * cos(t) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	//ld rad() const { return atan2(y, x); }
	//friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << "  (" << p.x << ", " << p.y << ")"; return os; }
} L[1 << 3]; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
Polygon P[LEN], H[1 << 3];
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return dot(d1, d2, d1, d3) / (d2 - d1).Euc(); }
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q, const int& f = STRONG) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) >= f || ccw(p1, p2, q) >= f;
	return ccw(p0, p1, q) >= f && ccw(p1, p2, q) >= f;
}
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
ld area(const Polygon& H) {
	int sz = H.size();
	ld a = 0;
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return a * .5;
}
//void norm(Polygon& H) {
//	ld a = area(H);
//	assert(!zero(a));
//	if (a < 0) std::reverse(H.begin(), H.end());
//	return;
//}
struct Seg {
	Pos s, e;
	Seg(Pos s_ = Pos(), Pos e_ = Pos()) : s(s_), e(e_) {}
	Pos dir() const { return (s - e).unit(); }
	bool operator < (const Seg& l) const { return s == l.s ? e < l.e : s < l.s; }
	bool operator == (const Seg& l) const { return s == l.s && e == l.e; }
	Pos p(const ld& rt = .5) const { return s + (e - s) * rt; }
	ld green(const ld& lo = 0, const ld& hi = 1) const {
		ld d = hi - lo;
		ld ratio = (lo + hi) * .5;
		Pos m = p(ratio);
		return m.y * d * (s.x - e.x);
	}
};
Seg S[1 << 3][LEN];
typedef std::vector<Seg> Segs;
ld dot(const Seg& p, const Seg& q) { return dot(p.s, p.e, q.s, q.e); }
bool intersect(const Seg& u, const Seg& v) { return intersect(u.s, u.e, v.s, v.e); }
ld intersection(const Seg& s1, const Seg& s2, const bool& f = STRONG) {
	const Pos& p1 = s1.s, p2 = s1.e, q1 = s2.s, q2 = s2.e;
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	ld a2 = ((p2 - p1) / (p1 - q1)) / -det;
	if (f == WEAK) return fit(a1, 0, 1);
	if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return -1;
}
bool inner_check(const Polygon& H, const Pos& q, const Pos& d = Pos(0, 0)) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		const Pos& p1 = H[i], & p2 = H[(i + 1) % sz];
		if (ccw(p1, p2, q) < 0) return 0;
		if (on_seg_strong(p1, p2, q) && !eq(d.x, d.y)) {
			if (sign((p2 - p1) / d) > 0) return 1;
			else return 0;
		}
		if (on_seg_strong(p1, p2, q)) return 2;
	}
	return 1;
}
bool inner_check_concave(const std::vector<Pos>& H, const Pos& p, const Pos& s, const Pos& e) {
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) {
			assert(collinear(cur, nxt, s, e));
			return dot(cur, nxt, s, e) > 0 ? 1 : 0;
		}
		if (zero(cur.y - nxt.y)) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
Pos get_pos(const Pos& l, const Seg& p, const Seg& q) {
	Pos p1 = p.s, p2 = p.e, q1 = q.s, q2 = q.e;
	if (!inside(p2, l, p1, q1, WEAK) && !inside(p2, l, p1, q2, WEAK)) {
		if (intersect(l, p1, q1, q2) && intersect(l, p2, q1, q2)) return Pos(0, 1);
		else return Pos(0, 0);
	}
	Polygon tri = { p1, p2, l };
	bool in1 = inner_check(tri, q1), in2 = inner_check(tri, q2);
	if (!in1 && !in2) return Pos(0, 0);
	ld r1 = 0, r2 = 1;
	if (in1 && in2) {
		r1 = intersection(p, Seg(l, q1), WEAK);
		r2 = intersection(p, Seg(l, q2), WEAK);
	}
	else if (in1) r1 = intersection(p, Seg(l, q1), WEAK);
	else if (in2) r2 = intersection(p, Seg(l, q2), WEAK);
	else r1 = r2 = 0;
	if (r2 < r1) std::swap(r1, r2);
	return Pos(r1, r2);
}
struct Event {
	ld x;
	int f;
	bool operator < (const Event& o) const { return eq(x, o.x) ? f < o.f : sign(x - o.x) < 0; }
	bool operator == (const Event& o) const { return eq(x, o.x) && f == o.f; }
};
typedef std::vector<Event> Ve;
Pos cen;
Segs Z[1 << 3];
bool cmpt(const Seg& p, const Seg& q) {
	assert(ccw(cen, p.s, p.e));
	assert(ccw(cen, q.s, q.e));
	Pos u = p.s - cen;
	Pos v = q.s - cen;
	bool f1 = O < u;
	bool f2 = O < v;
	if (f1 != f2) return f1;
	assert(!zero(u / v));
	return u / v > 0;
}
void cut(const Pos& s, const Pos& e, const Pos& p0, const Pos& p1, const Pos& p2, Vld& V, Ve& tmp) {
	Seg s0 = Seg(s, e);
	if (on_seg_strong(s, e, p1)) {
		ld x = projection(s, e, p1);
		int tq1 = ccw(p0, p1, p2);
		assert(tq1);
		if (!ccw(s, e, p0)) {
			int d = sign(dot(s, e, p0, p1));
			if (d > 0) {
				V.push_back(x);
				if (tq1 > 0) tmp.push_back({ x, -1 });
				else tmp.push_back({ x, 1 }), tmp.push_back({ x, -1 });
			}
			else {
				if (tq1 > 0) return;
				else V.push_back(x), tmp.push_back({ x, -1 });
			}
		}
		else if (!ccw(s, e, p2)) {
			int d = sign(dot(s, e, p1, p2));
			if (d > 0) {
				V.push_back(x);
				if (tq1 > 0) tmp.push_back({ x, 1 });
				else tmp.push_back({ x, 1 }), tmp.push_back({ x, -1 });
			}
			else {
				if (tq1 > 0) return;
				else V.push_back(x); tmp.push_back({ x, 1 });
			}
		}
		else {
			int tq0 = ccw(s, e, p0), tq2 = ccw(s, e, p2);
			assert(tq0 && tq2);
			if (tq0 != tq2) {
				int tq = ccw(s, e, p0, p2);
				assert(tq);
				V.push_back(x);
				if (tq < 0) tmp.push_back({ x, 1 });
				else tmp.push_back({ x, -1 });
			}
			else if (ccw(p0, p1, p2) < 0) {
				ld x = projection(s, e, p1); V.push_back(x);
				tmp.push_back({ x, 1 }); tmp.push_back({ x, -1 });
			}
		}
	}
	else {
		if (!ccw(s, e, p2)) return;
		Seg sk = Seg(p1, p2);
		ld x = intersection(s0, sk);
		if (x < 0) return;
		int tq = ccw(s, e, p1, p2);
		assert(tq);
		if (tq < 0) V.push_back(x), tmp.push_back({ x, 1 });
		else V.push_back(x), tmp.push_back({ x, -1 });
	}
	return;
}
void clean(const Pos& s, const Pos& e, const Seg& s0, const int& c0, Ve& tmp, Ve& ve) {
	std::sort(tmp.begin(), tmp.end());
	tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
	if (tmp.empty()) {
		Pos m = s0.p(.5);
		if (inner_check_concave(H[c0], m, s, e)) {
			ve.push_back({ 0, 1 });
			ve.push_back({ 1, -1 });
		}
		return;
	}
	int v = 0, szt = tmp.size();
	if (tmp[0].f == -1) {
		if (tmp[0].x > 0) ve.push_back({ 0, 1 });
		else v++;
	}
	if (tmp.back().f == 1) {
		if (tmp.back().x < 1) ve.push_back({ 1, -1 });
		else szt--;
	}
	for (v; v < szt; v++) ve.push_back(tmp[v]);
	return;
}
void query(const int& q) {
	std::cin >> L[RED] >> L[GREEN] >> L[BLUE];
	memset(A, 0, sizeof A);
	memset(I, -1, sizeof I);
	memset(C, 0, sizeof C);
	for (int c = 1; c < (1 << 3); c++) H[c].clear(), Z[c].clear();
	for (int c = 1; c < (1 << 3); c <<= 1) {
		int f0 = inner_check(P[0], L[c]), fk = 0;
		if (f0 != 1) continue;
		if (f0) { C[0] |= c; I[c] = 0; }
		for (int k = 1; k <= K; k++) {
			fk = inner_check(P[k], L[c]);
			if (fk) {
				C[0] -= c;
				C[k] |= c;
				I[c] = k;
				break;
			}
		}
		if (fk) continue;
		for (int k = 1; k <= K; k++) {
			const Polygon& T = P[k];
			M = T.size();
			Pos pl = T[0], pr = T[0];
			for (int j = 0; j < M; j++) {
				if (ccw(L[c], pl, T[j]) > 0) pl = T[j];
				if (ccw(L[c], pr, T[j]) < 0) pr = T[j];
			}
			S[c][k] = Seg(pr, pl);
		}
		int sz = P[0].size();
		for (int i = 0; i < sz; i++) {
			const Pos& u = P[0][i], & v = P[0][(i + 1) % sz];
			assert(ccw(L[c], u, v) > 0);
			Seg w = Seg(u, v);
			Polygon VP = { Pos(0, 0) };
			for (int k = 1; k <= K; k++) {
				Pos se = get_pos(L[c], w, S[c][k]);
				if (!eq(se.x, se.y)) VP.push_back(se);
			}
			VP.push_back(Pos(1, 1));
			std::sort(VP.begin(), VP.end());
			ld hi = 0;
			for (const Pos& p : VP) {
				if (hi < p.LO) {
					Pos s = w.p(hi), e = w.p(p.LO);
					Z[c].push_back(Seg(s, e));
					hi = p.HI;
				}
				else hi = std::max(hi, p.HI);
			}
		}
		for (int k = 1; k <= K; k++) {
			const Polygon& T = P[k];
			sz = T.size();
			for (int i = 0; i < sz; i++) {
				const Pos& u = T[i], & v = T[(i + 1) % sz];
				if (ccw(L[c], u, v) >= 0) continue;
				Seg w = Seg(v, u);
				Polygon VP = { Pos(0, 0) };
				for (int k_ = 1; k_ <= K; k_++) {
					if (k_ == k) continue;
					Pos se = get_pos(L[c], w, S[c][k_]);
					if (!eq(se.x, se.y)) VP.push_back(se);
				}
				VP.push_back(Pos(1, 1));
				std::sort(VP.begin(), VP.end());
				ld hi = 0;
				for (const Pos& p : VP) {
					if (hi < p.LO) {
						Pos s = w.p(hi), e = w.p(p.LO);
						Z[c].push_back(Seg(s, e));
						hi = p.HI;
					}
					else hi = std::max(hi, p.HI);
				}
			}
		}
		cen = L[c];
		std::sort(Z[c].begin(), Z[c].end(), cmpt);
		sz = Z[c].size();
		for (int z = 0; z < sz; z++) {
			H[c].push_back(Z[c][z].s);
			H[c].push_back(Z[c][z].e);
		}
		H[c].erase(unique(H[c].begin(), H[c].end()), H[c].end());
		if (H[c][0] == H[c].back()) H[c].pop_back();
	}

	if (!I[RED] && !I[GREEN] && !I[BLUE]) {//R & G & B
		Segs VS;
		for (int i = 0; i < 3; i++) {
			int c0 = 1 << i;
			int c1 = (1 << ((i + 1) % 3));
			int c2 = (1 << ((i + 2) % 3));
			int sz = H[c0].size();
			for (int j = 0; j < sz; j++) {
				Ve ve = { { 0, 1 }, { 1, -1 } };
				Vld V = { 0, 1 };
				int j1 = (j + 1) % sz;
				const Pos& s = H[c0][j], & e = H[c0][j1];
				Seg s0 = Seg(s, e);
				for (const int cc : { c1, c2 }) {
					int sz1 = H[cc].size();
					Ve tmp;
					for (int k = 0; k < sz1; k++) {
						int k0 = (k - 1 + sz1) % sz1;
						int k2 = (k + 1) % sz1;
						const Pos& p0 = H[cc][k0], & p1 = H[cc][k], & p2 = H[cc][k2];
						cut(s, e, p0, p1, p2, V, tmp);
					}
					clean(s, e, s0, cc, tmp, ve);
				}
				std::sort(V.begin(), V.end());
				V.erase(unique(V.begin(), V.end()), V.end());
				std::sort(ve.begin(), ve.end());
				Polygon vp;
				int szr = ve.size(), szx = V.size(), cnt = 0;
				for (int x = 0, k = 0; x < szx - 1; x++) {
					const ld& s = V[x], e = V[x + 1];
					while (k < szr && eq(ve[k].x, s)) { cnt += ve[k].f; k++; }
					if (cnt > 2) vp.push_back(Pos(s, e));
				}
				for (const Pos& se : vp) VS.push_back(Seg(s0.p(se.LO), s0.p(se.HI)));
			}
		}
		std::sort(VS.begin(), VS.end());
		VS.erase(unique(VS.begin(), VS.end()), VS.end());
		for (const Seg& se : VS) A[WHITE] += se.green();
	}

	for (int i = 0; i < 3; i++) {//R & G, G & B, B & R
		int c1 = (1 << ((i + 1) % 3));
		int c2 = (1 << ((i + 2) % 3));
		if (!I[c1] && !I[c2]) {
			int c = c1 | c2;
			Segs VS;
			for (const int& ca : { c1, c2 }) {
				int cb = (ca == c1 ? c2 : c1);
				int sza = H[ca].size();
				int szb = H[cb].size();
				for (int j = 0; j < sza; j++) {
					Ve ve = { { 0, 1 }, { 1, -1 } };
					Vld V = { 0, 1 };
					int j1 = (j + 1) % sza;
					const Pos& s = H[ca][j], & e = H[ca][j1];
					Seg s0 = Seg(s, e);
					Ve tmp;
					for (int k = 0; k < szb; k++) {
						int k0 = (k - 1 + szb) % szb;
						int k2 = (k + 1) % szb;
						const Pos& p0 = H[cb][k0], & p1 = H[cb][k], & p2 = H[cb][k2];
						cut(s, e, p0, p1, p2, V, tmp);
					}
					clean(s, e, s0, cb, tmp, ve);
					std::sort(V.begin(), V.end());
					V.erase(unique(V.begin(), V.end()), V.end());
					std::sort(ve.begin(), ve.end());
					Polygon vp;
					int szr = ve.size(), szx = V.size(), cnt = 0;
					for (int x = 0, k = 0; x < szx - 1; x++) {
						const ld& s = V[x], e = V[x + 1];
						while (k < szr && eq(ve[k].x, s)) { cnt += ve[k].f; k++; }
						if (cnt > 1) vp.push_back(Pos(s, e));
					}
					for (const Pos& se : vp) VS.push_back(Seg(s0.p(se.LO), s0.p(se.HI)));
				}
			}
			std::sort(VS.begin(), VS.end());
			VS.erase(unique(VS.begin(), VS.end()), VS.end());
			for (const Seg& se : VS) A[c] += se.green();
		}
	}

	for (int c = 1; c < (1 << 3); c <<= 1) {//R, G, B
		if (!I[c]) A[c] += area(H[c]);
	}

	for (int k = 1; k <= K; k++) {//area of blue polygons
		A[C[k]] += area(P[k]);
	}
	if (!I[RED] && !I[GREEN] && !I[BLUE]) {//i-e - R & G & B
		for (int c = 1; c < WHITE; c++) A[c] -= A[WHITE];
	}
	for (int i = 0; i < 3; i++) {//i-e - c1 & c2
		int c1 = (1 << ((i + 1) % 3));
		int c2 = (1 << ((i + 2) % 3));
		if (!I[c1] && !I[c2]) {
			int c = c1 | c2;
			A[c1] -= A[c];
			A[c2] -= A[c];
		}
	}
	A[BLACK] = area(P[0]);
	for (int c = 1; c < (1 << 3); c++) A[BLACK] -= A[c];
	for (int c = 0; c < (1 << 3); c++) A[c] = std::max(A[c], (ld).0);
	std::cout << "Case #" << q << ":\n";
	std::cout << "R: " << A[RED] << "\n";
	std::cout << "G: " << A[GREEN] << "\n";
	std::cout << "B: " << A[BLUE] << "\n";
	std::cout << "Y: " << A[YELLOW] << "\n";
	std::cout << "M: " << A[MAGENTA] << "\n";
	std::cout << "C: " << A[CYAN] << "\n";
	std::cout << "W: " << A[WHITE] << "\n";
	std::cout << "L: " << A[BLACK] << "\n";
	return;
}
#define TIME
void solve() {
#ifdef TIME
	auto start = std::chrono::high_resolution_clock::now();
#endif
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(13);
	freopen("tests/candle_09.in", "r", stdin);
	freopen("tests/candle_09c.out", "w", stdout);
	std::cin >> N;
	P[0].resize(N);
	for (Pos& p : P[0]) std::cin >> p;
	std::cin >> K;
	for (int i = 1; i <= K; i++) {
		std::cin >> M;
		P[i].resize(M);
		for (Pos& p : P[i]) std::cin >> p;
	}
	std::cin >> Q;
	for (int q = 1; q <= Q; q++) query(q);
#ifdef TIME
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Start time: " << std::chrono::duration_cast<std::chrono::microseconds>(start.time_since_epoch()).count() << " ¥ìs\n";
	std::cout << "End time: " << std::chrono::duration_cast<std::chrono::microseconds>(end.time_since_epoch()).count() << " ¥ìs\n";
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "Execution time: " << duration.count() << " microseconds\n";
#endif
	return;
}
int main() { solve(); return 0; }//kitpc? 14? candle & candle & candle & shadow