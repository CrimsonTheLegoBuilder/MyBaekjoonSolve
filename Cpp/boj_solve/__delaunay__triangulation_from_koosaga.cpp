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
typedef long double lld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e5;
int N, M, T, Q;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

//O(N long N) Delaunay_triangulation && Voronoi_diagram solver 
//code from Koosaga'
struct Pii {
	ll x, y;
	int i;
	Pii(ll X = 0, ll Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pii& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pii& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
	Pii operator * (const ll& n) const { return { x * n, y * n }; }
	Pii operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pii& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pii& p) const { return { x * p.y - y * p.x }; }
	Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
	Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
	Pii& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	Pii& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
	Pii operator ~ () const { return { -y, x }; }
	ll operator ! () const { return x * y; }
	ll Euc() const { return x * x + y * y; }
	ll Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
};
const Pii O = { 0, 0 };
const Pii INFP = { INF, INF };
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
	bool used = false;
	QuadEdge* rev() const { return rot->rot; }
	QuadEdge* lnext() const { return rot->rev()->onext->rot; }
	QuadEdge* oprev() const { return rot->onext->rot; }
	Pii dest() const { return rev()->origin; }
};
QuadEdge* make_edge(Pii from, Pii to) {
	QuadEdge* e1 = new QuadEdge;
	QuadEdge* e2 = new QuadEdge;
	QuadEdge* e3 = new QuadEdge;
	QuadEdge* e4 = new QuadEdge;
	e1->origin = from;
	e2->origin = to;
	e3->origin = e4->origin = INFP;
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
	lld det = -det3<lld>(b.x, b.y, b.Euc(), c.x, c.y, a.Euc(), d.x, d.y, d.Euc());
	det += det3<lld>(a.x, a.y, a.Euc(), c.x, c.y, a.Euc(), d.x, d.y, d.Euc());
	det -= det3<lld>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), d.x, d.y, d.Euc());
	det += det3<lld>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), d.x, d.y, d.Euc());
	if (abs(det) > INF) return det > 0;
	else {
		ll det = -det3<ll>(b.x, b.y, b.Euc(), c.x, c.y, a.Euc(), d.x, d.y, d.Euc());
		det += det3<ll>(a.x, a.y, a.Euc(), c.x, c.y, a.Euc(), d.x, d.y, d.Euc());
		det -= det3<ll>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), d.x, d.y, d.Euc());
		det += det3<ll>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), d.x, d.y, d.Euc());
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
		int sign = dcmp(cross(C[l], C[l + 1], C[r]));
		if (!sign) return std::make_pair(a, b->rev());
		QuadEdge* c = connect(a, b->rev());
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
			rdi = rdi->rev()->onext;
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
std::vector<std::tuple<Pii, Pii, Pii>> Delaunay_triangulation(std::vector<Pii> C) {
	//std::sort(C.begin(), C.end(), [](const Pii& p, const Pii& q) {
	//	return p.x < q.x || (p.x == q.x && p.y < q.y);
	//	});
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
	while (kek < edges.size()) {
		if (!(e = edges[kek++])->used) add();
	}
	sz = C.size();
	std::vector<std::tuple<Pii, Pii, Pii>> triangles;
	for (int i = 0; i < sz; i += 3) triangles.push_back(std::make_tuple(C[i], C[i + 1], C[i + 2]));
	return triangles;
}
struct Pdd {
	ld x, y;
	Pdd(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pdd& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pdd& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pdd& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pdd operator + (const Pdd& p) const { return { x + p.x, y + p.y }; }
	Pdd operator - (const Pdd& p) const { return { x - p.x, y - p.y }; }
	Pdd operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pdd operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pdd& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pdd& p) const { return { x * p.y - y * p.x }; }
	Pdd operator ~ () const { return { -y, x }; }
	ld operator ! () const { return x * y; }
	Pdd& operator += (const Pdd& p) { x += p.x; y += p.y; return *this; }
	Pdd& operator -= (const Pdd& p) { x -= p.x; y -= p.y; return *this; }
	Pdd& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pdd& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
	Pdd unit() const { return *this / mag(); }
	friend std::istream& operator >> (std::istream& is, Pdd& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pdd& p) { os << p.x << " " << p.y; return os; }
}; const Pdd O = { 0, 0 };
Pdd P(const Pii& p) { return Pdd((ld)p.x, (ld)p.y); }
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
	Vec& operator /= (const ld& scalar) { vy /= scalar; vx /= scalar; return *this; }
	ld mag() const { return hypot(vy, vx); }
}; const Vec Zero = { 0, 0 };
struct Line {//ax + by = c
	Vec s;
	ld c;
	Line(Vec V = Vec(0, 0), ld C = 0) : s(V), c(C) {}
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld CCW = s / l.s;
		return zero(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
	}
	ld operator * (const Line& l) const { return s * l.s; }
	ld operator / (const Line& l) const { return s / l.s; }
	Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
	Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
	ld dist(const Pdd& p) const { return s.vy * p.x + s.vx * p.y; }
	ld above(const Pdd& p) const { return s.vy * p.x + s.vx * p.y - c; }
	ld mag() const { return s.mag(); }
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
};
const Line Xaxis = { { 0, -1 }, 0 };
const Line Yaxis = { { 1, 0 }, 0 };
Line L(const Pdd& s, const Pdd& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { { dy, dx } , c };
}
Line rotate(const Line& l, const Pdd& p, ld the) {
	Vec s = l.s;
	ld x = -s.vx, y = s.vy;
	ld vx = -(x * cos(the) - y * sin(the));
	ld vy = x * sin(the) + y * cos(the);
	ld c = vy * p.x + vx * p.y;
	return Line(Vec(vy, vx), c);
}
Line rotate90(const Line& l, const Pdd& p) {
	Vec s = ~l.s;
	ld c = s.vy * p.x + s.vx * p.y;
	return { s, c };
}
Pdd intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
ld cross(const Pdd& d1, const Pdd& d2, const Pdd& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pdd& d1, const Pdd& d2, const Pdd& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pdd& d1, const Pdd& d2, const Pdd& d3) { return (d2 - d1) * (d3 - d2); }
bool CW(const Line& l1, const Line& l2, const Line& target) {
	if (l1.s / l2.s < TOL) return 0;
	Pdd p = intersection(l1, l2);
	return target.s.vy * p.x + target.s.vx * p.y > target.c - TOL;
}
bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pdd>& hull) {
	std::deque<Line> dq;
	std::sort(HP.begin(), HP.end());
	for (const Line& l : HP) {
		if (!dq.empty() && zero(dq.back() / l)) continue;
		while (dq.size() >= 2 && CW(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
		while (dq.size() >= 2 && CW(l, dq.front(), dq[1])) dq.pop_front();
		dq.push_back(l);
	}
	while (dq.size() >= 3 && CW(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
	while (dq.size() >= 3 && CW(dq.back(), dq.front(), dq[1])) dq.pop_front();
	for (int i = 0; i < dq.size(); i++) {
		Line cur = dq[i], nxt = dq[(i + 1) % (int)dq.size()];
		if (cur / nxt < TOL) {
			hull.clear();
			return 0;
		}
		hull.push_back(intersection(cur, nxt));
	}
	return 1;
}
struct Circle {
	Pdd c;
	ld r;
	Circle(Pdd C = Pdd(0, 0), ld R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator > (const Pdd& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pdd& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pdd& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }// coord trans | check right
	ld A() const { return r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c.x >> c.c.y >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c.x << " " << c.c.y << " " << c.r; return os; }
} INVAL = { { 0, 0 }, -1 };
Circle enclose_circle(const Pdd& u, const Pdd& v) {
	Pdd c = (u + v) * .5;
	return Circle(c, (c - u).mag());
}
Circle enclose_circle(const Pdd& u, const Pdd& v, const Pdd& w) {
	Line l1 = rotate90(L(u, v), (u + v) * .5);
	Line l2 = rotate90(L(v, w), (v + w) * .5);
	if (zero(l1 / l2)) return { { 0, 0 }, -1 };
	Pdd c = intersection(l1, l2);
	ld r = (c - u).mag();
	return Circle(c, r);
}
std::vector<Pii> C;
std::vector<Pdd> poly, vd[LEN];
std::vector<int> gph[LEN];
ld Voronoi_diagram(ld wl, ld wr, ld hd, ld hu, std::vector<Pii> C) {
	int sz = C.size();
	poly.resize(sz);
	for (int i = 0; i < sz; i++) poly[i] = P(C[i]);
	assert(sz);
	if (sz == 1) {}
	else if (sz == 2) {
		gph[0].push_back(1);
		gph[1].push_back(0);
	}
	else {
		std::vector<std::tuple<Pii, Pii, Pii>> dt = Delaunay_triangulation(C);
		for (const std::tuple<Pii, Pii, Pii>& tri : dt) {
			int a = std::get<0>(tri).i;
			int b = std::get<1>(tri).i;
			int c = std::get<2>(tri).i;
			gph[a].push_back(b);
			gph[a].push_back(c);
			gph[b].push_back(a);
			gph[b].push_back(c);
			gph[c].push_back(a);
			gph[c].push_back(b);
		}
	}
	ld ret = 0;
	for (int i = 0; i < N; i++) {
		std::sort(gph[i].begin(), gph[i].end());
		gph[i].resize(unique(gph[i].begin(), gph[i].end()) - gph[i].begin());
		std::vector<Line> HP;
		for (const int& j : gph[i]) {
			Line vec = L(poly[j], poly[i]);
			Line hp = rotate90(~vec.s, (poly[i] + poly[j] * .5));
			HP.push_back(hp);
		}
		HP.push_back(Line(Vec(0, 1), hu));
		HP.push_back(Line(Vec(0, -1), hd));
		HP.push_back(Line(Vec(1, 0), wr));
		HP.push_back(Line(Vec(-1, 0), wl));
		std::vector<Pdd> HPI = {};
		if (!half_plane_intersection(HP, HPI)) continue;
		//vd[i] = HPI;
		for (int j = 0; j < HPI.size(); j++) {
			ld dist = (HPI[j] - poly[i]).mag();
			ret = std::max(ret, dist);
		}
	}
	return ret;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	ld w, h;
	std::cin >> N >> w >> h;
	C.resize(N);
	for (int i = 0; i < N; i++) std::cin >> C[i], C[i].i = i;
	std::cout << Voronoi_diagram(0, w, 0, h, C) << "\n";
	return;
}
int main() { solve(); return 0; }