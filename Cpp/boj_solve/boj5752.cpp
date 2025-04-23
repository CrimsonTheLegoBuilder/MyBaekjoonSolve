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
typedef long double ld;
//typedef double ld;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<bool> Vbool;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 105;
const ld TOL = 1e-9;
const ld PI = acos(-1);
inline int sign(const ll& x) { return x < 0 ? -1 : x > 0; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
ll gcd(ll a, ll b) { while (b) { ll tmp = a % b; a = b; b = tmp; } return a; }
inline bool eq(const ld& u, const ld& v) { return zero(u - v); }
inline ll sq(const ll& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
inline ld fit(const ld& x, const ld& lo, const ld& hi) { return std::min(hi, std::max(lo, x)); }

#define SCALE 1
#define __FUCK__ ;
#define WHAT_THE_FUCK
//#define DEBUG

int B, M, N, S;
struct Pos {
	ld x, y;
	int i;
	bool rv;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) { i = -1, rv = 0; }
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	//bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	//bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	//bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
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
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} p0, p1, key, vec; const Pos O = Pos(0, 0); const Pos INVAL = Pos(INF, INF);
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
bool cmpr(const Pos& p, const Pos& q) {
	//bool f1 = O < p;
	//bool f2 = O < q;
	//if (f1 != f2) return f1;
	//int tq = ccw(O, p, q);
	//return !tq ? p.rv > q.rv : tq > 0;
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
ld area(const Polygon& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
struct Seg {
	Pos s, e;
	int i;
	Seg(Pos s_ = Pos(), Pos e_ = Pos(), int i_ = -1) : s(s_), e(e_), i(i_) {}
	bool operator < (const Seg& o) const { return s == o.s ? e < o.e : s < o.s; }
	bool operator == (const Seg& o) const { return s == o.s && e == o.e; }
	//Pos inx(const Seg& o) const { return intersection(s, e, o.s, o.e); }
} seg[LEN], frag[LEN * LEN * 10];
bool intersect(const Seg& p, const Seg& q) { return intersect(p.s, p.e, q.s, q.e); }
Pos intersection(const Seg& p, const Seg& q) {
	if (p.s == q.s) return p.s;
	if (p.s == q.e) return p.s;
	if (p.e == q.s) return p.e;
	if (p.e == q.e) return p.e;
	return intersection(p.s, p.e, q.s, q.e);
}
Polygon INX[LEN];
void inx_sort(Polygon& INX, const Pos& a) {
	std::sort(INX.begin(), INX.end(), [&](const Pos& p, const Pos& q) -> bool {
		return (a - p).Euc() < (a - q).Euc();
		//return (a - p).mag() < (a - q).mag();
		});
	INX.erase(unique(INX.begin(), INX.end()), INX.end());
}
int I0, I1;
std::map<Pos, Polygon> map_pos;
ld A[LEN * LEN + 10];
Polygon cell[LEN * LEN + 10]; int ci;
//std::set<int> cell_i[LEN * LEN + 10];
//int P[LEN * LEN + 10];//disjoint set
//int find(int i) { return P[i] < 0 ? i : P[i] = find(P[i]); }
//bool join(int i, int j) {
//	i = find(i), j = find(j);
//	if (i == j) return 0;
//	if (P[i] < P[j]) P[i] += P[j], P[j] = i;
//	else P[j] += P[i], P[i] = j;
//	return 1;
//}
int V[LEN * LEN * 10];
Vint GS[LEN * LEN * 10];
void dfs(const int& i, int v) {
	V[v] = 1;
	cell[i].push_back(frag[v].s);
	//cell_i[i].insert(v);
	for (const int& w : GS[v]) {
		if (V[w]) continue;
		dfs(i, w);
	}
	return;
}
//struct Info {
//	int i, c;
//	Info(int i_ = 0, int c_ = 0) : i(i_), c(c_) {}
//};
//std::vector<Info> GC[LEN * LEN * 10];
//int zero_one_bfs(int v, int g) {
//	memset(V, -1, sizeof V);
//	std::deque<Info> DQ;
//	DQ.push_front(Info(v, 0));
//	V[v] = 0;
//	while (DQ.size()) {
//		Info p = DQ.front(); DQ.pop_front();
//		if (p.i == g) return V[g];
//		for (const Info& w : GC[p.i]) {
//			if (!~V[w.i]) {
//				if (w.c) DQ.push_back(w);
//				else if (!w.c) DQ.push_front(w);
//				V[w.i] = V[p.i] + w.c;
//			}
//		}
//	}
//#ifdef DEBUG
//	std::cout << "I am stupid\n";
//	assert(~V[g]);
//#endif
//	return V[g];
//}
int inner_check(const Polygon& H, const Pos& p) {//concave
	int sz = H.size(), cnt = 0;
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (zero(cur.y - nxt.y)) continue;
		//if (cur.y == nxt.y) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		//if (nxt.y <= p.y || cur.y > p.y) continue;
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}
bool inner_check_concave(const std::vector<Pos>& H, const Pos& p) {
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (zero(cur.y - nxt.y)) continue;
		//if (cur.y == nxt.y) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		//if (nxt.y <= p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
ld dist(const Pos& p0, const Pos& p1, const Pos& q) {
	ld f0 = dot(p0, p1, q);
	ld f1 = dot(p1, p0, q);
	if (f0 > 0 || f1 > 0) return std::min((p0 - q).mag(), (p1 - q).mag());
	return std::abs(cross(p0, p1, q) / (p0 - p1).mag());
}
ld dist(const Polygon& H, const Pos& p) {
	int sz = H.size();
	ld d = INF;
	for (int i = 0; i < sz; i++) {
		const Pos& p0 = H[i], & p1 = H[(i + 1) % sz];
		d = std::min(d, dist(p0, p1, p));
	}
	return d;
}
struct Circle {
	Pos c;
	ld r;
	Circle(Pos c_ = Pos(0, 0), ld r_ = 0) : c(c_), r(r_) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator < (const Circle& q) const {
		//ld dist = (c - q.c).mag();
		//return r < q.r && dist + r < q.r + TOL;
		return r > q.r;
	}
	bool operator >= (const Circle& q) const {
		ld dist = (c - q.c).mag();
		//return r >= q.r && dist + q.r < r + TOL;
		return r >= q.r && dist + q.r <= r;
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	//bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	//bool operator >= (const Pos& p) const { return sign(r - (c - p).mag()) >= 0; }
	bool operator >= (const Pos& p) const { return r >= (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	Circle& operator *= (const ld& n) { c *= n; r *= n; return *this; }
	//ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
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
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
};
//bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
typedef std::vector<Circle> Disks;
Vld intersections(const Circle& a, const Circle& b) {
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;
	//ll ra = a.r, rb = b.r;
	ld ra = a.r, rb = b.r;
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
struct Arc {
	ld lo, hi;// [lo, hi] - radian range of arc, 0 ~ 2pi
	Arc(ld l_ = 0, ld h_ = 0) : lo(l_), hi(h_) {}
	//bool operator < (const Arc& a) const { return !sign(hi - a.hi) ? lo < a.lo : hi < a.hi; }
	bool operator < (const Arc& a) const { return hi == a.hi ? lo < a.lo : hi < a.hi; }
};
std::vector<Arc> arcs[LEN], valid_arcs[LEN];
//Circle disks[LEN];
//struct Seg {
//	Pos s, e;
//	int i;
//	Seg(Pos s_, Pos e_, int i_ = 0) : s(s_), e(e_), i(i_) {}
//	friend std::ostream& operator << (std::ostream& os, const Seg& S) {
//		os << "Seg[" << S.i + 1 << "] :: Seg.s : " << S.s << " Seg.e : " << S.e;
//		return os;
//	}
//};
Seg make_seg(const ld& lo, const ld& hi, const Circle& c, const int& i) {
	Pos LO = Pos(1, 0).rot(lo) * c.r + c.c;
	Pos HI = Pos(1, 0).rot(hi) * c.r + c.c;
	return Seg(LO, HI, i);
}
typedef std::vector<Seg> Segs;
bool query() {//brute O(N^4)
	map_pos.clear();
	for (int i = 0; i < LEN; i++) {
		arcs[i].clear();
		valid_arcs[i].clear();
	}
	for (int i = 0; i <= S; i++) INX[i].clear();
	for (int i = 0; i <= I0; i++) GS[i].clear();
	for (int i = 0; i <= ci; i++) cell[i].clear();

	//std::cout << "query start::\n";

	std::cin >> B; Polygon P(B); for (Pos& p : P) std::cin >> p;// , p *= SCALE;
	std::cin >> N; Polygon I(N); for (Pos& p : I) std::cin >> p;// , p *= SCALE;
	std::cin >> M; Disks C(M); for (Circle& c : C) std::cin >> c;// , c *= SCALE;

	//std::cout << "input OK::\n";

	if (!B && !N && !M) return 0;

	//circle remove
	std::sort(C.begin(), C.end());
	Vbool F(M, 1);
	for (int i = 0; i < M; i++) {
		for (int j = i + 1; j < M; j++) {
			if (C[i] >= C[j]) F[j] = 0;
		}
	}
	Disks tmp;
	for (int i = 0; i < M; i++) if (F[i]) tmp.push_back(C[i]);
	C = tmp;
	M = C.size();
	//circle remove

	//if (N >= 778) std::cout << "q = (" << I[778].x << ", " << I[778].y << ")\n";
	//if (N >= 777) std::cout << "p = (" << I[777].x << ", " << I[777].y << ")\n";
	//if (N >= 776) std::cout << "r = (" << I[776].x << ", " << I[776].y << ")\n";
	//std::cout << "C = [\n";
	//for (Circle& c : C) {
	//	std::cout << "(" << c.c.x << ", " << c.c.y << ", " << c.r << "),\n";
	//}
	//std::cout << "]\n";

	//informer remove
	F.resize(N, 1);
	for (int i = 0; i < N; i++) {
		for (const Circle& c : C) {
			if (c >= I[i]) {
				F[i] = 0;
				break;
			}
		}
	}
	//informer remove

	//get segments
	Segs segs;
	for (int i = 0; i < M; i++) {
		const Circle& p = C[i];
		arcs[i] = { Arc(0, 0) };
		for (int j = 0; j < M; j++) {
			if (i == j) continue;
			Vld inxs = intersections(C[i], C[j]);
			if (!inxs.size()) continue;
			ld lo, hi;
			if (inxs.size() == 1) { lo = hi = inxs[0]; }
			else { assert(inxs.size() == 2); lo = inxs[0], hi = inxs[1]; }
			if (lo <= hi) arcs[i].push_back(Arc(lo, hi));
			else {
				arcs[i].push_back(Arc(0, hi));
				arcs[i].push_back(Arc(lo, PI * 2));
			}
		}
		std::sort(arcs[i].begin(), arcs[i].end());
		arcs[i].push_back(Arc(PI * 2, PI * 2));
		ld hi = 0;
		for (const Arc& a : arcs[i]) {//sweep circle
			if (a.lo > hi) {
				Pos s = p.p(hi), e = p.p(a.lo);
				if (e < s) std::swap(s, e);
				segs.push_back(Seg(s, e));
				hi = a.hi;
			}
			else hi = std::max(hi, a.hi);
		}
	}
	//get segments

	//std::cout << "segs:: " << segs.size() << "\n";

	//get intersections
	Polygon INXS;
	std::sort(segs.begin(), segs.end());
	segs.erase(unique(segs.begin(), segs.end()), segs.end());
	S = segs.size();
	for (int i = 0; i < S; i++) {
		for (int j = i + 1; j < S; j++) {
			if (intersect(segs[i], segs[j])) {
				Pos inx = intersection(segs[i], segs[j]);
				INX[i].push_back(inx);
				INX[j].push_back(inx);
				INXS.push_back(inx);
			}
		}
	}
	//get intersections

	//get frags
	std::sort(INXS.begin(), INXS.end());
	INXS.erase(unique(INXS.begin(), INXS.end()), INXS.end());

	//std::cout << "INXS.sz:: " << INXS.size() << "\n";

	I0 = 0;
	for (int i = 0; i < S; i++) {
		inx_sort(INX[i], seg[i].s);
		Polygon& v = INX[i];
		int sz = v.size();
		for (int j = 0; j < sz - 1; j++) {
			frag[I0] = Seg(v[j], v[j + 1]);
			frag[I0].i = I0;
			I0++;
		}
	}
	I1 = I0;
	for (int i = 0; i < S; i++) {
		Polygon& v = INX[i];
		int sz = v.size();
		for (int j = 0; j < sz - 1; j++) {
			frag[I0] = Seg(v[j + 1], v[j]);
			frag[I0].i = I0;
			I0++;
		}
	}
	//get frags

	//get graph
	for (int i = 0; i < I0; i++) {
		key = frag[i].s;
		vec = frag[i].e - frag[i].s;
		vec.i = frag[i].i;
		map_pos[key].push_back(vec);

		key = frag[i].e;
		vec = frag[i].s - frag[i].e;
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
			assert(cur.rv != nxt.rv);
			GS[nxt.i].push_back(cur.i);
		}
	}
	//get graph

	//get polygons
	memset(V, 0, sizeof V);
	ci = 0;
	for (int i = 0; i < I0; i++) {
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
				//cell_i[ci].clear();
				A[ci] = 0;
				ci--;
			}
			ci++;
		}
	}
	//get polygons

	//std::cout << "ci:: " << ci << "\n";

	//get informer's id & distance
	Vint OK, ID;
	for (int i = 0; i < N; i++) if (F[i]) OK.push_back(i);
	for (const int i : OK) {
		for (int c = 0; c < ci; c++) {
			//std::cout << "fuck::\n";
			if (inner_check_concave(cell[c], I[i])) {
				//if (inner_check(cell[c], I[i])) {
				F[i] = 0;
				break;
			}
		}
	}
	for (const int& i : OK) if (F[i]) ID.push_back(i);
	ld d = 0;
	int ret = -1;
	for (const int& i : ID) {
		ld t = dist(P, I[i]);
		if (d < t) {
			//if (sign(d - t) < 0) {
			d = t;
			ret = i;
		}
	}
	//get informer's id & distance

	if (ret == -1) std::cout << "Mission impossible\n";
	else std::cout << "Contact informer " << ret + 1 << "\n";
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	//freopen("impos.in", "r", stdin);
	//freopen("impos.txt", "w", stdout);
	while (query());
	return;
}
int main() { solve(); return 0; }//5752