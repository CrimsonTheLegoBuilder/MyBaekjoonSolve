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
//typedef double ld;
typedef long double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const ll INF = 1e17;		  
const int LEN = 105;
const ld TOL = 1e-15;
//const ll MOD = 1'000'000'007;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
int gcd(int a, int b) { return !b ? a : gcd(b, a % b); }

//#define AUTO_CHECK
//#define DEBUG

#define STRONG 1
#define WEAK 0
#define NO_MERGE 0

#ifdef AUTO_CHECK
#include <fstream>
#define WHAT_THE_FUCK
#endif

bool INNER_CHECK;
bool EDGE_IGNORE;
int ai, bi;

int N, M, K, T, Q;
ld RET = 0;
bool F[LEN];
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
int len[10];
bool cmpx(const Pos& p, const Pos& q) { return zero(p.x - q.x) ? p.y < q.y : p.x < q.x; }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return sign(ret); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return sign(ret); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2, bool weak = WEAK) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	if (EDGE_IGNORE) return f1 && f2;
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
Polygon graham_scan(Polygon& C) {
	Polygon H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
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
Polygon rotate_and_norm(Polygon B, const int& j0, const Polygon& A, const int& i0, const ld& t) {
	int sz = B.size();
	for (int j = 0; j < sz; j++) B[j] = B[j].rot(t);
	Pos v = A[i0] - B[j0];
	for (int j = 0; j < sz; j++) B[j] += v;
	return B;
}
struct Seg {
	Pos s, e;
	int h, i;
	Seg(Pos S = Pos(0, 0), Pos E = Pos(0, 0)) : s(S), e(E) { h = 0; i = 0; }
	bool operator == (const Seg& S) const { return s == S.s && e == S.e; }
	bool operator != (const Seg& S) const { return !(*this == S); }
	//bool operator < (const Seg& S) const { return (s == S.s) ? e < S.e : s < S.s; }
	bool operator < (const Seg& rhs) const {
		//if (rhs.s == s) return cross(rhs.s, rhs.e, e) < 0;
		//if (rhs.s < s) return cross(rhs.s, rhs.e, s) < 0;
		//return cross(rhs.s, rhs.e, e) < 0;
		if (rhs.s == s) return ccw(rhs.s, rhs.e, e) < 0;
		if (rhs.s < s) return ccw(rhs.s, rhs.e, s) < 0;
		return ccw(rhs.s, rhs.e, e) < 0;
	}
	friend std::ostream& operator << (std::ostream& os, const Seg& S) { os << "DEBUG::Seg s: " << S.s << " | e: " << S.e << " DEBUG::Seg\n"; return os; }
} seg[LEN << 3];
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
				if (s == S.s) return e < S.e;
				return s < S.s;
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
	if (INNER_CHECK) {
		Seg A = seg[a], B = seg[b];
		if (A.h == B.h) return 0;
		if (!intersect(A.s, A.e, B.s, B.e)) return 0;
		if (A.h != ai) std::swap(A, B);
		if (A.e.i != (A.s.i + 1) % len[A.h]) std::swap(A.s, A.e);
		assert((A.s.i + 1) % len[A.h] == A.e.i);
		if (B.e.i != (B.s.i + 1) % len[B.h]) std::swap(B.s, B.e);
		assert((B.s.i + 1) % len[B.h] == B.e.i);
		if (on_seg_weak(B.s, B.e, A.s)) return ccw(B.s, B.e, A.e) > 0;
		if (on_seg_weak(B.s, B.e, A.e)) return ccw(B.s, B.e, A.s) > 0;
		if (on_seg_weak(A.s, A.e, B.s)) return ccw(A.s, A.e, B.e) > 0;
		if (on_seg_weak(A.s, A.e, B.e)) return ccw(A.s, A.e, B.s) > 0;
		return 0;
	}
	const Pos& d1 = seg[a].s, & d2 = seg[a].e, & d3 = seg[b].s, & d4 = seg[b].e;
	return intersect(d1, d2, d3, d4, WEAK);
}
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
			if (intersect(l->i, i)) return 1;
			splay(l);
		}
		splay(x);
		if (x->r) {
			Node* r = x->r;
			while (r->l) r = r->l;
			if (intersect(r->i, i)) return 1;
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
//void mirror(Polygon& H) { for (Pos& p : H) p.x *= -1; }
inline bool sweep(const int& sz) {
	for (int i = 0; i < sz; i++) {
		if (~pos[i].d) { if (ST.insert(pos[i].i)) { return 1; } }
		else { if (ST.pop(pos[i].i)) { return 1; } }
	}
	return 0;
}
bool two_polygon_cross_check(const Polygon& H1, const Polygon& H2, const int& n1, const int& n2) {
	memset(seg, 0, sizeof seg);
	memset(pos, 0, sizeof pos);
	int sz1 = H1.size();
	for (int i = 0; i < sz1; i++) {
		Pos s = H1[i], e = H1[(i + 1) % sz1];
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
	int sz2 = H2.size();
	for (int i = 0; i < sz2; i++) {
		Pos s = H2[i], e = H2[(i + 1) % sz2];
		if (e < s) std::swap(s, e);
		seg[i + sz1].s = s;
		seg[i + sz1].e = e;
		seg[i + sz1].h = n2;
		seg[i + sz1].i = i;
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
bool two_polygon_cross_check(const Polygon& H, const std::vector<Bound>& B, const int& n1, const int& n2) {
	memset(seg, 0, sizeof seg);
	memset(pos, 0, sizeof pos);
	ai = n1;
	bi = n2;
	int sz1 = H.size();
	for (int i = 0; i < sz1; i++) {
		Pos s = H[i], e = H[(i + 1) % sz1];
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
void make_bnd(std::vector<Bound>& V, const Pos& x1, const Pos& x2, const int& n) {
	Pos d1 = x1, d2 = x2;
	assert(d2 != d1);
	if (d2 < d1) std::swap(d1, d2);
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
ld bnd_remove(std::vector<Bound>& V, std::vector<Bound>& V2, bool merge = 1) {
	ld rmv = 0;
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
				rmv += (V[nxt].e - V[nxt].s).mag();
				int s = V[k].s.i;
				int e = V[k].e.i;
				std::swap(V[k].e, V[nxt].e);
				std::swap(V[k].e, V[nxt].s);
				V[nxt].i = V[k].i;
				V[k].s.i = V[nxt].s.i = s;
				V[k].e.i = V[nxt].e.i = e;
			}
			else if (V[k].e <= V[nxt].e) {
				rmv += (V[k].e - V[nxt].s).mag();
				std::swap(V[k].e, V[nxt].s);
				std::swap(V[k].e.i, V[nxt].s.i);
			}
		}
		for (int k = i; k < j; k++) if (V[k].s != V[k].e) V2.push_back(V[k]);
	}
	//std::sort(V2.begin(), V2.end());
	return rmv;
}
bool inner_check(const Polygon& A, const int& a, const Polygon& B, const int& b) {
	int sza = A.size();
	int szb = B.size();
	int CCW;

	const Pos& a0 = A[(a - 1 + sza) % sza], & a1 = A[a], & a2 = A[(a + 1) % sza];
	const Pos& b0 = B[(b - 1 + szb) % szb], & b1 = B[b], & b2 = B[(b + 1) % szb];

	CCW = ccw(a0, a1, a2);
	if (CCW > 0 && ccw(a1, a0, b0) < 0 && ccw(a1, a2, b0) > 0) return 1;
	if (CCW < 0 && !(ccw(a1, a0, b0) >= 0 && ccw(a1, a2, b0) <= 0)) return 1;
	if (CCW > 0 && ccw(a1, a0, b2) < 0 && ccw(a1, a2, b2) > 0) return 1;
	if (CCW < 0 && !(ccw(a1, a0, b2) >= 0 && ccw(a1, a2, b2) <= 0)) return 1;

	CCW = ccw(b0, b1, b2);
	if (CCW > 0 && ccw(b1, b0, a0) < 0 && ccw(b1, b2, a0) > 0) return 1;
	if (CCW < 0 && !(ccw(b1, b0, a0) >= 0 && ccw(b1, b2, a0) <= 0)) return 1;
	if (CCW > 0 && ccw(b1, b0, a2) < 0 && ccw(b1, b2, a2) > 0) return 1;
	if (CCW < 0 && !(ccw(b1, b0, a2) >= 0 && ccw(b1, b2, a2) <= 0)) return 1;

	return 0;
}
void cross_check(const Polygon& H, const Pos& p, const Pos& q, Polygon& inx) {
	bool fp = 1, fq = 1;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		const Pos& cur = H[i], & nxt = H[(i + 1) % sz];
		if (intersect(cur, nxt, p, q, STRONG)) {
			if (!collinear(cur, nxt, p, q)) inx.push_back(intersection(cur, nxt, p, q));
			else {
				if (on_seg_strong(p, q, cur)) inx.push_back(cur);
				if (on_seg_strong(p, q, nxt)) inx.push_back(nxt);
				if (on_seg_strong(cur, nxt, p))inx.push_back(p);
				if (on_seg_strong(cur, nxt, q))inx.push_back(q);
			}
		}
		if (ccw(cur, nxt, p) < 0) fp = 0;
		if (ccw(cur, nxt, q) < 0) fq = 0;
	}
	if (fp) inx.push_back(p);
	if (fq) inx.push_back(q);
	std::sort(inx.begin(), inx.end(), cmpx);
	inx.erase(unique(inx.begin(), inx.end()), inx.end());
	return;
}
ld polygon_cross_check(const Polygon& A, const Polygon& B) {
	ld ret = -1.;
	INNER_CHECK = 0;
	EDGE_IGNORE = 0;
	ai = bi = -1;
	len[0] = N;
	len[1] = M;

	EDGE_IGNORE = 1;
	if (two_polygon_cross_check(A, B, 0, 1)) {
#ifdef DEBUG
		std::cout << "FUCK:: 2::\n";
#endif
		return -1.;
	}
	EDGE_IGNORE = 0;

	SetPos SB;
	for (int b = 0; b < M; b++) SB.insert(B[b]);
	for (int a = 0; a < N; a++) {
		auto p = SB.find(A[a]);
		if (p != SB.end()) {
			int b = p->i;
			if (inner_check(A, a, B, b)) {
#ifdef DEBUG
				std::cout << "FUCK:: 1::\n";
#endif
				return -1.;
			}
		}
	}

	std::vector<Bound> VS, V, VA, VB;
	std::vector<Bound>().swap(VS);
	std::vector<Bound>().swap(V);
	bnd_init(A, B, VS, 0, 1);
	ret = bnd_remove(VS, V, NO_MERGE);
	if (ret < RET) return RET;

	INNER_CHECK = 1;
	int sz = V.size();
	for (int i = 0; i < sz; i++) if (V[i].i == 1) VB.push_back(V[i]);
	if (two_polygon_cross_check(A, VB, 0, 1)) {
#ifdef DEBUG
		std::cout << "FUCK:: 3::\n";
#endif
		return -1.;
	}
	INNER_CHECK = 0;

	//if (inner_check(A, B[0]) == 2 || inner_check(B, A[0]) == 2) return -1.;

#ifdef DEBUG
	std::cout << "WHAT THE FUCK::\n";
#endif
	return ret;
}
ld sweep(const Polygon& A, const Polygon& B, const Pos& v) {
	ld ret = 0;
	bool prl = 0;
	Vld V;
	Polygon inx;
	for (int j = 0; j < M; j++) {
		prl = 0;
		Pos J0 = B[j], J1 = B[(j + 1) % M];
		ld tq = v / (J0 - J1);
		ld h = std::abs(tq / v.mag());
		if (zero(tq)) prl = 1;
		Polygon box = { J0, J1, J0 + v, J1 + v };
		box = graham_scan(box);
		for (int i = 0; i < N; i++) {
			inx.clear();
			Pos I0 = A[i], I1 = A[(i + 1) % N];
			if (prl && collinear(I0, I1, J0, J1)) {
				if (sign(dot(I0, I1, J0, J1)) > 0) continue;
				if (v * (J0 - J1) < 0) std::swap(J0, J1);
				if (v * (I0 - I1) > 0) std::swap(I0, I1);
				Pos J2 = J1 + v;
				Pos vec = J0 - J1;
				if (on_seg_strong(J1, J2, I0)) inx.push_back(I0);
				//if (on_seg_strong(J1, J2, I0 - vec)) inx.push_back(I0 - vec);
				//if (on_seg_strong(J1, J2, I1)) inx.push_back(I1);
				if (on_seg_strong(J1, J2, I1 - vec)) inx.push_back(I1 - vec);
				for (const Pos& p : inx) V.push_back(std::abs((p - J1).mag()));
			}
			else if (!prl) {
				cross_check(box, I0, I1, inx);
				for (const Pos& p : inx) V.push_back(std::abs(cross(J0, J1, p) / h));
			}
		}
	}
	std::sort(V.begin(), V.end());
	V.erase(unique(V.begin(), V.end()), V.end());
	int sz = V.size();
#ifdef DEBUG
	std::cout << "FUCK::\n";
	std::cout << "sz:: " << sz << "\n";
#endif
	for (const ld& d : V) {
		Pos vec = v.unit() * d;
		Polygon B2;
		for (const Pos& p : B) B2.push_back(p + vec);
		T = 0; for (Pos& p : B2) p.i = T, T++;
		ret = std::max(ret, polygon_cross_check(A, B2));
	}
	return ret;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);

	std::cin >> N; Polygon A(N);
	for (Pos& p : A) std::cin >> p;
	norm(A);
	T = 0; for (Pos& p : A) p.i = T, T++;

	std::cin >> M; Polygon B(M);
	for (Pos& p : B) std::cin >> p;
	norm(B);

	RET = 0;
	for (int i = 0; i < N; i++) {
		Pos& I0 = A[i], & I1 = A[(i + 1) % N];
		for (int j = 0; j < M; j++) {
			Pos& J0 = B[j], & J1 = B[(j + 1) % M];
			ld t = rad(J0 - J1, I1 - I0);
			Polygon B2 = rotate_and_norm(B, j, A, i, t);
			RET = std::max(RET, sweep(A, B2, A[(i + 1) % N] - B2[(j + 1) % M]));
#ifdef DEBUG
			std::cout << "FUCK::\n";
			std::cout << "theta:: " << t << "\n";
			std::cout << "A = [ \n";
			for (Pos& p : A) std::cout << p << "\n";
			std::cout << "]\n";
			std::cout << "B2 = [ \n";
			for (Pos& p : B2) std::cout << p << "\n";
			std::cout << "]\n";
			std::cout << "ret:: " << ret << "\n";
#endif
		}
	}
	std::cout << RET << "\n";
	return;
}
int main() { solve(); return 0; }//boj12772 Polygonal Puzzle

/*

8
0 0
0 10
10 10
15 15
24 6
24 10
30 10
30 0
7
-5 0
-5 10
10 10
15 5
20 10
35 10
35 0

30.142135624



3
1 0
0 30
40 0
3
1 0
0 30
40 0

50

*/