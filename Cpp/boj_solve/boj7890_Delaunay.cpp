#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>
#include <queue>
#include <deque>
#include <tuple>
typedef long long ll;
typedef double ld;
typedef long double lld;
const ld INF = 1e18;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e5;
int N, M, T, Q;
ll V[LEN];
bool zero(const ld& x) { return std::abs(x) < TOL; }
int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

//O(N log N) Delaunay_triangulation && Voronoi_diagram solver
//https://cp-algorithms.com/geometry/delaunay.html
struct Pii {
	int x, y, i;
	Pii(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pii& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pii& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
	Pii operator * (const int& n) const { return { x * n, y * n }; }
	Pii operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pii& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pii& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
	Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
	Pii& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pii& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pii operator ~ () const { return { -y, x }; }
	ll operator ! () const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
};
const Pii Oii = { 0, 0 };
const Pii INF_PT = { (int)INF, (int)INF };
ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pii& d1, const Pii& d2, const Pii& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
struct QuadEdge {
	Pii origin;
	QuadEdge* rot = nullptr;
	QuadEdge* onext = nullptr;
	bool used = 0;
	QuadEdge* rev() const { return rot->rot; }//reverse
	QuadEdge* lnext() const { return rot->rev()->onext->rot; }
	QuadEdge* oprev() const { return rot->onext->rot; }
	Pii dest() const { return rev()->origin; }//destination
};
QuadEdge* make_edge(Pii from, Pii to) {
	QuadEdge* e1 = new QuadEdge;
	QuadEdge* e2 = new QuadEdge;
	QuadEdge* e3 = new QuadEdge;
	QuadEdge* e4 = new QuadEdge;
	e1->origin = from;
	e2->origin = to;
	e3->origin = e4->origin = INF_PT;
	e1->rot = e3;
	e2->rot = e4;
	e3->rot = e2;
	e4->rot = e1;
	e1->onext = e1;
	e2->onext = e2;
	e3->onext = e4;
	e4->onext = e3;
	return e1;
}
void splice(QuadEdge* a, QuadEdge* b) {
	std::swap(a->onext->rot->onext, b->onext->rot->onext);
	std::swap(a->onext, b->onext);
}
void delete_edge(QuadEdge* e) {
	splice(e, e->oprev());
	splice(e->rev(), e->rev()->oprev());
	delete e->rev()->rot;
	delete e->rev();
	delete e->rot;
	delete e;
}
QuadEdge* connect(QuadEdge* a, QuadEdge* b) {
	QuadEdge* e = make_edge(a->dest(), b->origin);
	splice(e, a->lnext());
	splice(e->rev(), b);
	return e;
}
bool left_of(Pii p, QuadEdge* e) { return cross(p, e->origin, e->dest()) > 0; }
bool right_of(Pii p, QuadEdge* e) { return cross(p, e->origin, e->dest()) < 0; }
template <class T> T det3(T a1, T a2, T a3, T b1, T b2, T b3, T c1, T c2, T c3) {
	return a1 * (b2 * c3 - c2 * b3)
		- a2 * (b1 * c3 - c1 * b3)
		+ a3 * (b1 * c2 - c1 * b2);
}
bool in_circle(Pii a, Pii b, Pii c, Pii d) {
	lld det = -det3<lld>(b.x, b.y, b.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
	det += det3<lld>(a.x, a.y, a.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
	det -= det3<lld>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), d.x, d.y, d.Euc());
	det += det3<lld>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), c.x, c.y, c.Euc());
	if (fabs(det) > 1e18) return det > 0;//overflow prevention (refer to koosaga)
	else {
		ll det = -det3<ll>(b.x, b.y, b.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
		det += det3<ll>(a.x, a.y, a.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
		det -= det3<ll>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), d.x, d.y, d.Euc());
		det += det3<ll>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), c.x, c.y, c.Euc());
		return det > 0;
	}
}
std::pair<QuadEdge*, QuadEdge*> build_tr(int l, int r, std::vector<Pii>& C) {
	if (r - l + 1 == 2) {
		QuadEdge* res = make_edge(C[l], C[r]);
		return std::make_pair(res, res->rev());
	}
	if (r - l + 1 == 3) {
		QuadEdge* a = make_edge(C[l], C[l + 1]), * b = make_edge(C[l + 1], C[r]);
		splice(a->rev(), b);
		int sign = ccw(C[l], C[l + 1], C[r]);
		if (!sign) return std::make_pair(a, b->rev());
		QuadEdge* c = connect(b, a);
		if (sign == 1) return std::make_pair(a, b->rev());
		else return std::make_pair(c->rev(), c);
	}
	int m = l + r >> 1;
	QuadEdge* ldo, * ldi, * rdo, * rdi;
	std::tie(ldo, ldi) = build_tr(l, m, C);
	std::tie(rdi, rdo) = build_tr(m + 1, r, C);
	while (1) {
		if (left_of(rdi->origin, ldi)) {
			ldi = ldi->lnext();
			continue;
		}
		if (right_of(ldi->origin, rdi)) {
			rdi = rdi->rev()->onext;//rnext
			continue;
		}
		break;
	}
	QuadEdge* basel = connect(rdi->rev(), ldi);
	auto valid = [&basel](QuadEdge* e) { return right_of(e->dest(), basel); };
	if (ldi->origin == ldo->origin) ldo = basel->rev();
	if (rdi->origin == rdo->origin) rdo = basel;
	while (1) {
		QuadEdge* lcand = basel->rev()->onext;
		if (valid(lcand)) {
			while (in_circle(basel->dest(), basel->origin, lcand->dest(), lcand->onext->dest())) {
				QuadEdge* t = lcand->onext;
				delete_edge(lcand);
				lcand = t;
			}
		}
		QuadEdge* rcand = basel->oprev();
		if (valid(rcand)) {
			while (in_circle(basel->dest(), basel->origin, rcand->dest(), rcand->oprev()->dest())) {
				QuadEdge* t = rcand->oprev();
				delete_edge(rcand);
				rcand = t;
			}
		}
		if (!valid(lcand) && !valid(rcand)) break;
		if (!valid(lcand) || (valid(rcand) && in_circle(lcand->dest(), lcand->origin, rcand->origin, rcand->dest())))
			basel = connect(rcand, basel->rev());
		else
			basel = connect(basel->rev(), lcand->rev());
	}
	return std::make_pair(ldo, rdo);
}
//std::vector<std::tuple<Pii, Pii, Pii>> Delaunay_triangulation(std::vector<Pii> C) {
void Delaunay_triangulation(std::vector<Pii> C) {
	std::sort(C.begin(), C.end());
	int sz = C.size();
	auto ret = build_tr(0, sz - 1, C);
	QuadEdge* e = ret.first;
	std::vector<QuadEdge*> edges = { e };
	while (cross(e->onext->dest(), e->dest(), e->origin) < 0) e = e->onext;
	auto add = [&C, &e, &edges]() {
		QuadEdge* cur = e;
		do {
			cur->used = 1;
			C.push_back(cur->origin);
			edges.push_back(cur->rev());
			cur = cur->lnext();
		} while (cur != e);
		};
	add();
	C.clear();
	int kek = 0;
	while (kek < edges.size()) if (!(e = edges[kek++])->used) add();
	sz = C.size();
	//std::vector<std::tuple<Pii, Pii, Pii>> triangles;
	//for (int i = 0; i < sz; i += 3) triangles.push_back(std::make_tuple(C[i], C[i + 1], C[i + 2]));
	for (int i = 0; i < sz; i += 3) {
		Pii a = C[i], b = C[i + 1], c = C[i + 2];
		V[a.i] = std::min({ V[a.i], (a - b).Euc(), (a - b).Euc() });
		V[b.i] = std::min({ V[b.i], (b - a).Euc(), (b - c).Euc() });
		V[c.i] = std::min({ V[c.i], (c - a).Euc(), (c - b).Euc() });
	}
	//return triangles;
	return;
}
std::vector<Pii> C;
//std::vector<int> seed[LEN];
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) {
		std::cin >> N;
		C.resize(N);
		for (int i = 0; i < N; i++) std::cin >> C[i], C[i].i = i, V[i] = INF;
		//assert(N);
		if (N == 1) {}
		else if (N == 2) { ll D = (C[0] - C[1]).Euc(); std::cout << D << "\n" << D << "\n"; }
		else { Delaunay_triangulation(C); for (int i = 0; i < N; i++) std::cout << V[i] << "\n"; }
	}
	return;
}
int main() { solve(); return 0; }//boj7890



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
//	ll x, y;
//	int i;
//	Pii(ll X = 0, ll Y = 0, int I = 0) : x(X), y(Y), i(I) {}
//	bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pii& p) const { return x != p.x || y != p.y; }
//	bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
//	bool operator <= (const Pii& p) const { return x == p.x ? y <= p.y : x <= p.x; }
//	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
//	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
//	Pii operator * (const ll& n) const { return { x * n, y * n }; }
//	Pii operator / (const ll& n) const { return { x / n, y / n }; }
//	ll operator * (const Pii& p) const { return { x * p.x + y * p.y }; }
//	ll operator / (const Pii& p) const { return { x * p.y - y * p.x }; }
//	Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
//	Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
//	Pii& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
//	Pii& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
//	Pii operator ~ () const { return { -y, x }; }
//	ll operator ! () const { return x * y; }
//	ll Euc() const { return x * x + y * y; }
//	ll Man() const { return std::abs(x) + std::abs(y); }
//	ld mag() const { return hypot(x, y); }
//	friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
//};
//const Pii Oii = { 0, 0 };
//const Pii INF_PT = { (ll)INF, (ll)INF };
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
//void Delaunay_triangulation(std::vector<Pii> C) {
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
//std::vector<int> seed[LEN];
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
//int main() { solve(); return 0; }//boj7890
