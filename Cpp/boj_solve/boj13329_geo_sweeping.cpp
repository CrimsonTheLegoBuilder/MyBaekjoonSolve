#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
typedef long long ll;
const int LEN = 100'001;
int N, M, v;
bool visible[LEN];

struct Pos {
	int x, y;
	int i;
	bool operator < (const Pos& p) const { return (ll)y * p.x < (ll)x * p.y; }
	//bool operator <= (const Pos& p) const { return y * p.x <= x * p.y; }
}s, e, S[LEN], E[LEN];
std::vector<Pos> meteor, A;

ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (ll)(d2.x - d1.x) * (ll)(d3.y - d2.y) - (ll)(d2.y - d1.y) * (ll)(d3.x - d2.x);
}

struct Idx {
	int i;
	bool operator < (const Idx& x) const {
		if (i == x.i) return 0;
		//if (S[i] <= S[x.i] && E[x.i] <= E[i]) return cross(E[i], S[i], S[x.i]) > 0;
		//else if (S[x.i] <= S[i] && E[i] <= E[x.i]) return cross(E[x.i], S[x.i], S[i]) < 0;
		if (S[i] < S[x.i]) return cross(E[i], S[i], S[x.i]) > 0;
		else return cross(E[x.i], S[x.i], S[i]) < 0;
	}
};
std::set<Idx> I;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) {
		std::cin >> M;
		meteor.resize(M);
		for (int j = 0; j < M; j++) { std::cin >> meteor[j].x >> meteor[j].y; }
		s = *std::min_element(meteor.begin(), meteor.end());
		e = *std::max_element(meteor.begin(), meteor.end());
		S[i] = s, E[i] = e;
		A.push_back({ s.x, s.y, i });
		A.push_back({ e.x, e.y, -i });
	}
	std::sort(A.begin(), A.end());

	for (const Pos& a : A) {
		if (a.i > 0) {
			I.insert({ a.i });
			v = (*I.begin()).i;
			visible[v] = 1;
		}
		else {
			I.erase({ -a.i });
			if (I.empty()) continue;
			v = (*I.begin()).i;
			visible[v] = 1;
		}
	}
	int cnt = 0;
	for (int i = 1; i <= N; i++) if (!visible[i]) cnt++;
	std::cout << cnt << "\n";
	return 0;
}

/*

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 100'001;
int N, M, v;
bool visible[LEN];

struct Pos {
	int x, y;
	int i;
	bool operator < (const Pos& p) const { return (ll)y * p.x < (ll)x * p.y; }
	//bool operator <= (const Pos& p) const { return y * p.x <= x * p.y; }
}s, e, S[LEN], E[LEN];
std::vector<Pos> meteor, A;

ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (ll)(d2.x - d1.x) * (d3.y - d2.y) - (ll)(d2.y - d1.y) * (d3.x - d2.x);
}

struct Seg {
	int i;
	Seg(int i) : i(i) {}
	bool operator < (const Seg& x) const {
		if (i == x.i) return 0;
		//if (S[i] <= S[x.i] && E[x.i] <= E[i]) return cross(E[i], S[i], S[x.i]) > 0;
		//else if (S[x.i] <= S[i] && E[i] <= E[x.i]) return cross(E[x.i], S[x.i], S[i]) < 0;
		if (S[i] < S[x.i]) return cross(E[i], S[i], S[x.i]) > 0;
		else return cross(E[x.i], S[x.i], S[i]) < 0;
	}
};


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
	void insert(int i) {
		if (!root) {
			root = new Node(i);
			return;
		}
		Node* p = root;
		Node** pp;
		while (1) {
			if (p->i == i) return;
			if (Seg(i) < Seg(p->i)) {
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
	}
	bool find(int i) {
		if (!root) return 0;
		Node* p = root;
		while (1) {
			if (p->i == i) break;
			if (Seg(i) < Seg(p->i)) {
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
	void pop(int i) {
		if (!find(i)) return;
		Node* p = root;
		if (p->l && p->r) {
			root = p->l; root->p = 0;
			Node* l = root;
			while (l->r) l = l->r;
			l->r = p->r;
			p->r->p = l;
		}
		else if (p->l) root = p->l, root->p = 0;
		else if (p->r) root = p->r, root->p = 0;
		else root = 0;
		p->l = p->r = 0;
		delete p;
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
	for (int i = 1; i <= N; i++) {
		std::cin >> M;
		meteor.resize(M);
		for (int j = 0; j < M; j++) { std::cin >> meteor[j].x >> meteor[j].y; }
		s = *std::min_element(meteor.begin(), meteor.end());
		e = *std::max_element(meteor.begin(), meteor.end());
		S[i] = s, E[i] = e;
		A.push_back({ s.x, s.y, i });
		A.push_back({ e.x, e.y, -i });
	}
	std::sort(A.begin(), A.end());

	for (const Pos& a : A) {
		if (a.i > 0) {
			I.insert(a.i);
			v = I.top();
			//std::cout << v << "\n";
			visible[v] = 1;
		}
		else {
			I.pop(-a.i);
			v = I.top();
			if (!~v) continue;
			visible[v] = 1;
		}
	}
	int cnt = 0;
	for (int i = 1; i <= N; i++) if (!visible[i]) cnt++;
	std::cout << cnt << "\n";
	return 0;
}

*/