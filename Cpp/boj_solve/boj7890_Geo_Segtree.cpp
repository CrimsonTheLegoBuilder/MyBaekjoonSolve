#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
typedef long long ll;
typedef double ld;
const int LEN = 1e5 + 5;
const int INF = 1e9;
const ll LINF = 1e18;
int N, T;

/*

2-Dimensional coord' sort tree
https://github.com/koosaga/olympiad/blob/master/ICPC/CERC/cerc2008_j.cpp

Because of the memory limit exceed, studied the segtree referring to Koosaga's code

*/
ll sqr(const int& a) { return (ll)a * a; }
struct Pos {
	int x, y, i;
	Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	ll operator ! () const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	ll Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} pos[LEN];
const Pos O = Pos(0, 0);
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
struct Node {
	Pos p;//mid point
	bool spl;//dx < dy ?
	int sx, ex, sy, ey;
	Node(Pos P = Pos(0, 0), bool SPL = 0, int SX = 0, int EX = 0, int SY = 0, int EY = 0) :
		p(P), spl(SPL), sx(SX), ex(EX), sy(SY), ey(EY) {}
} tree[LEN << 2];//segment tree
bool V[LEN << 2];//visited
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
void init(int s = 0, int e = N - 1, int n = 1) {//divide & conquer
	int MAXX = -INF, MAXY = -INF, MINX = INF, MINY = INF;
	int m = s + e >> 1;
	for (int i = s; i <= e; i++) {
		MAXX = std::max(MAXX, pos[i].x);
		MAXY = std::max(MAXY, pos[i].y);
		MINX = std::min(MINX, pos[i].x);
		MINY = std::min(MINY, pos[i].y);
	}
	tree[n].spl = (MAXX - MINX) < (MAXY - MINY);//slope cmp :: dx < dy
	if (tree[n].spl) std::sort(pos + s, pos + e + 1, cmpy);//if dy is dominant, sort by y
	else std::sort(pos + s, pos + e + 1, cmpx);//if dx is dominant, sort by x
	V[n] = 1;
	tree[n] = Node(pos[m], tree[n].spl, MINX, MAXX, MINY, MAXY);
	if (s <= m - 1) init(s, m - 1, n << 1);
	if (m + 1 <= e) init(m + 1, e, n << 1 | 1);
	return;
}
ll search(const Pos& t, ll X = LINF, int n = 1) {//divide & conquer | refer to koosaga
	ll A = LINF, S = LINF;
	if (t != tree[n].p) S = std::min(X, (t - tree[n].p).Euc());
	if (tree[n].spl) {//if dy is dominant
		if (!cmpy(tree[n].p, t)) {//p.y >= t.y
			if (V[n << 1]) A = search(t, S, n << 1);//L search
			S = std::min(S, A);
			if (V[n << 1 | 1] && sqr(tree[n << 1 | 1].sy - t.y) < S) A = search(t, S, n << 1 | 1);
			//if R < MIN, execute R search
		}
		else {//p.y < t.y
			if (V[n << 1 | 1]) A = search(t, S, n << 1 | 1);//R search
			S = std::min(S, A);
			if (V[n << 1] && sqr(tree[n << 1].ey - t.y) < S) A = search(t, S, n << 1);
			//if L < MIN, execute L search
		}
	}
	else {//if dx is dominant
		if (!cmpx(tree[n].p, t)) {//p.x >= t.x
			if (V[n << 1]) A = search(t, S, n << 1);
			S = std::min(S, A);
			if (V[n << 1 | 1] && sqr(tree[n << 1 | 1].sx - t.x) < S) A = search(t, S, n << 1 | 1);
		}
		else {//p.x < t.x
			if (V[n << 1 | 1]) A = search(t, S, n << 1 | 1);
			S = std::min(S, A);
			if (V[n << 1] && sqr(tree[n << 1].ex - t.x) < S) A = search(t, S, n << 1);
		}
	}
	return std::min(S, A);
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) {
		std::cin >> N;
		memset(V, 0, sizeof V);
		for (int i = 0; i < N; i++) std::cin >> pos[i], pos[i].i = i;
		init();
		std::sort(pos, pos + N, cmpi);
		for (int i = 0; i < N; i++) std::cout << search(pos[i]) << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj7890 CERC 2008 J In case of failure


////fucking memory limit exceed
//
//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cstring>
//#include <cassert>
//#include <cmath>
//#include <vector>
//#include <queue>
//#include <deque>
//#include <tuple>
//typedef long long ll;
//typedef double ld;
//typedef long double lld;
//const ld INF = 1e18;
//const ld TOL = 1e-7;
//const ld PI = acos(-1);
//const int LEN = 1e5;
//int N, M, T, Q;
//ll V[LEN];
//bool zero(const ld& x) { return std::abs(x) < TOL; }
//int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
//int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
//ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
//
////O(N log N) Delaunay_triangulation && Voronoi_diagram solver
////https://cp-algorithms.com/geometry/delaunay.html
//struct Pii {
//	int x, y, i;
//	Pii(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
//	bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pii& p) const { return x != p.x || y != p.y; }
//	bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
//	bool operator <= (const Pii& p) const { return x == p.x ? y <= p.y : x <= p.x; }
//	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
//	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
//	Pii operator * (const int& n) const { return { x * n, y * n }; }
//	Pii operator / (const int& n) const { return { x / n, y / n }; }
//	ll operator * (const Pii& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
//	ll operator / (const Pii& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
//	Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
//	Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
//	Pii& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
//	Pii& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
//	Pii operator ~ () const { return { -y, x }; }
//	ll operator ! () const { return (ll)x * y; }
//	ll Euc() const { return (ll)x * x + (ll)y * y; }
//	int Man() const { return std::abs(x) + std::abs(y); }
//	ld mag() const { return hypot(x, y); }
//	friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
//};
//const Pii Oii = { 0, 0 };
//const Pii INF_PT = { (int)INF, (int)INF };
//ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
//ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
//ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
//ll dot(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) * (d4 - d3); }
//int ccw(const Pii& d1, const Pii& d2, const Pii& d3) {
//	ll ret = cross(d1, d2, d3);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//struct QuadEdge {
//	Pii origin;
//	QuadEdge* rot = nullptr;
//	QuadEdge* onext = nullptr;
//	bool used = 0;
//	QuadEdge* rev() const { return rot->rot; }//reverse
//	QuadEdge* lnext() const { return rot->rev()->onext->rot; }
//	QuadEdge* oprev() const { return rot->onext->rot; }
//	Pii dest() const { return rev()->origin; }//destination
//};
//QuadEdge* make_edge(Pii from, Pii to) {
//	QuadEdge* e1 = new QuadEdge;
//	QuadEdge* e2 = new QuadEdge;
//	QuadEdge* e3 = new QuadEdge;
//	QuadEdge* e4 = new QuadEdge;
//	e1->origin = from;
//	e2->origin = to;
//	e3->origin = e4->origin = INF_PT;
//	e1->rot = e3;
//	e2->rot = e4;
//	e3->rot = e2;
//	e4->rot = e1;
//	e1->onext = e1;
//	e2->onext = e2;
//	e3->onext = e4;
//	e4->onext = e3;
//	return e1;
//}
//void splice(QuadEdge* a, QuadEdge* b) {
//	std::swap(a->onext->rot->onext, b->onext->rot->onext);
//	std::swap(a->onext, b->onext);
//}
//void delete_edge(QuadEdge* e) {
//	splice(e, e->oprev());
//	splice(e->rev(), e->rev()->oprev());
//	delete e->rev()->rot;
//	delete e->rev();
//	delete e->rot;
//	delete e;
//}
//QuadEdge* connect(QuadEdge* a, QuadEdge* b) {
//	QuadEdge* e = make_edge(a->dest(), b->origin);
//	splice(e, a->lnext());
//	splice(e->rev(), b);
//	return e;
//}
//bool left_of(Pii p, QuadEdge* e) { return cross(p, e->origin, e->dest()) > 0; }
//bool right_of(Pii p, QuadEdge* e) { return cross(p, e->origin, e->dest()) < 0; }
//template <class T> T det3(T a1, T a2, T a3, T b1, T b2, T b3, T c1, T c2, T c3) {
//	return a1 * (b2 * c3 - c2 * b3)
//		- a2 * (b1 * c3 - c1 * b3)
//		+ a3 * (b1 * c2 - c1 * b2);
//}
//bool in_circle(Pii a, Pii b, Pii c, Pii d) {
//	lld det = -det3<lld>(b.x, b.y, b.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
//	det += det3<lld>(a.x, a.y, a.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
//	det -= det3<lld>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), d.x, d.y, d.Euc());
//	det += det3<lld>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), c.x, c.y, c.Euc());
//	if (fabs(det) > 1e18) return det > 0;//overflow prevention (refer to koosaga)
//	else {
//		ll det = -det3<ll>(b.x, b.y, b.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
//		det += det3<ll>(a.x, a.y, a.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
//		det -= det3<ll>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), d.x, d.y, d.Euc());
//		det += det3<ll>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), c.x, c.y, c.Euc());
//		return det > 0;
//	}
//}
//std::pair<QuadEdge*, QuadEdge*> build_tr(int l, int r, std::vector<Pii>& C) {
//	if (r - l + 1 == 2) {
//		QuadEdge* res = make_edge(C[l], C[r]);
//		return std::make_pair(res, res->rev());
//	}
//	if (r - l + 1 == 3) {
//		QuadEdge* a = make_edge(C[l], C[l + 1]), * b = make_edge(C[l + 1], C[r]);
//		splice(a->rev(), b);
//		int sign = ccw(C[l], C[l + 1], C[r]);
//		if (!sign) return std::make_pair(a, b->rev());
//		QuadEdge* c = connect(b, a);
//		if (sign == 1) return std::make_pair(a, b->rev());
//		else return std::make_pair(c->rev(), c);
//	}
//	int m = l + r >> 1;
//	QuadEdge* ldo, * ldi, * rdo, * rdi;
//	std::tie(ldo, ldi) = build_tr(l, m, C);
//	std::tie(rdi, rdo) = build_tr(m + 1, r, C);
//	while (1) {
//		if (left_of(rdi->origin, ldi)) {
//			ldi = ldi->lnext();
//			continue;
//		}
//		if (right_of(ldi->origin, rdi)) {
//			rdi = rdi->rev()->onext;//rnext
//			continue;
//		}
//		break;
//	}
//	QuadEdge* basel = connect(rdi->rev(), ldi);
//	auto valid = [&basel](QuadEdge* e) { return right_of(e->dest(), basel); };
//	if (ldi->origin == ldo->origin) ldo = basel->rev();
//	if (rdi->origin == rdo->origin) rdo = basel;
//	while (1) {
//		QuadEdge* lcand = basel->rev()->onext;
//		if (valid(lcand)) {
//			while (in_circle(basel->dest(), basel->origin, lcand->dest(), lcand->onext->dest())) {
//				QuadEdge* t = lcand->onext;
//				delete_edge(lcand);
//				lcand = t;
//			}
//		}
//		QuadEdge* rcand = basel->oprev();
//		if (valid(rcand)) {
//			while (in_circle(basel->dest(), basel->origin, rcand->dest(), rcand->oprev()->dest())) {
//				QuadEdge* t = rcand->oprev();
//				delete_edge(rcand);
//				rcand = t;
//			}
//		}
//		if (!valid(lcand) && !valid(rcand)) break;
//		if (!valid(lcand) || (valid(rcand) && in_circle(lcand->dest(), lcand->origin, rcand->origin, rcand->dest())))
//			basel = connect(rcand, basel->rev());
//		else
//			basel = connect(basel->rev(), lcand->rev());
//	}
//	return std::make_pair(ldo, rdo);
//}
////std::vector<std::tuple<Pii, Pii, Pii>> Delaunay_triangulation(std::vector<Pii> C) {
//void Delaunay_triangulation(std::vector<Pii>& C) {
//	std::sort(C.begin(), C.end());
//	int sz = C.size();
//	auto ret = build_tr(0, sz - 1, C);
//	QuadEdge* e = ret.first;
//	std::vector<QuadEdge*> edges = { e };
//	while (cross(e->onext->dest(), e->dest(), e->origin) < 0) e = e->onext;
//	auto add = [&C, &e, &edges]() {
//		QuadEdge* cur = e;
//		do {
//			cur->used = 1;
//			C.push_back(cur->origin);
//			edges.push_back(cur->rev());
//			cur = cur->lnext();
//		} while (cur != e);
//		};
//	add();
//	C.clear();
//	int kek = 0;
//	while (kek < edges.size()) if (!(e = edges[kek++])->used) add();
//	sz = C.size();
//	//std::vector<std::tuple<Pii, Pii, Pii>> triangles;
//	//for (int i = 0; i < sz; i += 3) triangles.push_back(std::make_tuple(C[i], C[i + 1], C[i + 2]));
//	for (int i = 0; i < sz; i += 3) {
//		Pii a = C[i], b = C[i + 1], c = C[i + 2];
//		V[a.i] = std::min({ V[a.i], (a - b).Euc(), (a - b).Euc() });
//		V[b.i] = std::min({ V[b.i], (b - a).Euc(), (b - c).Euc() });
//		V[c.i] = std::min({ V[c.i], (c - a).Euc(), (c - b).Euc() });
//	}
//	//return triangles;
//	return;
//}
//std::vector<Pii> C;
////std::vector<int> seed[LEN];
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> T;
//	while (T--) {
//		std::cin >> N;
//		C.resize(N);
//		for (int i = 0; i < N; i++) std::cin >> C[i], C[i].i = i, V[i] = INF;
//		//assert(N);
//		if (N == 1) {}
//		else if (N == 2) { ll D = (C[0] - C[1]).Euc(); std::cout << D << "\n" << D << "\n"; }
//		else { Delaunay_triangulation(C); for (int i = 0; i < N; i++) std::cout << V[i] << "\n"; }
//	}
//	return;
//}
//int main() { solve(); return 0; }//boj7890 CERC 2008 J In case of failure



//Pos Q[LEN];
//ll MIN = LINF;
//void solve(const Pos& t, int n = 1) {//divide & conquer | refer to koosaga
//	if (t != tree[n].p) MIN = std::min(MIN, (t - tree[n].p).Euc());
//	if (tree[n].spl) {//if dy is dominant
//		if (!cmpy(tree[n].p, t)) {//p.y >= t.y
//			if (V[n << 1]) solve(t, n << 1);//L search
//			if (V[n << 1 | 1] && sqr(tree[n << 1 | 1].sy - t.y) < MIN)
//				solve(t, n << 1 | 1);//if L < MIN, execute R search
//		}
//		else {//p.y < t.y
//			if (V[n << 1 | 1]) solve(t, n << 1 | 1);
//			if (V[n << 1] && sqr(tree[n << 1].ey - t.y) < MIN) solve(t, n << 1);
//		}
//	}
//	else {//if dx is dominant
//		if (!cmpx(tree[n].p, t)) {//p.x >= t.x
//			if (V[n << 1]) solve(t, n << 1);
//			if (V[n << 1 | 1] && sqr(tree[n << 1 | 1].sx - t.x) < MIN) solve(t, n << 1 | 1);
//		}
//		else {//p.y < t.y
//			if (V[n << 1 | 1]) solve(t, n << 1 | 1);
//			if (V[n << 1] && sqr(tree[n << 1].ex - t.x) < MIN) solve(t, n << 1);
//		}
//	}
//	return;
//}
		//for (int i = 0; i < N; i++) {
		//	MIN = LINF;
		//	//solve(pos[i]);
		//	solve(Q[i]);
		//	std::cout << MIN << "\n";
		//}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> T;
//	while (T--) {
//		std::cin >> N;
//		memset(V, 0, sizeof V);
//		for (int i = 0; i < N; i++) std::cin >> pos[i], pos[i].i = i;// , Q[i] = pos[i];
//		init();
//		std::sort(pos, pos + N, cmpi);
//		for (int i = 0; i < N; i++) std::cout << search(pos[i]) << "\n";
//	}
//	return;
//}