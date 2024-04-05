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
std::vector<std::tuple<Pii, Pii, Pii>> Delaunay_triangulation(std::vector<Pii> C) {
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
}; const Pdd Odd = { 0, 0 };
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
	return Line(Vec(dy, dx), c);
}
Line L(const Vec& s, const Pdd& p) {
	ld c = s.vy * p.x + s.vx * p.y;
	return Line(s, c);
}
Line rotate(const Line& l, const Pdd& p, ld the) {
	Vec s = l.s;
	ld x = -s.vx, y = s.vy;
	ld vx = -(x * cos(the) - y * sin(the));
	ld vy = x * sin(the) + y * cos(the);
	ld c = vy * p.x + vx * p.y;
	return Line(Vec(vy, vx), c);
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
bool on_seg_strong(const Pdd& d1, const Pdd& d2, const Pdd& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && (ret > 0 || zero(ret));
}
bool intersect(const Pdd& s1, const Pdd& s2, const Pdd& d1, const Pdd& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	//return f1 && f2;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
bool inner_check(const std::vector<Pdd>& H, const Pdd& p) {//concave
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pdd cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (zero(cur.y - nxt.y)) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
ld area(std::vector<Pdd>& H) {
	ld ret = 0;
	int h = H.size();
	for (int i = 0; i < h; i++) {
		Pdd cur = H[i], nxt = H[(i + 1) % h];
		ret += cross(Odd, cur, nxt);
	}
	return ret / 2;
}
bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pdd>& hull) {
	auto cw = [&](const Line& l1, const Line& l2, const Line& target) -> bool {
		if (l1.s / l2.s < TOL) return 0;
		return target.above(intersection(l1, l2)) > -TOL;
		};
	std::deque<Line> dq;
	std::sort(HP.begin(), HP.end());
	for (const Line& l : HP) {
		if (!dq.empty() && zero(dq.back() / l)) continue;
		while (dq.size() >= 2 && cw(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
		while (dq.size() >= 2 && cw(l, dq.front(), dq[1])) dq.pop_front();
		dq.push_back(l);
	}
	while (dq.size() >= 3 && cw(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
	while (dq.size() >= 3 && cw(dq.back(), dq.front(), dq[1])) dq.pop_front();
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
std::vector<Pii> C;
std::vector<Pdd> H;// , vd[LEN];
std::vector<int> seed[LEN];
ld Voronoi_diagram(const ld& wl, const ld& wr, const ld& hd, const ld& hu, std::vector<Pii> C) {
	int sz = C.size();
	assert(sz > 2);
	assert(!zero(area(H)));
	if (sz == 1) {}
	else if (sz == 2) {}
	else {
		std::vector<std::tuple<Pii, Pii, Pii>> dt = Delaunay_triangulation(C);
		for (const std::tuple<Pii, Pii, Pii>& tri : dt) {
			int a = std::get<0>(tri).i;
			int b = std::get<1>(tri).i;
			int c = std::get<2>(tri).i;
			seed[a].push_back(b);
			seed[a].push_back(c);
			seed[b].push_back(a);
			seed[b].push_back(c);
			seed[c].push_back(a);
			seed[c].push_back(b);
		}
	}
	ld ret = 0;
	for (int i = 0; i < sz; i++) {
		std::sort(seed[i].begin(), seed[i].end());
		seed[i].erase(unique(seed[i].begin(), seed[i].end()), seed[i].end());
		std::vector<Line> HP;
		for (const int& j : seed[i]) {
			Line hp = L(~L(H[i], H[j]).s, (H[i] + H[j]) * .5);
			HP.push_back(hp);
		}
		HP.push_back(Line(Vec(0, 1), hu));
		HP.push_back(Line(Vec(0, -1), -hd));
		HP.push_back(Line(Vec(1, 0), wr));
		HP.push_back(Line(Vec(-1, 0), -wl));
		std::vector<Pdd> HPI = {};
		if (!half_plane_intersection(HP, HPI)) continue;
		//vd[i] = HPI;
		for (int j = 0; j < HPI.size(); j++) {
			Pdd p1 = HPI[j], p2 = HPI[(j + 1) % HPI.size()];
			for (int k = 0; k < H.size(); k++) {
				Pdd h1 = H[k], h2 = H[(k + 1) % H.size()];
				if (intersect(p1, p2, h1, h2)) {
					Line l1 = L(p1, p2), l2 = L(h1, h2);
					if (zero(l1 / l2)) continue;
					Pdd inx = intersection(l1, l2);
					ret = std::max(ret, (H[i] - inx).mag());
				}
			}
		}
		std::sort(HPI.begin(), HPI.end(), [&](const Pdd& p, const Pdd& q) -> bool {
			ld a = (H[i] - p).mag();
			ld b = (H[i] - q).mag();
			return a > b;
			});
		for (int j = 0; j < HPI.size(); j++) {
			if (inner_check(H, HPI[j])) {
				ret = std::max(ret, (H[i] - HPI[j]).mag());
				break;
			}
		}
	}
	return ret;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	std::cin >> N;
	C.resize(N);
	H.resize(N);
	ld wl = INF, wr = -INF, hd = INF, hu = -INF;
	for (int i = 0; i < N; i++) {
		std::cin >> C[i];
		C[i].i = i;
		H[i] = P(C[i]);
		wl = std::min(wl, H[i].x - 10.);
		wr = std::max(wr, H[i].x + 10.);
		hd = std::min(hd, H[i].y - 10.);
		hu = std::max(hu, H[i].y + 10.);
	}
	std::cout << Voronoi_diagram(wl, wr, hd, hu, C) << "\n";
	return;
}
int main() { solve(); return 0; }//boj15695 ACM-ICPC World Finals 2018 G Panda Preserve

//Line rotate90(const Line& l, const Pdd& p) {
//	Vec s = ~l.s;
//	ld c = s.vy * p.x + s.vx * p.y;
//	return Line(s, c);
//}
//Line hp = rotate90(L(poly[i], poly[j]), (poly[i] + poly[j]) * .5);


//int ccw(const Pii& d1, const Pii& d2, const Pdd& d3) {
//	ld ret = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//ld dot(const Pii& d1, const Pdd& d2, const Pii& d3) {
//	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
//}
//bool on_seg_strong(const Pii& s1, const Pii& s2, const Pdd p) {
//	ld ret = dot(s1, p, s2);
//	return !ccw(s1, s2, p) && (ret > 0 || zero(ret));
//}
//bool inner_check(const std::vector<Pii>& H, const int& sz, const Pdd& p) {//concave
//	int cnt = 0;
//	for (int i = 0; i < sz; i++) {
//		Pii cur = H[i], nxt = H[(i + 1) % sz];
//		if (on_seg_strong(cur, nxt, p)) return 1;
//		if (cur.y == nxt.y) continue;
//		if (nxt.y < cur.y) std::swap(cur, nxt);
//		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
//		cnt += ccw(cur, nxt, p) > 0;
//	}
//	return cnt & 1;
//}