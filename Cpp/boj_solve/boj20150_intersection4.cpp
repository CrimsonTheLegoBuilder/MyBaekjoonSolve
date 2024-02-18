#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
typedef long double ld;
const int LEN = 200'001;

int N;

struct Pos {
	ll x, y;
	int i, d;
	bool operator < (const Pos& p) const {
		return x == p.x ? (d == p.d ? y < p.y : d > p.d) : x < p.x;
	}
} pos[LEN << 1];
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (ll)(d2.x - d1.x) * (d3.y - d2.y) - (ll)(d2.y - d1.y) * (d3.x - d2.x);
}
int C(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ccw = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return ccw > 0 ? 1 : ccw < 0 ? -1 : 0;
}
int D(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll dot = (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
	return dot > 0 ? 1 : dot < 0 ? -1 : 0;
}
struct Seg {
	Pos l, r;
	bool operator < (const Seg& x) const {
		if (x.l < l) return cross(x.r, x.l, l) > 0;
		return cross(x.r, x.l, r) > 0;
	}
	bool operator == (const Seg& x) const {
		if (x.l < l) return !cross(x.r, x.l, l);
		return !cross(x.r, x.l, r);
	}

} seg[LEN];
bool X(int a, int b) {
	Pos d1 = seg[a].l, d2 = seg[a].r, d3 = seg[b].l, d4 = seg[b].r;
	bool F1 = C(d1, d2, d3) * C(d2, d1, d4) > 0 && C(d3, d4, d1) * C(d4, d3, d2) > 0;
	bool F2 = !C(d1, d2, d3) && D(d1, d3, d2) >= 0 ||
		!C(d1, d2, d4) && D(d1, d4, d2) >= 0 ||
		!C(d3, d4, d1) && D(d3, d1, d4) >= 0 ||
		!C(d3, d4, d2) && D(d3, d2, d4) >= 0;
	return F1 || F2;
}

class SplayTree {
	struct Node {
		int i;
		Node* l;
		Node* r;
		Node* p;
		Node(int i) : i(i), l(0), r(0), p(0) {}
		~Node() { if (l) delete l; if (r) delete r; }
	}*root;

	void rotate(Node* x) {
		Node* p = x->p;
		if (!p) return;  //if x == root: return
		Node* b = 0;
		if (x == p->l) {
			p->l = b = x->r;
			x->r = p;
		}
		else {
			p->r = b = x->l;
			x->l = p;
		}
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
				if ((x == p->l) == (p == g->l)) rotate(p);
				else rotate(x);
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
			if (X(l->i, i)) return 1;
			splay(l);
		}
		splay(x);
		if (x->r) {
			Node* r = x->r;
			while (r->l) r = r->l;
			if (X(r->i, i)) return 1;
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
			root = p->l; root->p = 0;
			while (l->r) l = l->r;
			l->r = r;
			r->p = l;
			while (r->l) r = r->l;
			if (X(l->i, r->i)) return 1;
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
}I;



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> seg[i].l.x >> seg[i].l.y >> seg[i].r.x >> seg[i].r.y;
		if (seg[i].r < seg[i].l) std::swap(seg[i].r, seg[i].l);
		seg[i].l.i = i, seg[i].r.i = i;
		seg[i].l.d = 1, seg[i].r.d = -1;
		pos[i << 1] = seg[i].l;
		pos[i << 1 | 1] = seg[i].r;
	}
	std::sort(pos, pos + (N * 2));
	for (int i = 0; i < N * 2; i++) {
		if (~pos[i].d) {
			if (I.insert(pos[i].i)) {
				std::cout << 1;
				return 0;
			}
		}
		else {
			if (I.pop(pos[i].i)) {
				std::cout << 1;
				return 0;
			}
		}
	}
	std::cout << 0;
	return 0;
}



//bool X(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
//	bool F1 = C(d1, d2, d3) * C(d2, d1, d4) > 0 && C(d3, d4, d1) * C(d4, d3, d2) > 0;
//	bool F2 = !C(d1, d2, d3) && D(d1, d3, d2) >= 0 ||
//		!C(d1, d2, d4) && D(d1, d4, d2) >= 0 ||
//		!C(d3, d4, d1) && D(d3, d1, d4) >= 0 ||
//		!C(d3, d4, d2) && D(d3, d2, d4) >= 0;
//	return F1 || F2;
//}


/*

8
0 0 9 0
1 5 8 1
1 -5 9 -1
2 -7 9 -2
11 1 15 5
11 0 15 0
11 -1 15 -2
9 -5 11 5
5 10 15 -10


12
0 0 25 0
1 1 1 3
1 -1 1 -3
3 1 3 -1
2 2 4 2
2 -2 4 -2
4 0 6 0
5 1 5 3
5 -1 5 -3
7 1 7 -1
6 2 8 2
-6 2 -8 2

12
0 0 2 0
1 1 1 3
1 -1 1 -3
3 1 3 -1
2 2 4 2
2 -2 4 -2
4 0 6 0
5 1 5 3
5 -1 5 -3
7 1 7 -1
6 2 8 2
-6 2 -8 2

*/