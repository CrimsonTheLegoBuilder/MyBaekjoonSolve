#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <cassert>
#include <vector>
#include <set>
#include <map>
typedef long long ll;
typedef double ld;
//typedef long double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 505;
const ld TOL = 1e-8;
const ld PI = acos(-1);
//const ll MOD = 1'000'000'007;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ll sq(const ll& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
inline ld fit(const ld& x, const ld& lo, const ld& hi) { return std::min(hi, std::max(lo, x)); }
int gcd(int a, int b) { return !b ? a : gcd(b, a % b); }
#define LINE 1
#define CIRCLE 2
#define STRONG 0
#define WEAK 1
#define LO x
#define HI y
#define NO_MERGE 0
#define EDGE 1
#define EDGE_IGNORE 0
//#define AUTO_CHECK
//#define DEBUG
//#define EVENT_COUNT

#ifdef AUTO_CHECK
#include <fstream>
#define WHAT_THE_FUCK
#endif

int ai, bi;
int N, M, K, T, Q;
ld RET = 0;
ld mxa[LEN], mxb[LEN];
struct Pos {
	ld x, y;
	int i, d;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) { i = 0, d = 0; }
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	//bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	//bool operator < (const Pos& r) const { return x == r.x ? y == r.y ? d < r.d : y < r.y : x < r.x; }
	bool operator < (const Pos& r) const { return zero(x - r.x) ? zero(y - r.y) ? d < r.d : y < r.y : x < r.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const ld& n) { x /= n; y /= n; return *this; }
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
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << "(" << p.x << ", " << p.y << ")"; return os; }
} pos[LEN << 3]; const Pos O = Pos(0, 0);
typedef std::set<Pos> SetPos;
typedef std::vector<Pos> Polygon;
int len[2];
bool cmpx(const Pos& p, const Pos& q) { return zero(p.x - q.x) ? p.y < q.y : p.x < q.x; }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ld ret = cross(d1, d2, d3, d4); return sign(ret); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
bool parallel(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3, d4); }
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q, const int& f = STRONG) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) >= f || ccw(p1, p2, q) >= f;
	return ccw(p0, p1, q) >= f && ccw(p1, p2, q) >= f;
}
ld rad(const Pos& p1, const Pos& p2, const Pos& p3) { return rad(p2 - p1, p3 - p2); }
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2, bool weak = WEAK, bool edge = EDGE) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	if (edge == EDGE_IGNORE) return f1 && f2;
	bool f3;
	if (weak == WEAK) f3 = on_seg_weak(s1, s2, d1) ||
		on_seg_weak(s1, s2, d2) ||
		on_seg_weak(d1, d2, s1) ||
		on_seg_weak(d1, d2, s2);
	else f3 = on_seg_strong(s1, s2, d1) ||
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
bool norm(Polygon& H) {
	ld a = area(H);
	if (zero(a)) return 1;
	if (a < 0) std::reverse(H.begin(), H.end());
	return 0;
}
int inner_check(const std::vector<Pos>& H, const Pos& p) {//concave
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (cur.y == nxt.y) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		//if (nxt.y <= p.y || cur.y > p.y) continue;
		if (nxt.y < p.y + TOL || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}
bool inner_check(const Polygon& B, const int& b, const Polygon& A, const int& a) {
	int sza = A.size();
	int szb = B.size();
	const Pos& a0 = A[(a - 1 + sza) % sza], & a1 = A[a], & a2 = A[(a + 1) % sza];
	const Pos& b0 = B[(b - 1 + szb) % szb], & b1 = B[b], & b2 = B[(b + 1) % szb];
	return inside(b0, b1, b2, a0)
		&& inside(b0, b1, b2, a2)
		&& !inside(a0, a1, a2, b0, WEAK)
		&& !inside(a0, a1, a2, b2, WEAK);
}
//bool inner_check(const Pos& a0, const Pos& a1, const Pos& a2, const Pos& b0, const Pos& b1, const Pos& b2) {
//	int CCW;
//	CCW = ccw(a0, a1, a2);
//	if (CCW > 0 && ccw(a1, a0, b0) < 0 && ccw(a1, a2, b0) > 0) return 1;
//	if (CCW < 0 && !(ccw(a1, a0, b0) >= 0 && ccw(a1, a2, b0) <= 0)) return 1;
//	if (CCW > 0 && ccw(a1, a0, b2) < 0 && ccw(a1, a2, b2) > 0) return 1;
//	if (CCW < 0 && !(ccw(a1, a0, b2) >= 0 && ccw(a1, a2, b2) <= 0)) return 1;
//	CCW = ccw(b0, b1, b2);
//	if (CCW > 0 && ccw(b1, b0, a0) < 0 && ccw(b1, b2, a0) > 0) return 1;
//	if (CCW < 0 && !(ccw(b1, b0, a0) >= 0 && ccw(b1, b2, a0) <= 0)) return 1;
//	if (CCW > 0 && ccw(b1, b0, a2) < 0 && ccw(b1, b2, a2) > 0) return 1;
//	if (CCW < 0 && !(ccw(b1, b0, a2) >= 0 && ccw(b1, b2, a2) <= 0)) return 1;
//	return 0;
//}
//Polygon rotate_and_norm(Polygon B, const int& j0, const Polygon& A, const int& i0, const ld& t, Pos& v) {
//	int sz = B.size();
//	for (int j = 0; j < sz; j++) B[j] = B[j].rot(t);
//	v = A[i0] - B[j0];
//	for (int j = 0; j < sz; j++) B[j] += v;
//	return B;
//}
struct Seg {
	Pos s, e;
	int h, i;
	Seg(Pos s_ = Pos(0, 0), Pos e_ = Pos(0, 0)) : s(s_), e(e_) { h = 0; i = 0; }
	bool operator == (const Seg& S) const { return s == S.s && e == S.e; }
	bool operator != (const Seg& S) const { return !(*this == S); }
	//bool operator < (const Seg& S) const { return (s == S.s) ? e < S.e : s < S.s; }
	bool operator < (const Seg& rhs) const {
		if (rhs.s == s) return ccw(rhs.s, rhs.e, e) < 0;
		if (rhs.s < s) return ccw(rhs.s, rhs.e, s) < 0;
		return ccw(rhs.s, rhs.e, e) < 0;
	}
	Pos p(const ld& rt) const { return s + (e - s) * rt; }
	ld green(const ld& lo = 0, const ld& hi = 1) const {
		ld d = hi - lo;
		ld ratio = (lo + hi) * .5;
		Pos m = p(ratio);
		return m.y * d * (s.x - e.x);
	}
	Seg operator - (const ld& n) const {
		Pos v = ~(e - s).unit();
		return { s + v * n, e + v * n };
	}
	Seg& operator -= (const ld& n) {
		Pos v = ~(e - s).unit();
		s += v * n; e += v * n;
		return *this;
	}
	ld mag() const { return (e - s).mag(); }
	friend std::ostream& operator << (std::ostream& os, const Seg& S) { os << "DEBUG::Seg s: " << S.s << " | e: " << S.e << " DEBUG::Seg\n"; return os; }
} seg[LEN << 3];
bool intersect(const Seg& u, const Seg& v) { return intersect(u.s, u.e, v.s, v.e); }
ld intersection(const Seg& s1, const Seg& s2, const bool& f = STRONG) {
	const Pos& p1 = s1.s, p2 = s1.e, q1 = s2.s, q2 = s2.e;
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	ld a2 = ((p2 - p1) / (p1 - q1)) / -det;
	if (f == WEAK) {
		if (ccw(s1.s, s1.e, s2.s) * ccw(s1.s, s1.e, s2.e) > 0) return -1;
		return a1;
	}
	if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return -1;
}
//Vld intersections(const Seg& l, const Polygon& H) {
//	int sz = H.size();
//	Vld ret;
//	const Pos& s = l.s, & e = l.e;
//	for (int i = 0; i < sz; i++) {
//		const Pos& p0 = H[i], & p1 = H[(i + 1) % sz];
//		if (parallel(s, e, p0, p1)) continue;
//		if (ccw(s, e, p0) * ccw(s, e, p1) <= 0) {
//			ld x = intersection(Seg(s, e), Seg(p0, p1));
//			ret.push_back(x);
//		}
//	}
//	std::sort(ret.begin(), ret.end());
//	ret.erase(unique(ret.begin(), ret.end(), eq), ret.end());
//	return ret;
//}
struct Bound {
	Pos s, e;
	int i;
	Bound(Pos S = Pos(0, 0), Pos E = Pos(0, 0), int I = -1) : s(S), e(E), i(I) {}
	bool operator == (const Bound& S) const { return s == S.s && e == S.e; }
	bool operator != (const Bound& S) const { return !(*this == S); }
	bool operator < (const Bound& S) const {
		ld tq = cross(s, e, S.s, S.e);
		if (zero(tq)) {
			if (collinear(s, e, S.s, S.e)) {
				if (s == S.s) return cmpx(e, S.e);
				return cmpx(s, S.s);
			}
			return ccw(s, e, S.s) > 0;
		}
		return tq > 0;
	}
	friend std::ostream& operator << (std::ostream& os, const Bound& S) {
		os << "DEBUG::Seg:: s: " << S.s << " | e: " << S.e << " DEBUG::Seg\n";
		return os;
	}
};
bool collinear(const Bound& a, const Bound& b) { return collinear(a.s, a.e, b.s, b.e); }
bool intersect(const int& a, const int& b) {
	Seg A = seg[a], B = seg[b];
	if (A.h == B.h) return 0;
	if (!intersect(A.s, A.e, B.s, B.e, STRONG)) return 0;
	if (intersect(A.s, A.e, B.s, B.e, WEAK, EDGE_IGNORE)) return 1;
	if (A.h != ai) std::swap(A, B);
	if (A.e.i != (A.s.i + 1) % len[A.h]) std::swap(A.s, A.e);
	assert((A.s.i + 1) % len[A.h] == A.e.i);
	if (ai == 0) {
		if (on_seg_weak(A.s, A.e, B.s)) return ccw(A.s, A.e, B.e) > 0;
		if (on_seg_weak(A.s, A.e, B.e)) return ccw(A.s, A.e, B.s) > 0;
	}
	else {
		if (on_seg_weak(A.s, A.e, B.s)) return ccw(A.s, A.e, B.e) < 0;
		if (on_seg_weak(A.s, A.e, B.e)) return ccw(A.s, A.e, B.s) < 0;
	}
	return 0;
}
//struct Event {
//	ld t;
//	Pos v;
//	Event(ld t_ = 0, Pos v_ = Pos()) : t(t_), v(v_) {}
//	bool operator < (const Event& e) const { return zero(t - e.t) ? v < e.v : t < e.t; }
//	bool operator == (const Event& e) const { return zero(t - e.t) && v == e.v; }
//};
//std::vector<Event> events;
//inline void conv(const Polygon& B, const Event& E, Polygon& B2) {
//	for (const Pos& p : B) B2.push_back(p.rot(E.t) + E.v);
//	T = 0; for (Pos& p : B2) p.i = T, T++, p.d = 0;
//	return;
//}
//#define __WHAT_THE__ !
//#define __FUCK_YOU__ 1
//bool ST_DEBUG = __WHAT_THE__ __FUCK_YOU__;
class SplayTree {
	struct Node {
		int i;
		Node* l;
		Node* r;
		Node* p;
		Node(int i) : i(i), l(0), r(0), p(0) {}
		~Node() { if (l) delete l; if (r) delete r; }
	} *root;

	void rotate(Node* x) {
		Node* p = x->p;     //p : parent of x
		if (!p) return;     //if x == root: return
		Node* b = 0;
		if (x == p->l) {    //if x is left child
			p->l = b = x->r;
			x->r = p;
		}
		else {              //else if x is right child
			p->r = b = x->l;
			x->l = p;
		}
		//parent update
		x->p = p->p;
		p->p = x;
		if (b) b->p = p;
		(x->p ? p == x->p->l ? x->p->l : x->p->r : root) = x;
	}
	void splay(Node* x) {
		while (x->p) {
			Node* p = x->p;
			Node* g = p->p;
			if (g) {
				if ((x == p->l) == (p == g->l)) rotate(p);//zig-zig
				else rotate(x);//zig-zag
			}
			rotate(x);
		}
	}
public:
	SplayTree() : root(0) {}
	~SplayTree() { if (root) delete root; }
	void clear() {
		if (root) delete root;
		root = 0;
	}
	bool insert(int i) {
		if (!root) {
			root = new Node(i);
			return 0;
		}
		Node* p = root;
		Node** pp;
		while (1) {
			if (p->i == i) return 0;
			if (seg[i] < seg[p->i]) {
				if (!p->l) {
					pp = &p->l;
					break;
				}
				p = p->l;
			}
			else {
				if (!p->r) {
					pp = &p->r;
					break;
				}
				p = p->r;
			}
		}
		Node* x = new Node(i);
		x->p = p;
		*pp = x;
		splay(x);

		if (x->l) {
			Node* l = x->l;
			while (l->r) l = l->r;
			if (intersect(l->i, i)) {
				return 1;
			}
			splay(l);
		}
		splay(x);
		if (x->r) {
			Node* r = x->r;
			while (r->l) r = r->l;
			if (intersect(r->i, i)) {
				return 1;
			}
			splay(r);
		}

		splay(x);
		return 0;
	}
	bool find(int i) {
		if (!root) return 0;
		Node* p = root;
		while (1) {
			if (p->i == i) break;
			if (seg[i] < seg[p->i]) {
				if (!p->l) break;
				p = p->l;
			}
			else {
				if (!p->r) break;
				p = p->r;
			}
		}
		splay(p);
		return p->i == i;
	}
	bool pop(int i) {
		if (!find(i)) return 0;
		Node* p = root;
		if (p->l && p->r) {
			Node* l = p->l;
			Node* r = p->r;
			root = l; root->p = 0;
			while (l->r) l = l->r;
			l->r = r;
			r->p = l;
			while (r->l) r = r->l;
			if (intersect(l->i, r->i)) return 1;
			splay(r);
		}
		else if (p->l) root = p->l, root->p = 0;
		else if (p->r) root = p->r, root->p = 0;
		else root = 0;
		p->l = p->r = 0;
		delete p;
		return 0;
	}
	int top() {
		if (!root) return -1;
		Node* p = root;
		while (p->l) p = p->l;
		splay(p);
		return p->i;
	}
} ST;
inline bool sweep(const int& sz) {
	for (int i = 0; i < sz; i++) {
		if (~pos[i].d) { if (ST.insert(pos[i].i)) { return 1; } }
		else { if (ST.pop(pos[i].i)) { return 1; } }
	}
	return 0;
}
bool two_polygon_cross_check(const Polygon& A, const std::vector<Bound>& B, const int& n1, const int& n2) {
	memset(seg, 0, sizeof seg);
	memset(pos, 0, sizeof pos);
	ai = n1;
	bi = n2;
	int sz1 = A.size();
	for (int i = 0; i < sz1; i++) {
		Pos s = A[i], e = A[(i + 1) % sz1];
		if (e < s) std::swap(s, e);
		seg[i].s = s;
		seg[i].e = e;
		seg[i].h = n1;
		seg[i].i = i;
		s.i = e.i = i;
		s.d = 1, e.d = -1;
		pos[i << 1] = s;
		pos[i << 1 | 1] = e;
	}
	int sz2 = B.size();
	for (int i = 0; i < sz2; i++) {
		Pos s = B[i].s, e = B[i].e;
		if (e < s) std::swap(s, e);
		seg[i + sz1].s = s;
		seg[i + sz1].e = e;
		seg[i + sz1].h = n2;
		s.i = e.i = i + sz1;
		s.d = 1, e.d = -1;
		pos[(i + sz1) << 1] = s;
		pos[(i + sz1) << 1 | 1] = e;
	}
	int sz = (sz1 + sz2) << 1;
	std::sort(pos, pos + sz);
	ST.clear();
	return sweep(sz);
}
void make_bnd(std::vector<Bound>& V, Pos d1, Pos d2, const int& n) {
	assert(d2 != d1);
	if (cmpx(d2, d1)) std::swap(d1, d2);
	V.push_back(Bound(d1, d2, n));
	return;
}
void bnd_init(const Polygon& h1, const Polygon& h2, std::vector<Bound>& V, const int& n1 = -1, const int& n2 = -1) {
	int sz1 = h1.size();
	int sz2 = h2.size();
	for (int i = 0; i < sz1; i++) make_bnd(V, h1[i], h1[(i + 1) % sz1], n1);
	for (int j = 0; j < sz2; j++) make_bnd(V, h2[j], h2[(j + 1) % sz2], n2);
	std::sort(V.begin(), V.end());
	return;
}
void bnd_remove(std::vector<Bound>& V, std::vector<Bound>& V2, bool merge = 1) {
	//ld rmv = 0;
	int sz = V.size();
	for (int i = 0, j; i < sz; i = j) {
		j = i;
		while (j < sz && collinear(V[i], V[j])) j++;
		for (int k = i; k < j - 1; k++) {
			int nxt = k + 1;
			if (V[k].e < V[nxt].s) continue;
			else if (V[k].e == V[nxt].s) {
				if (!merge) continue;
				std::swap(V[k].s, V[nxt].s);
			}
			else if (V[nxt].e < V[k].e) {
				//rmv += (V[nxt].e - V[nxt].s).mag();
				int s = V[k].s.i;
				int e = V[k].e.i;
				std::swap(V[k].e, V[nxt].e);
				std::swap(V[k].e, V[nxt].s);
				V[nxt].i = V[k].i;
				V[k].s.i = V[nxt].s.i = s;
				V[k].e.i = V[nxt].e.i = e;
				V[k].e.d = V[nxt].s.d = 1;
			}
			else if (V[k].e <= V[nxt].e) {
				//rmv += (V[k].e - V[nxt].s).mag();
				std::swap(V[k].e, V[nxt].s);
				std::swap(V[k].e.i, V[nxt].s.i);
				V[k].e.d = V[nxt].s.d = 1;
			}
		}
		for (int k = i; k < j; k++) if (V[k].s != V[k].e) V2.push_back(V[k]);
	}
	//std::sort(V2.begin(), V2.end());
	//return rmv;
	return;
}
bool bnd_check(const Polygon& A, const Polygon& B) {
	std::vector<Bound> V, R;
	int sza = A.size(), szb = B.size();
	for (int i = 0; i < sza; i++) {
		if (A[i] < A[(i + 1) % sza]) V.push_back(Bound(A[i], A[(i + 1) % sza], 0));
		else R.push_back(Bound(A[(i + 1) % sza], A[i], 0));
	}
	for (int i = 0; i < szb; i++) {
		if (B[i] < B[(i + 1) % szb]) R.push_back(Bound(B[i], B[(i + 1) % szb], 1));
		else V.push_back(Bound(B[(i + 1) % szb], B[i], 1));
	}
	std::sort(V.begin(), V.end());
	int sz = V.size();
	for (int i = 0, j; i < sz; i = j) {
		j = i;
		while (j < sz && collinear(V[i], V[j])) j++;
		for (int k = i; k < j - 1; k++) {
			int nxt = k + 1;
			if (V[k].e <= V[nxt].s) continue;
			if (on_seg_weak(V[k].s, V[k].e, V[nxt].s)) return 0;
			if (on_seg_weak(V[k].s, V[k].e, V[nxt].e)) return 0;
			if (on_seg_weak(V[nxt].s, V[nxt].e, V[k].s)) return 0;
			if (on_seg_weak(V[nxt].s, V[nxt].e, V[k].e)) return 0;
		}
	}
	std::sort(R.begin(), R.end());
	sz = R.size();
	for (int i = 0, j; i < sz; i = j) {
		j = i;
		while (j < sz && collinear(R[i], R[j])) j++;
		for (int k = i; k < j - 1; k++) {
			int nxt = k + 1;
			if (R[k].e <= R[nxt].s) continue;
			if (on_seg_weak(R[k].s, R[k].e, R[nxt].s)) return 0;
			if (on_seg_weak(R[k].s, R[k].e, R[nxt].e)) return 0;
			if (on_seg_weak(R[nxt].s, R[nxt].e, R[k].s)) return 0;
			if (on_seg_weak(R[nxt].s, R[nxt].e, R[k].e)) return 0;
		}
	}
	return 1;
}
bool inner_check(const Polygon& B, const Polygon& A) {
	if (!bnd_check(A, B)) {
		//std::cout << "SUCK::\n";
		return 0;
	}
	Polygon tmp;
	for (Pos p : A) p.d = 1, tmp.push_back(p);
	for (Pos p : B) p.d = 0, tmp.push_back(p);
	std::sort(tmp.begin(), tmp.end());
	int sz = tmp.size();
	for (int i = 0; i < sz - 1; i++) {
		if (tmp[i] == tmp[i + 1]) {
			if (!inner_check(B, tmp[i].i, A, tmp[i + 1].i)) {
				//std::cout << "SEX::\n";
				return 0;
			}
		}
	}
	std::vector<Bound> VS, V, VA, VB;
	bnd_init(A, B, VS, 0, 1);
	bnd_remove(VS, V, NO_MERGE);
	sz = V.size();
	for (int i = 0; i < sz; i++) {
		if (V[i].i == 0) VA.push_back(V[i]);
		if (V[i].i == 1) VB.push_back(V[i]);
	}
	if (two_polygon_cross_check(A, VB, 0, 1) ||
		two_polygon_cross_check(B, VA, 1, 0)) {
		//std::cout << "FUCK::\n";
		return 0;
	}
	//if (two_polygon_cross_check(A, VB, 0, 1)) {
	//	//std::cout << "FUCK::\n";
	//	return 0;
	//}
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N; len[0] = N;
	Polygon A(N); for (Pos& p : A) std::cin >> p; norm(A);
	T = 0; for (Pos& p : A) p.i = T, T++, p.d = 0;
	std::cin >> M; len[1] = M;
	Polygon B(M); for (Pos& p : B) std::cin >> p; norm(B);
	T = 0; for (Pos& p : B) p.i = T, T++, p.d = 1;
	Vld R;
	for (int i = 0; i < LEN; i++) mxa[i] = 0, mxb[i] = 0;
	for (int i = 0; i < N; i++) {
		ld x;
		const Pos& a0 = A[i], & a1 = A[(i + 1) % N];
		//const Pos& a0 = A[(i - 1 + N) % N], & a1 = A[i], & a2 = A[(i + 1) % N];
		Seg sa = Seg(O, a1);
		Seg a01 = Seg(a0, a1);
		for (int j = 0; j < M; j++) {
			const Pos& b0 = B[j], & b1 = B[(j + 1) % M];
			//const Pos& b0 = B[(j - 1 + M) % M], & b1 = B[j], & b2 = B[(j + 1) % M];
			//std::cout << "a0:: " << a0 << " ";
			//std::cout << "a1:: " << a1 << " ";
			//std::cout << "b0:: " << b0 << " ";
			//std::cout << "b1:: " << b1 << "\n";
			Seg sb = Seg(O, b1);
			Seg b01 = Seg(b0, b1);
			if (ccw(O, b0, b1) >= 0) {
				x = intersection(sa, b01, WEAK);
				//std::cout << "ax:: " << x << " ";
				if (x > 0) {
					R.push_back(x);
					mxa[i] = std::max(mxa[i], x);
				}
			}
			if (ccw(O, a0, a1) > 0) {
				x = intersection(sb, a01, WEAK);
				//std::cout << "bx:: " << 1 / x << "\n";
				if (x > 0) {
					R.push_back(1 / x);
					mxb[j] = std::max(mxb[j], 1 / x);
				}
			}
		}
	}
	ld limit = INF;
	for (int i = 0; i < N; i++) limit = std::min(limit, mxa[i]);
	for (int i = 0; i < M; i++) limit = std::min(limit, mxb[i]);
	std::sort(R.begin(), R.end());
	R.erase(unique(R.begin(), R.end(), eq), R.end());
	ld ret = 0;
	for (const ld& x : R) {
		if (x > limit) break;
		Polygon NA = A;
		for (Pos& p : NA) p *= x;
		//std::cout << "x:: " << x << "\nNA::\n";
		//for (Pos& p : NA) std::cout << p << "\n";
		//std::cout << "B::\n";
		//for (Pos& p : B) std::cout << p << "\n";
		bool f = inner_check(B, NA);
		//std::cout << "f:: " << f << "\n";
		if (f) ret = std::max(ret, x);
	}
	std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }//boj32567 Hedge Topiary

/*

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <cassert>
#include <vector>
#include <set>
#include <map>
typedef long long ll;
typedef double ld;
//typedef long double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 505;
const ld TOL = 1e-8;
const ld PI = acos(-1);
//const ll MOD = 1'000'000'007;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ll sq(const ll& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
inline ld fit(const ld& x, const ld& lo, const ld& hi) { return std::min(hi, std::max(lo, x)); }
int gcd(int a, int b) { return !b ? a : gcd(b, a % b); }
#define LINE 1
#define CIRCLE 2
#define STRONG 0
#define WEAK 1
#define LO x
#define HI y
#define NO_MERGE 0
#define EDGE 1
#define EDGE_IGNORE 0
//#define AUTO_CHECK
//#define DEBUG
//#define EVENT_COUNT

#ifdef AUTO_CHECK
#include <fstream>
#define WHAT_THE_FUCK
#endif

int ai, bi;
int N, M, K, T, Q;
ld RET = 0;
ld mxa[LEN], mxb[LEN];
struct Pos {
	ld x, y;
	int i, d;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) { i = 0, d = 0; }
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	//bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	//bool operator < (const Pos& r) const { return x == r.x ? y == r.y ? d < r.d : y < r.y : x < r.x; }
	bool operator < (const Pos& r) const { return zero(x - r.x) ? zero(y - r.y) ? d < r.d : y < r.y : x < r.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const ld& n) { x /= n; y /= n; return *this; }
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
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << "(" << p.x << ", " << p.y << ")"; return os; }
} pos[LEN << 3]; const Pos O = Pos(0, 0);
typedef std::set<Pos> SetPos;
typedef std::vector<Pos> Polygon;
int len[2];
bool cmpx(const Pos& p, const Pos& q) { return zero(p.x - q.x) ? p.y < q.y : p.x < q.x; }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ld ret = cross(d1, d2, d3, d4); return sign(ret); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
bool parallel(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3, d4); }
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q, const int& f = STRONG) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) >= f || ccw(p1, p2, q) >= f;
	return ccw(p0, p1, q) >= f && ccw(p1, p2, q) >= f;
}
ld rad(const Pos& p1, const Pos& p2, const Pos& p3) { return rad(p2 - p1, p3 - p2); }
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2, bool weak = WEAK, bool edge = EDGE) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	if (edge == EDGE_IGNORE) return f1 && f2;
	bool f3;
	if (weak == WEAK) f3 = on_seg_weak(s1, s2, d1) ||
		on_seg_weak(s1, s2, d2) ||
		on_seg_weak(d1, d2, s1) ||
		on_seg_weak(d1, d2, s2);
	else f3 = on_seg_strong(s1, s2, d1) ||
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
bool norm(Polygon& H) {
	ld a = area(H);
	if (zero(a)) return 1;
	if (a < 0) std::reverse(H.begin(), H.end());
	return 0;
}
int inner_check(const std::vector<Pos>& H, const Pos& p) {//concave
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (cur.y == nxt.y) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		//if (nxt.y <= p.y || cur.y > p.y) continue;
		if (nxt.y < p.y + TOL || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}
bool inner_check(const Polygon& B, const int& b, const Polygon& A, const int& a) {
	int sza = A.size();
	int szb = B.size();
	const Pos& a0 = A[(a - 1 + sza) % sza], & a1 = A[a], & a2 = A[(a + 1) % sza];
	const Pos& b0 = B[(b - 1 + szb) % szb], & b1 = B[b], & b2 = B[(b + 1) % szb];
	return inside(b0, b1, b2, a0)
		&& inside(b0, b1, b2, a2)
		&& !inside(a0, a1, a2, b0, WEAK)
		&& !inside(a0, a1, a2, b2, WEAK);
}
//bool inner_check(const Pos& a0, const Pos& a1, const Pos& a2, const Pos& b0, const Pos& b1, const Pos& b2) {
//	int CCW;
//	CCW = ccw(a0, a1, a2);
//	if (CCW > 0 && ccw(a1, a0, b0) < 0 && ccw(a1, a2, b0) > 0) return 1;
//	if (CCW < 0 && !(ccw(a1, a0, b0) >= 0 && ccw(a1, a2, b0) <= 0)) return 1;
//	if (CCW > 0 && ccw(a1, a0, b2) < 0 && ccw(a1, a2, b2) > 0) return 1;
//	if (CCW < 0 && !(ccw(a1, a0, b2) >= 0 && ccw(a1, a2, b2) <= 0)) return 1;
//	CCW = ccw(b0, b1, b2);
//	if (CCW > 0 && ccw(b1, b0, a0) < 0 && ccw(b1, b2, a0) > 0) return 1;
//	if (CCW < 0 && !(ccw(b1, b0, a0) >= 0 && ccw(b1, b2, a0) <= 0)) return 1;
//	if (CCW > 0 && ccw(b1, b0, a2) < 0 && ccw(b1, b2, a2) > 0) return 1;
//	if (CCW < 0 && !(ccw(b1, b0, a2) >= 0 && ccw(b1, b2, a2) <= 0)) return 1;
//	return 0;
//}
//Polygon rotate_and_norm(Polygon B, const int& j0, const Polygon& A, const int& i0, const ld& t, Pos& v) {
//	int sz = B.size();
//	for (int j = 0; j < sz; j++) B[j] = B[j].rot(t);
//	v = A[i0] - B[j0];
//	for (int j = 0; j < sz; j++) B[j] += v;
//	return B;
//}
struct Seg {
	Pos s, e;
	int h, i;
	Seg(Pos s_ = Pos(0, 0), Pos e_ = Pos(0, 0)) : s(s_), e(e_) { h = 0; i = 0; }
	bool operator == (const Seg& S) const { return s == S.s && e == S.e; }
	bool operator != (const Seg& S) const { return !(*this == S); }
	//bool operator < (const Seg& S) const { return (s == S.s) ? e < S.e : s < S.s; }
	bool operator < (const Seg& rhs) const {
		if (rhs.s == s) return ccw(rhs.s, rhs.e, e) < 0;
		if (rhs.s < s) return ccw(rhs.s, rhs.e, s) < 0;
		return ccw(rhs.s, rhs.e, e) < 0;
	}
	Pos p(const ld& rt) const { return s + (e - s) * rt; }
	ld green(const ld& lo = 0, const ld& hi = 1) const {
		ld d = hi - lo;
		ld ratio = (lo + hi) * .5;
		Pos m = p(ratio);
		return m.y * d * (s.x - e.x);
	}
	Seg operator - (const ld& n) const {
		Pos v = ~(e - s).unit();
		return { s + v * n, e + v * n };
	}
	Seg& operator -= (const ld& n) {
		Pos v = ~(e - s).unit();
		s += v * n; e += v * n;
		return *this;
	}
	ld mag() const { return (e - s).mag(); }
	friend std::ostream& operator << (std::ostream& os, const Seg& S) { os << "DEBUG::Seg s: " << S.s << " | e: " << S.e << " DEBUG::Seg\n"; return os; }
} seg[LEN << 3];
bool intersect(const Seg& u, const Seg& v) { return intersect(u.s, u.e, v.s, v.e); }
ld intersection(const Seg& s1, const Seg& s2, const bool& f = STRONG) {
	const Pos& p1 = s1.s, p2 = s1.e, q1 = s2.s, q2 = s2.e;
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	ld a2 = ((p2 - p1) / (p1 - q1)) / -det;
	if (f == WEAK) {
		if (ccw(s1.s, s1.e, s2.s) * ccw(s1.s, s1.e, s2.e) > 0) return -1;
		return a1;
	}
	if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return -1;
}
//Vld intersections(const Seg& l, const Polygon& H) {
//	int sz = H.size();
//	Vld ret;
//	const Pos& s = l.s, & e = l.e;
//	for (int i = 0; i < sz; i++) {
//		const Pos& p0 = H[i], & p1 = H[(i + 1) % sz];
//		if (parallel(s, e, p0, p1)) continue;
//		if (ccw(s, e, p0) * ccw(s, e, p1) <= 0) {
//			ld x = intersection(Seg(s, e), Seg(p0, p1));
//			ret.push_back(x);
//		}
//	}
//	std::sort(ret.begin(), ret.end());
//	ret.erase(unique(ret.begin(), ret.end(), eq), ret.end());
//	return ret;
//}
struct Bound {
	Pos s, e;
	int i;
	Bound(Pos S = Pos(0, 0), Pos E = Pos(0, 0), int I = -1) : s(S), e(E), i(I) {}
	bool operator == (const Bound& S) const { return s == S.s && e == S.e; }
	bool operator != (const Bound& S) const { return !(*this == S); }
	bool operator < (const Bound& S) const {
		ld tq = cross(s, e, S.s, S.e);
		if (zero(tq)) {
			if (collinear(s, e, S.s, S.e)) {
				if (s == S.s) return cmpx(e, S.e);
				return cmpx(s, S.s);
			}
			return ccw(s, e, S.s) > 0;
		}
		return tq > 0;
	}
	friend std::ostream& operator << (std::ostream& os, const Bound& S) {
		os << "DEBUG::Seg:: s: " << S.s << " | e: " << S.e << " DEBUG::Seg\n";
		return os;
	}
};
bool collinear(const Bound& a, const Bound& b) { return collinear(a.s, a.e, b.s, b.e); }
bool intersect(const int& a, const int& b) {
	Seg A = seg[a], B = seg[b];
	if (A.h == B.h) return 0;
	if (!intersect(A.s, A.e, B.s, B.e, STRONG)) return 0;
	if (intersect(A.s, A.e, B.s, B.e, WEAK, EDGE_IGNORE)) return 1;
	if (A.h != ai) std::swap(A, B);
	if (A.e.i != (A.s.i + 1) % len[A.h]) std::swap(A.s, A.e);
	assert((A.s.i + 1) % len[A.h] == A.e.i);
	if (ai == 0) {
		if (on_seg_weak(A.s, A.e, B.s)) return ccw(A.s, A.e, B.e) > 0;
		if (on_seg_weak(A.s, A.e, B.e)) return ccw(A.s, A.e, B.s) > 0;
	}
	else {
		if (on_seg_weak(A.s, A.e, B.s)) return ccw(A.s, A.e, B.e) < 0;
		if (on_seg_weak(A.s, A.e, B.e)) return ccw(A.s, A.e, B.s) < 0;
	}
	return 0;
}
//struct Event {
//	ld t;
//	Pos v;
//	Event(ld t_ = 0, Pos v_ = Pos()) : t(t_), v(v_) {}
//	bool operator < (const Event& e) const { return zero(t - e.t) ? v < e.v : t < e.t; }
//	bool operator == (const Event& e) const { return zero(t - e.t) && v == e.v; }
//};
//std::vector<Event> events;
//inline void conv(const Polygon& B, const Event& E, Polygon& B2) {
//	for (const Pos& p : B) B2.push_back(p.rot(E.t) + E.v);
//	T = 0; for (Pos& p : B2) p.i = T, T++, p.d = 0;
//	return;
//}
//#define __WHAT_THE__ !
//#define __FUCK_YOU__ 1
//bool ST_DEBUG = __WHAT_THE__ __FUCK_YOU__;
class SplayTree {
	struct Node {
		int i;
		Node* l;
		Node* r;
		Node* p;
		Node(int i) : i(i), l(0), r(0), p(0) {}
		~Node() { if (l) delete l; if (r) delete r; }
	} *root;

	void rotate(Node* x) {
		Node* p = x->p;     //p : parent of x
		if (!p) return;     //if x == root: return
		Node* b = 0;
		if (x == p->l) {    //if x is left child
			p->l = b = x->r;
			x->r = p;
		}
		else {              //else if x is right child
			p->r = b = x->l;
			x->l = p;
		}
		//parent update
		x->p = p->p;
		p->p = x;
		if (b) b->p = p;
		(x->p ? p == x->p->l ? x->p->l : x->p->r : root) = x;
	}
	void splay(Node* x) {
		while (x->p) {
			Node* p = x->p;
			Node* g = p->p;
			if (g) {
				if ((x == p->l) == (p == g->l)) rotate(p);//zig-zig
				else rotate(x);//zig-zag
			}
			rotate(x);
		}
	}
public:
	SplayTree() : root(0) {}
	~SplayTree() { if (root) delete root; }
	void clear() {
		if (root) delete root;
		root = 0;
	}
	bool insert(int i) {
		if (!root) {
			root = new Node(i);
			return 0;
		}
		Node* p = root;
		Node** pp;
		while (1) {
			if (p->i == i) return 0;
			if (seg[i] < seg[p->i]) {
				if (!p->l) {
					pp = &p->l;
					break;
				}
				p = p->l;
			}
			else {
				if (!p->r) {
					pp = &p->r;
					break;
				}
				p = p->r;
			}
		}
		Node* x = new Node(i);
		x->p = p;
		*pp = x;
		splay(x);

		if (x->l) {
			Node* l = x->l;
			while (l->r) l = l->r;
			if (intersect(l->i, i)) {
				return 1;
			}
			splay(l);
		}
		splay(x);
		if (x->r) {
			Node* r = x->r;
			while (r->l) r = r->l;
			if (intersect(r->i, i)) {
				return 1;
			}
			splay(r);
		}

		splay(x);
		return 0;
	}
	bool find(int i) {
		if (!root) return 0;
		Node* p = root;
		while (1) {
			if (p->i == i) break;
			if (seg[i] < seg[p->i]) {
				if (!p->l) break;
				p = p->l;
			}
			else {
				if (!p->r) break;
				p = p->r;
			}
		}
		splay(p);
		return p->i == i;
	}
	bool pop(int i) {
		if (!find(i)) return 0;
		Node* p = root;
		if (p->l && p->r) {
			Node* l = p->l;
			Node* r = p->r;
			root = l; root->p = 0;
			while (l->r) l = l->r;
			l->r = r;
			r->p = l;
			while (r->l) r = r->l;
			if (intersect(l->i, r->i)) return 1;
			splay(r);
		}
		else if (p->l) root = p->l, root->p = 0;
		else if (p->r) root = p->r, root->p = 0;
		else root = 0;
		p->l = p->r = 0;
		delete p;
		return 0;
	}
	int top() {
		if (!root) return -1;
		Node* p = root;
		while (p->l) p = p->l;
		splay(p);
		return p->i;
	}
} ST;
inline bool sweep(const int& sz) {
	for (int i = 0; i < sz; i++) {
		if (~pos[i].d) { if (ST.insert(pos[i].i)) { return 1; } }
		else { if (ST.pop(pos[i].i)) { return 1; } }
	}
	return 0;
}
bool two_polygon_cross_check(const Polygon& A, const std::vector<Bound>& B, const int& n1, const int& n2) {
	memset(seg, 0, sizeof seg);
	memset(pos, 0, sizeof pos);
	ai = n1;
	bi = n2;
	int sz1 = A.size();
	for (int i = 0; i < sz1; i++) {
		Pos s = A[i], e = A[(i + 1) % sz1];
		if (e < s) std::swap(s, e);
		seg[i].s = s;
		seg[i].e = e;
		seg[i].h = n1;
		seg[i].i = i;
		s.i = e.i = i;
		s.d = 1, e.d = -1;
		pos[i << 1] = s;
		pos[i << 1 | 1] = e;
	}
	int sz2 = B.size();
	for (int i = 0; i < sz2; i++) {
		Pos s = B[i].s, e = B[i].e;
		if (e < s) std::swap(s, e);
		seg[i + sz1].s = s;
		seg[i + sz1].e = e;
		seg[i + sz1].h = n2;
		s.i = e.i = i + sz1;
		s.d = 1, e.d = -1;
		pos[(i + sz1) << 1] = s;
		pos[(i + sz1) << 1 | 1] = e;
	}
	int sz = (sz1 + sz2) << 1;
	std::sort(pos, pos + sz);
	ST.clear();
	return sweep(sz);
}
void make_bnd(std::vector<Bound>& V, Pos d1, Pos d2, const int& n) {
	assert(d2 != d1);
	if (cmpx(d2, d1)) std::swap(d1, d2);
	V.push_back(Bound(d1, d2, n));
	return;
}
void bnd_init(const Polygon& h1, const Polygon& h2, std::vector<Bound>& V, const int& n1 = -1, const int& n2 = -1) {
	int sz1 = h1.size();
	int sz2 = h2.size();
	for (int i = 0; i < sz1; i++) make_bnd(V, h1[i], h1[(i + 1) % sz1], n1);
	for (int j = 0; j < sz2; j++) make_bnd(V, h2[j], h2[(j + 1) % sz2], n2);
	std::sort(V.begin(), V.end());
	return;
}
void bnd_remove(std::vector<Bound>& V, std::vector<Bound>& V2, bool merge = 1) {
	//ld rmv = 0;
	int sz = V.size();
	for (int i = 0, j; i < sz; i = j) {
		j = i;
		while (j < sz && collinear(V[i], V[j])) j++;
		for (int k = i; k < j - 1; k++) {
			int nxt = k + 1;
			if (V[k].e < V[nxt].s) continue;
			else if (V[k].e == V[nxt].s) {
				if (!merge) continue;
				std::swap(V[k].s, V[nxt].s);
			}
			else if (V[nxt].e < V[k].e) {
				//rmv += (V[nxt].e - V[nxt].s).mag();
				int s = V[k].s.i;
				int e = V[k].e.i;
				std::swap(V[k].e, V[nxt].e);
				std::swap(V[k].e, V[nxt].s);
				V[nxt].i = V[k].i;
				V[k].s.i = V[nxt].s.i = s;
				V[k].e.i = V[nxt].e.i = e;
				V[k].e.d = V[nxt].s.d = 1;
			}
			else if (V[k].e <= V[nxt].e) {
				//rmv += (V[k].e - V[nxt].s).mag();
				std::swap(V[k].e, V[nxt].s);
				std::swap(V[k].e.i, V[nxt].s.i);
				V[k].e.d = V[nxt].s.d = 1;
			}
		}
		for (int k = i; k < j; k++) if (V[k].s != V[k].e) V2.push_back(V[k]);
	}
	//std::sort(V2.begin(), V2.end());
	//return rmv;
	return;
}
bool bnd_check(const Polygon& A, const Polygon& B) {
	std::vector<Bound> V, R;
	int sza = A.size(), szb = B.size();
	for (int i = 0; i < sza; i++) {
		if (A[i] < A[(i + 1) % sza]) V.push_back(Bound(A[i], A[(i + 1) % sza], 0));
		else R.push_back(Bound(A[(i + 1) % sza], A[i], 0));
	}
	for (int i = 0; i < szb; i++) {
		if (B[i] < B[(i + 1) % szb]) R.push_back(Bound(B[i], B[(i + 1) % szb], 1));
		else V.push_back(Bound(B[(i + 1) % szb], B[i], 1));
	}
	std::sort(V.begin(), V.end());
	int sz = V.size();
	for (int i = 0, j; i < sz; i = j) {
		j = i;
		while (j < sz && collinear(V[i], V[j])) j++;
		for (int k = i; k < j - 1; k++) {
			int nxt = k + 1;
			if (V[k].e <= V[nxt].s) continue;
			if (on_seg_weak(V[k].s, V[k].e, V[nxt].s)) return 0;
			if (on_seg_weak(V[k].s, V[k].e, V[nxt].e)) return 0;
			if (on_seg_weak(V[nxt].s, V[nxt].e, V[k].s)) return 0;
			if (on_seg_weak(V[nxt].s, V[nxt].e, V[k].e)) return 0;
		}
	}
	std::sort(R.begin(), R.end());
	sz = R.size();
	for (int i = 0, j; i < sz; i = j) {
		j = i;
		while (j < sz && collinear(R[i], R[j])) j++;
		for (int k = i; k < j - 1; k++) {
			int nxt = k + 1;
			if (R[k].e <= R[nxt].s) continue;
			if (on_seg_weak(R[k].s, R[k].e, R[nxt].s)) return 0;
			if (on_seg_weak(R[k].s, R[k].e, R[nxt].e)) return 0;
			if (on_seg_weak(R[nxt].s, R[nxt].e, R[k].s)) return 0;
			if (on_seg_weak(R[nxt].s, R[nxt].e, R[k].e)) return 0;
		}
	}
	return 1;
}
bool inner_check(const Polygon& B, const Polygon& A) {
	if (!bnd_check(A, B)) {
		//std::cout << "SUCK::\n";
		return 0;
	}
	Polygon tmp;
	for (Pos p : A) p.d = 1, tmp.push_back(p);
	for (Pos p : B) p.d = 0, tmp.push_back(p);
	std::sort(tmp.begin(), tmp.end());
	int sz = tmp.size();
	for (int i = 0; i < sz - 1; i++) {
		if (tmp[i] == tmp[i + 1]) {
			if (!inner_check(B, tmp[i].i, A, tmp[i + 1].i)) {
				//std::cout << "SEX::\n";
				return 0;
			}
		}
	}
	std::vector<Bound> VS, V, VA, VB;
	bnd_init(A, B, VS, 0, 1);
	bnd_remove(VS, V, NO_MERGE);
	sz = V.size();
	for (int i = 0; i < sz; i++) {
		if (V[i].i == 0) VA.push_back(V[i]);
		if (V[i].i == 1) VB.push_back(V[i]);
	}
	if (two_polygon_cross_check(A, VB, 0, 1) ||
		two_polygon_cross_check(B, VA, 1, 0)) {
		//std::cout << "FUCK::\n";
		return 0;
	}
	//if (two_polygon_cross_check(A, VB, 0, 1)) {
	//	//std::cout << "FUCK::\n";
	//	return 0;
	//}
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N; len[0] = N;
	Polygon A(N); for (Pos& p : A) std::cin >> p; norm(A);
	T = 0; for (Pos& p : A) p.i = T, T++, p.d = 0;
	std::cin >> M; len[1] = M;
	Polygon B(M); for (Pos& p : B) std::cin >> p; norm(B);
	T = 0; for (Pos& p : B) p.i = T, T++, p.d = 1;
	Vld R;
	for (int i = 0; i < LEN; i++) mxa[i] = 0, mxb[i] = 0;
	for (int i = 0; i < N; i++) {
		ld x;
		const Pos& a0 = A[i], & a1 = A[(i + 1) % N];
		//const Pos& a0 = A[(i - 1 + N) % N], & a1 = A[i], & a2 = A[(i + 1) % N];
		Seg sa = Seg(O, a1);
		Seg a01 = Seg(a0, a1);
		for (int j = 0; j < M; j++) {
			const Pos& b0 = B[j], & b1 = B[(j + 1) % M];

			Seg sb = Seg(O, b1);
			Seg b01 = Seg(b0, b1);
			if (ccw(O, b0, b1) >= 0) {
				x = intersection(sa, b01, WEAK);
				if (x > 0) {
					R.push_back(x);
					mxa[i] = std::max(mxa[i], x);
				}
			}
			if (ccw(O, a0, a1) < 0) {
				x = intersection(sb, a01, WEAK);
				if (x > 0) {
					R.push_back(1 / x);
					mxb[j] = std::max(mxb[j], 1 / x);
				}
			}
		}
	}
	return;
}
int main() { solve(); return 0; }//boj32567 Hedge Topiary


*/