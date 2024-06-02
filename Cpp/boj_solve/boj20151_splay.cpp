#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <random>
#include <array>
#include <tuple>
typedef long long ll;
typedef double ld;
typedef std::pair<int, int> pi;
const ll INF = 1e17;
const int LEN = 2e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
int N, M, T, Q;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }

struct Pos {
	int x, y;
	int i, d;
	Pos(int X = 0, int Y = 0, int I = 0, int D = 0) : x(X), y(Y), i(I), d(D) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	//bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	inline bool operator < (const Pos& p) const { return x == p.x ? y == p.y ? d < p.d : y < p.y : x < p.x; }
	inline bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline Pos operator * (const int& n) const { return { x * n, y * n }; }
	inline Pos operator / (const int& n) const { return { x / n, y / n }; }
	inline ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	inline ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	inline Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	inline Pos operator - () const { return { -x, -y }; }
	inline Pos operator ~ () const { return { -y, x }; }
	inline Pos operator ! () const { return { y, x }; }
	inline ll xy() const { return (ll)x * y; }
	inline ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} pos[LEN << 1]; const Pos O = Pos(0, 0);
//bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
//bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
//bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
typedef std::vector<Pos> Polygon;
inline ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
inline ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
inline bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
inline bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0;
}
inline bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	//return f1 && f2;
	//bool f3 = on_seg_strong(s1, s2, d1) ||
	//	on_seg_strong(s1, s2, d2) ||
	//	on_seg_strong(d1, d2, s1) ||
	//	on_seg_strong(d1, d2, s2);
	//return (f1 && f2) || f3;
	bool f3 = on_seg_weak(s1, s2, d1) ||
		on_seg_weak(s1, s2, d2) ||
		on_seg_weak(d1, d2, s1) ||
		on_seg_weak(d1, d2, s2);
	return (f1 && f2) || f3;
}
struct Seg {
	Pos s, e;
	Seg(Pos S = Pos(0, 0), Pos E = Pos(0, 0)) : s(S), e(E) {}
	bool operator == (const Seg& S) const { return s == S.s && e == S.e; }
	bool operator != (const Seg& S) const { return !(*this == S); }
	//bool operator < (const Seg& S) const { return (s == S.s) ? e < S.e : s < S.s; }
	bool operator < (const Seg& rhs) const {
		if (rhs.s == s) return cross(rhs.s, rhs.e, e) < 0;
		if (rhs.s < s) return cross(rhs.s, rhs.e, s) < 0;
		return cross(rhs.s, rhs.e, e) < 0;
	}
	friend std::ostream& operator << (std::ostream& os, const Seg& S) {
		os << "DEBUG::Seg s: " << S.s << " | e: " << S.e << " DEBUG::Seg\n";
		return os;
	}
} seg[LEN];
inline bool intersect(const int& a, const int& b) {
	const Pos& d1 = seg[a].s, d2 = seg[a].e, d3 = seg[b].s, d4 = seg[b].e;
	return intersect(d1, d2, d3, d4);
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
inline bool solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> seg[i].s >> seg[i].e;
		if (seg[i].e < seg[i].s) std::swap(seg[i].s, seg[i].e);
		seg[i].s.i = seg[i].e.i = i;
		seg[i].s.d = 1, seg[i].e.d = -1;
		pos[i << 1] = seg[i].s;
		pos[i << 1 | 1] = seg[i].e;
	}
	N <<= 1;
	std::sort(pos, pos + N);
	for (int i = 0; i < N; i++) {
		if (~pos[i].d) {
			if (ST.insert(pos[i].i)) {
				std::cout << 1;
				return 0;
			}
		}
		else {
			if (ST.pop(pos[i].i)) {
				std::cout << 1;
				return 0;
			}
		}
	}
	std::cout << 0;
	return 0;
}
int main() { solve(); return 0; }//boj20151