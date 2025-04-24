#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <cassert>
#include <vector>
#include <unordered_set>
typedef long long ll;
typedef double ld;
typedef std::pair<int, int> pi;
const ll INF = 1e17;
const int LEN = 1e4 + 5;
const ld TOL = 1e-7;
//const ll MOD = 1'000'000'007;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
int gcd(int a, int b) { return !b ? a : gcd(b, a % b); }

//#define AUTO_CHECK

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
bool F[LEN];
struct Pos {
	int x, y;
	int i, d;
	Pos(int X = 0, int Y = 0, int I = 0, int D = 0) : x(X), y(Y), i(I), d(D) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	//bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator < (const Pos& r) const { return x == r.x ? y == r.y ? d < r.d : y < r.y : x < r.x; }
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
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
	//friend std::ostream& operator << (std::ostream& os, const Pos& p) {
	//	os << p.x << " " << p.y;
	//	os << " ::DEBUG:: " << p.i  << " " << p.d;
	//	return os;
	//}
} pos[LEN << 3]; const Pos O = Pos(0, 0);
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
struct PosHash { std::size_t operator() (const Pos& p) const { return (((ll)p.x + 100000) << 32) | ((ll)p.y + 100000); } };
typedef std::unordered_set<Pos, PosHash> SetPos;
typedef std::vector<Pos> Polygon;
int len[4];
Polygon H[4];
ll hash(const Pos& p) { return (((ll)p.x + 100000) << 32) | ((ll)p.y + 100000); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return !ret ? 0 : ret > 0 ? 1 : -1; }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
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
ll area(const Polygon& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
bool norm(Polygon& H) {
	ll a = area(H);
	if (!a) return 1;
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
		if (nxt.y <= p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}
struct Line {//ax + by = c
	Pos s;
	ll c;
	Line(Pos V = Pos(0, 0), ll C = 0) : s(V), c(C) {}
	bool operator == (const Line& l) const { return s == l.s && c == l.c; }
	bool operator < (const Line& l) const {
		bool f1 = cmpx(O, s);
		bool f2 = cmpx(O, l.s);
		if (f1 != f2) return f1;
		ll CCW = s / l.s;
		return !CCW ? c < l.c : CCW > 0;
	}
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.x << " " << l.s.x << " " << l.c; return os; }
};
Line L(const Pos& s, const Pos& e) {
	int dy, dx; ll c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	int _gcd = gcd(std::abs(dy), std::abs(dx));
	dy /= _gcd; dx /= _gcd;
	c = (ll)dy * s.x + (ll)dx * s.y;
	return Line(Pos(dy, dx), c);
}
struct Seg {
	Pos s, e;
	int h, i;
	Seg(Pos S = Pos(0, 0), Pos E = Pos(0, 0)) : s(S), e(E) { h = 0; i = 0; }
	bool operator == (const Seg& S) const { return s == S.s && e == S.e; }
	bool operator != (const Seg& S) const { return !(*this == S); }
	//bool operator < (const Seg& S) const { return (s == S.s) ? e < S.e : s < S.s; }
	bool operator < (const Seg& rhs) const {
		if (rhs.s == s) return cross(rhs.s, rhs.e, e) < 0;
		if (rhs.s < s) return cross(rhs.s, rhs.e, s) < 0;
		return cross(rhs.s, rhs.e, e) < 0;
	}
	friend std::ostream& operator << (std::ostream& os, const Seg& S) { os << "DEBUG::Seg s: " << S.s << " | e: " << S.e << " DEBUG::Seg\n"; return os; }
} seg[LEN << 3];
struct Bound {
	Line l;
	Pos s, e;
	int i;
	Bound(Pos S = Pos(0, 0), Pos E = Pos(0, 0), int I = -1) : s(S), e(E), i(I) { l = L(S, E); }
	bool operator == (const Bound& S) const { return l == S.l && s == S.s && e == S.e; }
	bool operator != (const Bound& S) const { return !(*this == S); }
	//bool operator < (const Bound& S) const { return (l == S.l) ? (s == S.s) ? e < S.e : s < S.s : l < S.l; }
	bool operator < (const Bound& S) const { return (l == S.l) ? (s == S.s) ? cmpx(e, S.e) : cmpx(s, S.s) : l < S.l; }
	friend std::ostream& operator << (std::ostream& os, const Bound& S) {
		os << "DEBUG::Seg l: " << S.l << " | s: " << S.s << " | e: " << S.e << " DEBUG::Seg\n";
		return os;
	}
};
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
void mirror(Polygon& H) { for (Pos& p : H) p.x *= -1; }
bool sweep(const int& sz) {
	for (int i = 0; i < sz; i++) {
		if (~pos[i].d) {
			if (ST.insert(pos[i].i)) { return 1; }
		}
		else {
			if (ST.pop(pos[i].i)) { return 1; }
		}
	}
	return 0;
}
bool polygon_cross_check(const Polygon& H, const int& n) {
	memset(seg, 0, sizeof seg);
	memset(pos, 0, sizeof pos);
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos s = H[i], e = H[(i + 1) % sz];
		if (e < s) std::swap(s, e);
		seg[i].s = s;
		seg[i].e = e;
		seg[i].h = n;
		seg[i].i = i;
		s.i = e.i = i;
		s.d = 1, e.d = -1;
		pos[i << 1] = s;
		pos[i << 1 | 1] = e;
	}
	sz <<= 1;
	std::sort(pos, pos + sz);
	ST.clear();
	return sweep(sz);
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
void bnd_init(Polygon& h1, Polygon& h2, std::vector<Bound>& V, const int& n1 = -1, const int& n2 = -1) {
	int sz1 = h1.size();
	int sz2 = h2.size();
	for (int i = 0; i < sz1; i++) make_bnd(V, h1[i], h1[(i + 1) % sz1], n1);
	for (int j = 0; j < sz2; j++) make_bnd(V, h2[j], h2[(j + 1) % sz2], n2);
	std::sort(V.begin(), V.end());
	return;
}
void bnd_remove(std::vector<Bound>& V, std::vector<Bound>& V2, bool merge = 1) {
	int sz = V.size();
	for (int i = 0, j; i < sz; i = j) {
		j = i;
		while (j < sz && V[i].l == V[j].l) j++;
		for (int k = i; k < j - 1; k++) {
			int nxt = k + 1;
			if (V[k].e < V[nxt].s) continue;
			else if (V[k].e == V[nxt].s) {
				if (!merge) continue;
				std::swap(V[k].s, V[nxt].s);
			}
			else if (V[nxt].e < V[k].e) {
				int s = V[k].s.i;
				int e = V[k].e.i;
				std::swap(V[k].e, V[nxt].e);
				std::swap(V[k].e, V[nxt].s);
				//if (!merge) continue;
				V[nxt].i = V[k].i;
				V[k].s.i = V[nxt].s.i = s;
				V[k].e.i = V[nxt].e.i = s;
			}
			else if (V[k].e <= V[nxt].e) {
				std::swap(V[k].e, V[nxt].s);
				std::swap(V[k].e.i, V[nxt].s.i);
			}
		}
		for (int k = i; k < j; k++) if (V[k].s != V[k].e) V2.push_back(V[k]);
	}
	//std::sort(V2.begin(), V2.end());
	return;
}
bool two_polygon_equal_check(std::vector<Bound>& V1, std::vector<Bound>& V2) {
	int sz = V1.size();
	//std::cout << "WTF:: v1.sz:: " << sz << " v2.sz:: " << V2.size() << "\n";
	if (sz != V2.size()) { return 0; }
	for (int i = 0; i < sz; i++) if (V1[i] != V2[i]) { return 0; }
	return 1;
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
#ifdef AUTO_CHECK
std::string solve(const std::string& input_file) {
	std::ifstream input(input_file);
#else
std::string solve() {
#endif
	INNER_CHECK = 0;
	EDGE_IGNORE = 0;
	ai = bi = -1;
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);

	for (int i = 0; i < 4; i++) Polygon().swap(H[i]);
	for (int j = 0; j < 3; j++) {
		std::unordered_set<ll> S;
		int x = -1;
		memset(F, 0, sizeof F);
#ifdef AUTO_CHECK
		input >> len[j];
		H[j].resize(len[j]);
		for (int i = 0; i < len[j]; i++) input >> H[j][i];
#else
		std::cin >> len[j];
		H[j].resize(len[j]);
		for (int i = 0; i < len[j]; i++) std::cin >> H[j][i];
#endif
		for (int i = 0; i < len[j]; i++)
			if (!ccw(H[j][(i - 1 + len[j]) % len[j]], H[j][i], H[j][(i + 1) % len[j]]) &&
				dot(H[j][(i - 1 + len[j]) % len[j]], H[j][i], H[j][(i + 1) % len[j]]) <= 0) {
				x = i; break;
			}
		for (int i = 0; i < len[j]; i++)
			if (!ccw(H[j][(i - 1 + len[j]) % len[j]], H[j][i], H[j][(i + 1) % len[j]]))
				F[i] = 1;
		for (int i = 0; i < len[j]; i++) if (!F[i]) H[3].push_back(H[j][i]);
		Polygon().swap(H[j]);
		H[j] = H[3];
		len[j] = (int)H[j].size();
		for (int i = 0; i < len[j]; i++) S.insert(hash(H[j][i]));
		//std::cout << "len[j]:: " << len[j] << " S.sz:: " << S.size() << "\n";
		if (x != -1 || len[j] != S.size()) return "A" + (j == 0 ? std::string("a") : j == 1 ? std::string("b") : std::string("ab")) + "stria is not a polygon";
		for (int k = 0; k < 2; k++) {
			if (polygon_cross_check(H[j], j)) return "A" + (j == 0 ? std::string("a") : j == 1 ? std::string("b") : std::string("ab")) + "stria is not a polygon";
			mirror(H[j]);
		}
		if (norm(H[j])) return "A" + (j == 0 ? std::string("a") : j == 1 ? std::string("b") : std::string("ab")) + "stria is not a polygon";
		for (int i = 0; i < len[j]; i++) H[j][i].i = i;
		Polygon().swap(H[3]);
	}

	//Python debug
	//std::cout << "A = [\n";
	//for (Pos& p : H[0]) std::cout << "  (" << p.x << ", " << p.y << "),\n";
	//std::cout << "]\n";
	//std::cout << "B = [\n";
	//for (Pos& p : H[1]) std::cout << "  (" << p.x << ", " << p.y << "),\n";
	//std::cout << "]\n";
	//std::cout << "AB = [\n";
	//for (Pos& p : H[2]) std::cout << "  (" << p.x << ", " << p.y << "),\n";
	//std::cout << "]\n";

	ll aa = area(H[0]), ba = area(H[1]), aba = area(H[2]);
	bool aeq = aa + ba == aba;

	std::vector<Bound> VS, V, VA, VB, VAB;
	bnd_init(H[0], H[1], VS);
	bnd_remove(VS, V);
	bnd_init(H[2], H[3], VAB);

	//Two polygons are exactly the same
	if (V.empty()) return "Aastria and Abstria intersect";

	bool seq = two_polygon_equal_check(V, VAB);

	//The union of two polygons is the same as the merged polygon
	if (aeq && seq) return "OK";
	//It's the same, but something is strange
	if (!aeq && seq) return "Aastria and Abstria intersect";

	const Polygon& A = H[0];
	const Polygon& B = H[1];
	N = A.size();
	M = B.size();
	len[0] = N;
	len[1] = M;

	EDGE_IGNORE = 1;
	//There are line segments that completely intersect
	if (two_polygon_cross_check(A, B, 0, 1)) return "Aastria and Abstria intersect";
	EDGE_IGNORE = 0;

	SetPos SB;
	for (int b = 0; b < M; b++) SB.insert(B[b]);
	for (int a = 0; a < N; a++) {
		auto p = SB.find(A[a]);
		if (p != SB.end()) {
			int b = p->i;
			//Check all parts that meet at one point
			if (inner_check(A, a, B, b)) return "Aastria and Abstria intersect";
		}
	}

	INNER_CHECK = 1;
	std::vector<Bound>().swap(VS);
	std::vector<Bound>().swap(V);
	bnd_init(H[0], H[1], VS, 0, 1);
	bnd_remove(VS, V, NO_MERGE);
	int sz = V.size();
	for (int i = 0; i < sz; i++) {
		if (V[i].i == 0) VA.push_back(V[i]);
		if (V[i].i == 1) VB.push_back(V[i]);
	}
	bool fa = two_polygon_cross_check(A, VB, 0, 1);
	bool fb = two_polygon_cross_check(B, VA, 1, 0);
	INNER_CHECK = 0;

	//Check if there is a point on a line segment
	//if (fa || fb) return "Aastria and Abstria intersect FUCK";
	if (fa || fb) return "Aastria and Abstria intersect";

	//One polygon completely contains another polygon
	if (inner_check(A, B[0]) == 2 || inner_check(B, A[0]) == 2) return "Aastria and Abstria intersect";

	//If all conditions are met, the union of the two polygons and the answer are different.
	return "The union of Aastria and Abstria is not equal to Aabstria";
}

#ifdef AUTO_CHECK
std::vector<std::string> file_names;
int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
		return 1;
	}

	std::ifstream file_list(argv[1]);
	std::vector<std::string> file_names;
	std::string file_name;
	while (file_list >> file_name) {//fuck
		file_names.push_back(file_name);
	}

	std::ofstream result_file("fucked_results.txt");
	bool F = 1;
	for (int i = 0; i < file_names.size(); i += 2) {
		std::string input_file = file_names[i];
		std::string output_file = file_names[i + 1];

		std::ifstream output_stream(output_file);
		std::string answer;

		std::getline(output_stream, answer);
		std::string result;

		//std::cout << "solve run\n";

		result = solve(input_file);

		//std::cout << "solve done\n";

		//std::cout << input_file << ' ' << output_file << '\n';

		std::cout << answer << " " << result << " " << (answer == result) << "\n";
		if (answer != result) {
			F = 0;
			//std::cout << "what the fuck?! wrong answer is returned!!\n";
			std::cout << "the file name is... ";
			std::cout << input_file << ", you idiot.\n";
			//result_file << input_file << '\n';
			result_file << "ANS:: " << answer << " RET:: " << result << "\n";
		}
	}
	if (F) result_file << "Accepted\n";
	result_file.close();
	return 0;
}//boj4000 Kingdom Reunion
#else
int main() { std::cout << solve() << "\n"; return 0; }//boj4000 Kingdom Reunion
#endif


/*

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <cassert>
#include <vector>
#include <random>
#include <array>
#include <tuple>
#include <unordered_set>
typedef long long ll;
typedef double ld;
typedef std::pair<int, int> pi;
const ll INF = 1e17;
const int LEN = 1e4 + 5;
const ld TOL = 1e-7;
//const ll MOD = 1'000'000'007;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
int gcd(int a, int b) { return !b ? a : gcd(b, a % b); }

//#define AUTO_CHECK

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
bool F[LEN];
struct Pos {
	int x, y;
	int i, d;
	Pos(int X = 0, int Y = 0, int I = 0, int D = 0) : x(X), y(Y), i(I), d(D) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	//bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator < (const Pos& r) const { return x == r.x ? y == r.y ? d < r.d : y < r.y : x < r.x; }
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
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
	//friend std::ostream& operator << (std::ostream& os, const Pos& p) {
	//	os << p.x << " " << p.y;
	//	os << " ::DEBUG:: " << p.i  << " " << p.d;
	//	return os;
	//}
} pos[LEN << 3]; const Pos O = Pos(0, 0);
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
struct PosHash { std::size_t operator() (const Pos& p) const { return (((ll)p.x + 100000) << 32) | ((ll)p.y + 100000); } };
typedef std::unordered_set<Pos, PosHash> SetPos;
typedef std::vector<Pos> Polygon;
int len[4];
Polygon H[4];
ll hash(const Pos& p) { return (((ll)p.x + 100000) << 32) | ((ll)p.y + 100000); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return !ret ? 0 : ret > 0 ? 1 : -1; }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
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
ll area(const Polygon& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
bool norm(Polygon& H) {
	ll a = area(H);
	if (!a) return 1;
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
		if (nxt.y <= p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}
struct Line {//ax + by = c
	Pos s;
	ll c;
	Line(Pos V = Pos(0, 0), ll C = 0) : s(V), c(C) {}
	bool operator == (const Line& l) const { return s == l.s && c == l.c; }
	bool operator < (const Line& l) const {
		bool f1 = cmpx(O, s);
		bool f2 = cmpx(O, l.s);
		if (f1 != f2) return f1;
		ll CCW = s / l.s;
		return !CCW ? c < l.c : CCW > 0;
	}
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.x << " " << l.s.x << " " << l.c; return os; }
};
Line L(const Pos& s, const Pos& e) {
	int dy, dx; ll c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	int _gcd = gcd(std::abs(dy), std::abs(dx));
	dy /= _gcd; dx /= _gcd;
	c = (ll)dy * s.x + (ll)dx * s.y;
	return Line(Pos(dy, dx), c);
}
struct Seg {
	Pos s, e;
	int h, i;
	Seg(Pos S = Pos(0, 0), Pos E = Pos(0, 0)) : s(S), e(E) { h = 0; i = 0; }
	bool operator == (const Seg& S) const { return s == S.s && e == S.e; }
	bool operator != (const Seg& S) const { return !(*this == S); }
	//bool operator < (const Seg& S) const { return (s == S.s) ? e < S.e : s < S.s; }
	bool operator < (const Seg& rhs) const {
		if (rhs.s == s) return cross(rhs.s, rhs.e, e) < 0;
		if (rhs.s < s) return cross(rhs.s, rhs.e, s) < 0;
		return cross(rhs.s, rhs.e, e) < 0;
	}
	friend std::ostream& operator << (std::ostream& os, const Seg& S) { os << "DEBUG::Seg s: " << S.s << " | e: " << S.e << " DEBUG::Seg\n"; return os; }
} seg[LEN << 3];
struct Bound {
	Line l;
	Pos s, e;
	int i;
	Bound(Pos S = Pos(0, 0), Pos E = Pos(0, 0), int I = -1) : s(S), e(E), i(I) { l = L(S, E); }
	bool operator == (const Bound& S) const { return l == S.l && s == S.s && e == S.e; }
	bool operator != (const Bound& S) const { return !(*this == S); }
	//bool operator < (const Bound& S) const { return (l == S.l) ? (s == S.s) ? e < S.e : s < S.s : l < S.l; }
	bool operator < (const Bound& S) const { return (l == S.l) ? (s == S.s) ? cmpx(e, S.e) : cmpx(s, S.s) : l < S.l; }
	friend std::ostream& operator << (std::ostream& os, const Bound& S) {
		os << "DEBUG::Seg l: " << S.l << " | s: " << S.s << " | e: " << S.e << " DEBUG::Seg\n";
		return os;
	}
};
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
void mirror(Polygon& H) { for (Pos& p : H) p.x *= -1; }
bool sweep(const int& sz) {
	for (int i = 0; i < sz; i++) {
		if (~pos[i].d) {
			if (ST.insert(pos[i].i)) { return 1; }
		}
		else {
			if (ST.pop(pos[i].i)) { return 1; }
		}
	}
	return 0;
}
bool polygon_cross_check(const Polygon& H, const int& n) {
	memset(seg, 0, sizeof seg);
	memset(pos, 0, sizeof pos);
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos s = H[i], e = H[(i + 1) % sz];
		if (e < s) std::swap(s, e);
		seg[i].s = s;
		seg[i].e = e;
		seg[i].h = n;
		seg[i].i = i;
		s.i = e.i = i;
		s.d = 1, e.d = -1;
		pos[i << 1] = s;
		pos[i << 1 | 1] = e;
	}
	sz <<= 1;
	std::sort(pos, pos + sz);
	ST.clear();
	return sweep(sz);
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
void bnd_init(Polygon& h1, Polygon& h2, std::vector<Bound>& V, const int& n1 = -1, const int& n2 = -1) {
	int sz1 = h1.size();
	int sz2 = h2.size();
	for (int i = 0; i < sz1; i++) make_bnd(V, h1[i], h1[(i + 1) % sz1], n1);
	for (int j = 0; j < sz2; j++) make_bnd(V, h2[j], h2[(j + 1) % sz2], n2);
	std::sort(V.begin(), V.end());
	return;
}
void bnd_remove(std::vector<Bound>& V, std::vector<Bound>& V2, bool merge = 1) {
	int sz = V.size();
	for (int i = 0, j; i < sz; i = j) {
		j = i;
		while (j < sz && V[i].l == V[j].l) j++;
		for (int k = i; k < j - 1; k++) {
			int nxt = k + 1;
			if (V[k].e < V[nxt].s) continue;
			else if (V[k].e == V[nxt].s) {
				if (!merge) continue;
				std::swap(V[k].s, V[nxt].s);
			}
			else if (V[nxt].e < V[k].e) {
				int s = V[k].s.i;
				int e = V[k].e.i;
				std::swap(V[k].e, V[nxt].e);
				std::swap(V[k].e, V[nxt].s);
				//if (!merge) continue;
				V[nxt].i = V[k].i;
				V[k].s.i = V[nxt].s.i = s;
				V[k].e.i = V[nxt].e.i = s;
			}
			else if (V[k].e <= V[nxt].e) {
				std::swap(V[k].e, V[nxt].s);
				std::swap(V[k].e.i, V[nxt].s.i);
			}
		}
		for (int k = i; k < j; k++) if (V[k].s != V[k].e) V2.push_back(V[k]);
	}
	//std::sort(V2.begin(), V2.end());
	return;
}
bool two_polygon_equal_check(std::vector<Bound>& V1, std::vector<Bound>& V2) {
	int sz = V1.size();
	//std::cout << "WTF:: v1.sz:: " << sz << " v2.sz:: " << V2.size() << "\n";
	if (sz != V2.size()) { return 0; }
	for (int i = 0; i < sz; i++) if (V1[i] != V2[i]) { return 0; }
	return 1;
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
#ifdef AUTO_CHECK
std::string solve(const std::string& input_file) {
	std::ifstream input(input_file);
#else
std::string solve() {
#endif
	INNER_CHECK = 0;
	EDGE_IGNORE = 0;
	ai = bi = -1;
	for (int j = 0; j < 3; j++) {
		std::unordered_set<ll> S;
		int x = -1;
		memset(F, 0, sizeof F);
#ifdef AUTO_CHECK
		input >> len[j];
		H[j].resize(len[j]);
		for (int i = 0; i < len[j]; i++) input >> H[j][i];
#else
		//std::cin >> len[j];
		//if (!(std::cin >> len[j])) return "";
		//H[j].resize(len[j]);
		//for (int i = 0; i < len[j]; i++) std::cin >> H[j][i];
#endif
		for (int i = 0; i < len[j]; i++)
			if (!ccw(H[j][(i - 1 + len[j]) % len[j]], H[j][i], H[j][(i + 1) % len[j]]) &&
				dot(H[j][(i - 1 + len[j]) % len[j]], H[j][i], H[j][(i + 1) % len[j]]) <= 0) {
				x = i; break;
			}
		for (int i = 0; i < len[j]; i++)
			if (!ccw(H[j][(i - 1 + len[j]) % len[j]], H[j][i], H[j][(i + 1) % len[j]]))
				F[i] = 1;
		for (int i = 0; i < len[j]; i++) if (!F[i]) H[3].push_back(H[j][i]);
		Polygon().swap(H[j]);
		H[j] = H[3];
		len[j] = (int)H[j].size();
		for (int i = 0; i < len[j]; i++) S.insert(hash(H[j][i]));
		//std::cout << "len[j]:: " << len[j] << " S.sz:: " << S.size() << "\n";
		if (x != -1 || len[j] != S.size()) return "A" + (j == 0 ? std::string("a") : j == 1 ? std::string("b") : std::string("ab")) + "stria is not a polygon";
		for (int k = 0; k < 2; k++) {
			if (polygon_cross_check(H[j], j)) return "A" + (j == 0 ? std::string("a") : j == 1 ? std::string("b") : std::string("ab")) + "stria is not a polygon";
			mirror(H[j]);
		}
		if (norm(H[j])) return "A" + (j == 0 ? std::string("a") : j == 1 ? std::string("b") : std::string("ab")) + "stria is not a polygon";
		for (int i = 0; i < len[j]; i++) H[j][i].i = i;
		Polygon().swap(H[3]);
	}

	//Python debug
	//std::cout << "A = [\n";
	//for (Pos& p : H[0]) std::cout << "  (" << p.x << ", " << p.y << "),\n";
	//std::cout << "]\n";
	//std::cout << "B = [\n";
	//for (Pos& p : H[1]) std::cout << "  (" << p.x << ", " << p.y << "),\n";
	//std::cout << "]\n";
	//std::cout << "AB = [\n";
	//for (Pos& p : H[2]) std::cout << "  (" << p.x << ", " << p.y << "),\n";
	//std::cout << "]\n";

	ll aa = area(H[0]), ba = area(H[1]), aba = area(H[2]);
	bool aeq = aa + ba == aba;

	std::vector<Bound> VS, V, VA, VB, VAB;
	bnd_init(H[0], H[1], VS);
	bnd_remove(VS, V);
	bnd_init(H[2], H[3], VAB);

	//Two polygons are exactly the same
	if (V.empty()) return "Aastria and Abstria intersect";

	bool seq = two_polygon_equal_check(V, VAB);

	//The union of two polygons is the same as the merged polygon
	if (aeq && seq) return "OK";
	//It's the same, but something is strange
	if (!aeq && seq) return "Aastria and Abstria intersect";

	const Polygon& A = H[0];
	const Polygon& B = H[1];
	N = A.size();
	M = B.size();
	len[0] = N;
	len[1] = M;

	EDGE_IGNORE = 1;
	//There are line segments that completely intersect
	if (two_polygon_cross_check(A, B, 0, 1)) return "Aastria and Abstria intersect";
	EDGE_IGNORE = 0;

	SetPos SB;
	for (int b = 0; b < M; b++) SB.insert(B[b]);
	for (int a = 0; a < N; a++) {
		auto p = SB.find(A[a]);
		if (p != SB.end()) {
			int b = p->i;
			//Check all parts that meet at one point
			if (inner_check(A, a, B, b)) return "Aastria and Abstria intersect";
		}
	}

	INNER_CHECK = 1;
	std::vector<Bound>().swap(VS);
	std::vector<Bound>().swap(V);
	bnd_init(H[0], H[1], VS, 0, 1);
	bnd_remove(VS, V, NO_MERGE);
	int sz = V.size();
	for (int i = 0; i < sz; i++) {
		if (V[i].i == 0) VA.push_back(V[i]);
		if (V[i].i == 1) VB.push_back(V[i]);
	}
	bool fa = two_polygon_cross_check(A, VB, 0, 1);
	bool fb = two_polygon_cross_check(B, VA, 1, 0);
	INNER_CHECK = 0;

	//Check if there is a point on a line segment
	//if (fa || fb) return "Aastria and Abstria intersect FUCK";
	if (fa || fb) return "Aastria and Abstria intersect";

	//One polygon completely contains another polygon
	if (inner_check(A, B[0]) == 2 || inner_check(B, A[0]) == 2) return "Aastria and Abstria intersect";

	//If all conditions are met, the union of the two polygons and the answer are different.
	return "The union of Aastria and Abstria is not equal to Aabstria";
}

void fuck() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int x = 0;
	int N;
	while (std::cin >> N) {
		len[x] = N;
		H[x].resize(N);
		for (int i = 0; i < N; i++) std::cin >> H[x][i];
		x++;
		if (x == 3) {
			x = 0;
			std::cout << solve() << "\n";
			for (int i = 0; i < 4; i++) Polygon().swap(H[i]);
		}
	}
	return;
}

#ifdef AUTO_CHECK
std::vector<std::string> file_names;
int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
		return 1;
	}

	std::ifstream file_list(argv[1]);
	std::vector<std::string> file_names;
	std::string file_name;
	while (file_list >> file_name) {//fuck
		file_names.push_back(file_name);
	}

	std::ofstream result_file("fucked_results.txt");
	bool F = 1;
	for (int i = 0; i < file_names.size(); i += 2) {
		std::string input_file = file_names[i];
		std::string output_file = file_names[i + 1];

		std::ifstream output_stream(output_file);
		std::string answer;

		std::getline(output_stream, answer);
		std::string result;

		//std::cout << "solve run\n";

		result = solve(input_file);

		//std::cout << "solve done\n";

		//std::cout << input_file << ' ' << output_file << '\n';

		std::cout << answer << " " << result << " " << (answer == result) << "\n";
		if (answer != result) {
			F = 0;
			//std::cout << "what the fuck?! wrong answer is returned!!\n";
			std::cout << "the file name is... ";
			std::cout << input_file << ", you idiot.\n";
			//result_file << input_file << '\n';
			result_file << "ANS:: " << answer << " RET:: " << result << "\n";
		}
	}
	if (F) result_file << "Accepted\n";
	result_file.close();
	return 0;
}//boj4000 Kingdom Reunion
#else
int main() { fuck(); return 0; }//boj4000 Kingdom Reunion
#endif

*/